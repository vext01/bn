/*
 * Copyright (c) 2011, Edd Barrett <vext01@gmail.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef BN_H_
#define BN_H_

union bnum {
	uint8_t		uint8;
	int8_t		int8;
	uint16_t	uint16;
	int16_t		int16;
	uint32_t	uint32;
	int32_t		int32;
	uint64_t	uint64;
	int64_t		int64;
};

struct bnum_tok {
	union bnum	num;
	uint8_t		width;
	uint8_t		signd;
};

struct bn_cast {
	uint8_t		width; /* bytes */
	uint8_t		signd;
};

struct bnum_tok	bn_new_bnum_tok(int64_t num, uint8_t width, uint8_t signd);
struct bnum_tok	bn_add(struct bnum_tok a, struct bnum_tok b);
struct bnum_tok bn_cast(struct bnum_tok t, struct bn_cast c);

#endif
