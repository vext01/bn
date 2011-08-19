#include <stdio.h>
#include <stdlib.h>

#include <readline/readline.h>

#include "bn.h"

struct bnum_tok *
bn_add(struct bnum_tok *a, struct bnum_tok *b)
{
	/* XXX fill in */
	switch (a->width) {
	case 1:
		break;
	case 2:
		break;
	case 4:
		switch (b->width) {
		case 1:
			break;
		case 2:
			break;
		case 4:
			printf("ACK!\n");
			if (a->signd && b->signd)
				a->num.int32 += b->num.int32;
			else if (a->signd && (!b->signd))
				a->num.int32 += b->num.uint32;
			else if ((!a->signd) && b->signd)
				a->num.uint32 += b->num.int32;
			else
				a->num.uint32 += b->num.uint32;
			break;
		case 8:
			break;
		};

		break;
	case 8:
		break;
	}

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
