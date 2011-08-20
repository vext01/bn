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

#include <stdio.h>
#include <stdlib.h>

#include <readline/readline.h>

#include "bn.h"

struct bnum_tok *
bn_cast(struct bnum_tok *t, struct bn_cast c)
{
	struct bnum_tok		*new;
	int64_t			val;

	if (t->signd) {
		switch (t->width) {
		case 1:
			val = (int8_t) t->num.int8;
			break;
		case 2:
			val = (int16_t) t->num.int16;
			break;
		case 4:
			val = (int32_t) t->num.int32;
			break;
		default:
			fprintf(stderr, "unkown width\n");
			break;
		};
	} else {
		switch (t->width) {
		case 1:
			val = (uint8_t) t->num.int8;
			break;
		case 2:
			val = (uint16_t) t->num.int16;
			break;
		case 4:
			val = (uint32_t) t->num.int32;
			break;
		default:
			fprintf(stderr, "unkown width\n");
			break;
		};
	}

	return (bn_new_bnum_tok(val, c.width, c.signd));
}

#define ADD_BLK(AAA, BBB)		\
			if (a->signd && b->signd) \
				a->num.int##AAA += b->num.int##BBB; \
			else if (a->signd && (!b->signd)) \
				a->num.int##AAA += b->num.uint##BBB; \
			else if ((!a->signd) && b->signd) \
				a->num.uint##AAA += b->num.int##BBB; \
			else \
				a->num.uint##AAA += b->num.uint##BBB; \
			break;

struct bnum_tok *
bn_add(struct bnum_tok *a, struct bnum_tok *b)
{
	switch (a->width) {
	case 1:
		switch (b->width) {
		case 1:
			ADD_BLK(8, 8);
			break;
		case 2:
			ADD_BLK(8, 16);
			break;
		case 4:
			ADD_BLK(8, 32);
			break;
		case 8:
			ADD_BLK(8, 64);
			break;
		};
		break;
	case 2:
		switch (b->width) {
		case 1:
			ADD_BLK(16, 8);
			break;
		case 2:
			ADD_BLK(16, 16);
			break;
		case 4:
			ADD_BLK(16, 32);
			break;
		case 8:
			ADD_BLK(16, 64);
			break;
		};
		break;
	case 4:
		switch (b->width) {
		case 1:
			ADD_BLK(32, 8);
			break;
		case 2:
			ADD_BLK(32, 16);
			break;
		case 4:
			ADD_BLK(32, 32);
			break;
		case 8:
			ADD_BLK(32, 64);
			break;
		};
		break;
	case 8:
		switch (b->width) {
		case 1:
			ADD_BLK(64, 8);
			break;
		case 2:
			ADD_BLK(64, 16);
			break;
		case 4:
			ADD_BLK(64, 32);
			break;
		case 8:
			ADD_BLK(64, 64);
			break;
		};
		break;
	};

	return (a);
}

struct bnum_tok *
bn_new_bnum_tok(int64_t num, uint8_t width, uint8_t signd)
{
	struct bnum_tok		*bnum;

	bnum = calloc(1, sizeof(struct bnum_tok));
	if (!bnum)
		fprintf(stderr, "malloc fail\n");

	bnum->signd = signd;
	bnum->width = width;

	switch(width) {
	case 1:
		fprintf(stderr, "width = 8\n");
		if (signd) {
			bnum->num.int8 = num;
		} else {
			bnum->num.uint8 = num;
		}
		break;
	case 2:
		fprintf(stderr, "width = 16\n");
		if (signd) {
			bnum->num.int16 = num;
		} else {
			bnum->num.uint16 = num;
		}
		break;
	case 4:
		fprintf(stderr, "width = 32\n");
		if (signd) {
			bnum->num.int32 = num;
		} else {
			bnum->num.uint32 = num;
		}
		break;
	case 8:
		fprintf(stderr, "width = 64\n");
		if (signd) {
			bnum->num.int64 = num;
		} else {
			bnum->num.uint64 = num;
		}
		break;
	default:
		fprintf(stderr, "unknown number width\n");
	};

	return (bnum);
}

void
bn_possibly_print_result_arrow(struct bnum_tok *b, uint8_t width, uint8_t signd)
{
	if ((b->width == width) && (b->signd == signd))
		printf("> ");
	else
		printf("  ");
}

void
bn_print(struct bnum_tok *bn)
{
	bn_possibly_print_result_arrow(bn, 1, 0);
	printf("%-10s: %-16u\n", "uint8_t", bn->num.uint8);

	bn_possibly_print_result_arrow(bn, 1, 1);
	printf("%-10s: %-16d\n", "int8_t", bn->num.int8);

	bn_possibly_print_result_arrow(bn, 2, 0);
	printf("%-10s: %-16u\n", "uint16_t", bn->num.uint16);

	bn_possibly_print_result_arrow(bn, 2, 1);
	printf("%-10s: %-16d\n", "int16_t", bn->num.int16);

	bn_possibly_print_result_arrow(bn, 4, 0);
	printf("%-10s: %-16u\n", "uint32_t", bn->num.uint32);

	bn_possibly_print_result_arrow(bn, 4, 1);
	printf("%-10s: %-16d\n", "int32_t", bn->num.int32);

	bn_possibly_print_result_arrow(bn, 8, 0);
	printf("%-10s: %-16llu\n", "uint64_t", bn->num.uint64);

	bn_possibly_print_result_arrow(bn, 8, 1);
	printf("%-10s: %-16lld\n", "int64_t", bn->num.int64);
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
