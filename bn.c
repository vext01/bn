#include <stdio.h>
#include <stdlib.h>

#include <readline/readline.h>

#include "bn.h"

struct bnum_tok *
new_bnum_tok(int64_t num, uint8_t width, uint8_t signd)
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
bn_print(struct bnum_tok *bn)
{
	printf("%-10s: %u\n", "uint8_t", bn->num.uint8);
	printf("%-10s: %d\n", "int8_t", bn->num.int8);
	printf("%-10s: %u\n", "uint16_t", bn->num.uint16);
	printf("%-10s: %d\n", "int16_t", bn->num.int16);
	printf("%-10s: %u\n", "uint32_t", bn->num.uint32);
	printf("%-10s: %d\n", "int32_t", bn->num.int32);
	printf("%-10s: %llu\n", "uint64_t", bn->num.uint64);
	printf("%-10s: %lld\n", "int64_t", bn->num.int64);

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
