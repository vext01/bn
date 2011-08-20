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
 *
 * Some code borrowed from HGD, original license and copyright follows.
 */

/*
 * Copyright (c) 2011, Edd Barrett <vext01@gmail.com>
 * Copyright (c) 2011, Martin Ellis <ellism88@gmail.com>
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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <readline/readline.h>

#include "bn.h"

int64_t
bn_to_common_signed_64(struct bnum_tok t)
{
	int64_t			val;

	if (t.signd) {
		switch (t.width) {
		case 1:
			val = (int8_t) t.num.int8;
			break;
		case 2:
			val = (int16_t) t.num.int16;
			break;
		case 4:
			val = (int32_t) t.num.int32;
			break;
		default:
			fprintf(stderr, "unkown width\n");
			break;
		};
	} else {
		switch (t.width) {
		case 1:
			val = (uint8_t) t.num.int8;
			break;
		case 2:
			val = (uint16_t) t.num.int16;
			break;
		case 4:
			val = (uint32_t) t.num.int32;
			break;
		default:
			fprintf(stderr, "unkown width\n");
			break;
		};
	}

	return (val);
}

struct bnum_tok
bn_cast(struct bnum_tok t, struct bn_cast c)
{
	int64_t			val = bn_to_common_signed_64(t);

	return (bn_new_bnum_tok(val, c.width, c.signd));
}

struct bnum_tok
bn_add(struct bnum_tok a, struct bnum_tok b)
{
	int64_t			val_a = bn_to_common_signed_64(a);
	int64_t			val_b = bn_to_common_signed_64(b);

	/* we return the type of the lhs, think that is right */
	return (bn_new_bnum_tok(val_a + val_b, a.width, a.signd));
}

struct bnum_tok
bn_new_bnum_tok(int64_t num, uint8_t width, uint8_t signd)
{
	struct bnum_tok		bnum;

	if (num > INT32_MAX)
		fprintf(stderr, "number was too large\n");

	memset(&bnum, 0, 8);
	bnum.signd = signd;
	bnum.width = width;

	switch(width) {
	case 1:
		fprintf(stderr, "width = 8\n");
		if (signd) {
			bnum.num.int8 = num;
		} else {
			bnum.num.uint8 = num;
		}
		break;
	case 2:
		fprintf(stderr, "width = 16\n");
		if (signd) {
			bnum.num.int16 = num;
		} else {
			bnum.num.uint16 = num;
		}
		break;
	case 4:
		fprintf(stderr, "width = 32\n");
		if (signd) {
			bnum.num.int32 = num;
		} else {
			bnum.num.uint32 = num;
		}
		break;
	case 8:
		fprintf(stderr, "width = 64\n");
		if (signd) {
			bnum.num.int64 = num;
		} else {
			bnum.num.uint64 = num;
		}
		break;
	default:
		fprintf(stderr, "unknown number width\n");
	};

	return (bnum);
}


/*
 * Turn bytes into hex for storage.
 * Caller must free. (this function is from HGD)
 */
char *
bn_bytes_to_hex(unsigned char *bytes, int len)
{
	char *hex;
	int i, hex_len;

	hex_len = len * 2 + 1;
	hex = malloc(hex_len); /* two hex chars for each byte */

	if (!hex)
		fprintf(stderr, "malloc crapped out\n");

	memset(hex, 0, hex_len);

	for (i = 0; i < len; i++)
		snprintf(hex, hex_len, "%s%02x", hex, bytes[i]);

	return (hex);
}

void
bn_print(struct bnum_tok bn)
{
	int64_t		 val = bn_to_common_signed_64(bn);
	char		*hex = bn_bytes_to_hex((unsigned char *) &val, bn.width);

	/* XXX print the actual type like C */
	printf("(%d %s bytes)  %d  0x%s\n", bn.width, bn.signd ? "signed" : "unsigned", bn.num.uint8, hex);

	free(hex);
}

int
main(void)
{
	char			*line = 0;
	union bnum		 bn;

	printf("sizeof int = %d\n", sizeof(int));
	printf("sizeof long = %d\n", sizeof(long));
	printf("sizeof long long = %d\n", sizeof(long long));
	printf("sizeof short = %d\n", sizeof(short));

	/* XXX can we bolt on readline? */
#if 0
	while ((line = readline("> ")) != NULL) {
		if (*line) {
			bn.int64 = strtoimax(line, NULL, 0);
			bn_print(bn);
		}
		free(line);
	}
#endif

	yyparse();

	return (EXIT_SUCCESS);

}
