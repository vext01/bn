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
#include <string.h>

/* #include <readline/readline.h> */

#include "bn.h"

uint8_t			bn_yyll_debug = 1;

int64_t
bn_to_common_signed_64(struct bnum_tok t)
{
	int64_t			val;

	if (t.signd) {
		switch (t.width) {
		case 8:
			val = (int8_t) t.num.int8;
			break;
		case 16:
			val = (int16_t) t.num.int16;
			break;
		case 32:
			val = (int32_t) t.num.int32;
			break;
		default:
			fprintf(stderr, "unkown width\n");
			break;
		};
	} else {
		switch (t.width) {
		case 8:
			val = (uint8_t) t.num.int8;
			break;
		case 16:
			val = (uint16_t) t.num.int16;
			break;
		case 32:
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

	memset(&bnum, 0, sizeof(bnum));
	bnum.signd = signd;
	bnum.width = width;

	switch(width) {
	case 8:
		if (signd) {
			bnum.num.int8 = num;
		} else {
			bnum.num.uint8 = num;
		}
		break;
	case 16:
		if (signd) {
			bnum.num.int16 = num;
		} else {
			bnum.num.uint16 = num;
		}
		break;
	case 32:
		if (signd) {
			bnum.num.int32 = num;
		} else {
			bnum.num.uint32 = num;
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

	for (i = len-1; i >= 0; i--) { /* XXX little endian only */
		printf("%02x\n", bytes[i]);
		snprintf(hex, hex_len, "%s%02x", hex, bytes[i]);
	}

	return (hex);
}

/* return a heap allocated type string from a bnum */
char *
bn_type_strng(struct bnum_tok t)
{
	char		*ret;

	if (t.signd)
		asprintf(&ret, "int%d_t", (int) t.width);
	else
		asprintf(&ret, "uint%d_t", (int) t.width);

	if (!ret)
		fprintf(stderr, "asprintf failed\n");

	return (ret);
}

void
bn_print(struct bnum_tok bn)
{
	int64_t		 val = bn_to_common_signed_64(bn);
	char		*hex;
	char		*type = bn_type_strng(bn);

	switch (bn.width) {
	case 8:
		hex = bn_bytes_to_hex((unsigned char *) &bn.num.uint8, 1);
		if (bn.signd)
			printf("  (%s): %d  0x%s  %o\n",
			    type, bn.num.int8, hex, bn.num.int8);
		else
			printf("  (%s): %d  0x%s  %o\n",
			    type, bn.num.uint8, hex, bn.num.uint8);

		break;
	case 16:
		hex = bn_bytes_to_hex((unsigned char *) &bn.num.uint16, 2);
		if (bn.signd)
			printf("  (%s): %d  0x%s  %o\n",
			    type, bn.num.int16, hex, bn.num.int16);
		else
			printf("  (%s): %d  0x%s  %o\n",
			    type, bn.num.uint16, hex, bn.num.uint16);

		break;
	case 32:
		hex = bn_bytes_to_hex((unsigned char *) &bn.num.uint32, 4);
		if (bn.signd)
			printf("  (%s): %d  0x%s  %o\n",
			    type, bn.num.int32, hex, bn.num.int32);
		else
			printf("  (%s): %d  0x%s  %o\n",
			    type, bn.num.uint32, hex, bn.num.uint32);

		break;

	default:
		fprintf(stderr, "Cant print integer of this width\n");
	};

	free(hex);
	free(type);
}

int
main(void)
{
	/* char			*line = 0; */
	union bnum		 bn;

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

	while (!feof(stdin)) {
		printf("> ");
		yyparse();
	}
	printf("\n");

	return (EXIT_SUCCESS);

}
