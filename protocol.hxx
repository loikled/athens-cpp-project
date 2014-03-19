/*
 * proto.h
 *
 * copyright (2012) Benoit Gschwind
 *
 */

#ifndef PROTO_H_
#define PROTO_H_

// needed to use RTTI
#include <typeinfo>


#include <cassert>
#include <stdexcept>
#include <sys/fcntl.h>
#include <stdint.h>
#include <vector>
#include <algorithm>
#include <iostream>

typedef unsigned char byte;

enum message_type_e {
	MSG_TYPE_NOP                        = 0,
	MSG_TYPE_PUBLIC                     = 1,
	MSG_TYPE_CHANGE_NICK                = 2,
	MSG_TYPE_PRIVATE                    = 3,
	MSG_TYPE_ACKNOWLEDGE_CHANGE_NICK    = 4,
	MSG_TYPE_MESSAGE                    = 5,
	MSG_TYPE_JOIN                       = 6,
	MSG_TYPE_PING                       = 7,
	MSG_TYPE_PONG                       = 8
};

struct message_header_t {
	uint32_t length;
	uint32_t type; /* message_type_e */
	uint16_t body_checksum;
	uint16_t head_checksum;
} __attribute__((packed)); /* do not use padding for alignment */

/* source : http://en.wikipedia.org/wiki/Fletcher%27s_checksum */
inline  uint16_t fletcher16(uint8_t const * data, int count) {
	uint16_t sum1 = 0;
	uint16_t sum2 = 0;
	int index;
	for (index = 0; index < count; ++index) {
		sum1 = (sum1 + data[index]) % 255;
		sum2 = (sum2 + sum1) % 255;
	}
	return (sum2 << 8) | sum1;
}



#endif /* PROTO_H_ */
