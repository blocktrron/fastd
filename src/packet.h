/*
  Copyright (c) 2012, Matthias Schiffer <mschiffer@universe-factory.net>
  Partly based on QuickTun Copyright (c) 2010, Ivo Smits <Ivo@UCIS.nl>.
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


#ifndef _FASTD_PACKET_H_
#define _FASTD_PACKET_H_

#include <stdint.h>


typedef enum _fastd_packet_type {
	PACKET_UNKNOWN = 0,
	PACKET_HANDSHAKE,
	PACKET_DATA,
} fastd_packet_type;

typedef enum _fastd_handshake_record_type {
	RECORD_HANDSHAKE_TYPE = 0,
	RECORD_REPLY_CODE,
	RECORD_ERROR_DETAIL,
	RECORD_FLAGS,
	RECORD_MODE,
	RECORD_PROTOCOL_NAME,
	RECORD_MAX,
} fastd_handshake_record_type;

typedef enum _fastd_handshake_type {
	HANDSHAKE_REQUEST = 0,
	HANDSHAKE_REPLY,
	HANDSHAKE_REHANDSHAKE_REQUEST,
	HANDSHAKE_MAX,
} fastd_handshake_type;

typedef enum _fastd_reply_code {
	REPLY_SUCCESS = 0,
	REPLY_MANDATORY_MISSING,
	REPLY_UNACCEPTABLE_VALUE,
	REPLY_MAX,
} fastd_reply_code;


typedef struct __attribute__ ((__packed__)) _fastd_packet {
	uint8_t req_id;
	uint16_t rsv;
	uint8_t tlv_data[];
} fastd_packet;

#endif /* _FASTD_PACKET_H_ */
