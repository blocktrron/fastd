/*
  Copyright (c) 2012-2013, Matthias Schiffer <mschiffer@universe-factory.net>
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice,
       this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#include "common.h"


void fastd_method_common_init(fastd_context_t *ctx, fastd_method_common_t *session, bool initiator) {
	memset(session, 0, sizeof(*session));

	session->valid_till = ctx->now;
	session->valid_till.tv_sec += ctx->conf->key_valid;

	session->refresh_after = ctx->now;
	session->refresh_after.tv_sec += ctx->conf->key_refresh - fastd_rand(ctx, 0, ctx->conf->key_refresh_splay);

	if (initiator) {
		session->send_nonce[0] = 3;
	}
	else {
		session->send_nonce[0] = 2;
		session->receive_nonce[0] = 1;
	}
}

bool fastd_method_is_nonce_valid(fastd_context_t *ctx, const fastd_method_common_t *session, const uint8_t nonce[COMMON_NONCEBYTES], int64_t *age) {
	if ((nonce[0] & 1) != (session->receive_nonce[0] & 1))
		return false;

	int i;
	*age = 0;

	for (i = COMMON_NONCEBYTES-1; i >= 0; i--) {
		*age *= 256;
		*age += session->receive_nonce[i]-nonce[i];
	}

	*age /= 2;

	if (*age >= 0) {
		if (timespec_diff(&ctx->now, &session->receive_last) > (int)ctx->conf->reorder_time*1000)
			return false;

		if (*age > ctx->conf->reorder_count)
			return false;
	}

	return true;
}

bool fastd_method_reorder_check(fastd_context_t *ctx, fastd_peer_t *peer, fastd_method_common_t *session, const uint8_t nonce[COMMON_NONCEBYTES], int64_t age) {
	if (age < 0) {
		session->receive_reorder_seen >>= age;
		session->receive_reorder_seen |= (1 >> (age+1));
		memcpy(session->receive_nonce, nonce, COMMON_NONCEBYTES);
		session->receive_last = ctx->now;
		return true;
	}
	else if (age == 0 || session->receive_reorder_seen & (1 << (age-1))) {
		pr_debug(ctx, "dropping duplicate packet from %P (age %u)", peer, (unsigned)age);
		return false;
	}
	else {
		pr_debug2(ctx, "accepting reordered packet from %P (age %u)", peer, (unsigned)age);
		session->receive_reorder_seen |= (1 << (age-1));
		return true;
	}
}