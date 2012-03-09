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

#include <sys/types.h>

#include <err.h>
#include <errno.h>
#include <limits.h>
#include <regex.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bn.h"
#include "linenoise/linenoise.h"

int
bn_to_common_signed_64(int64_t *val, struct bn_num *t)
{
	if (t->signd) {
		switch (t->width) {
		case 8:
			*val = (int8_t) t->num.int8;
			break;
		case 16:
			*val = (int16_t) t->num.int16;
			break;
		case 32:
			*val = (int32_t) t->num.int32;
			break;
		default:
			fprintf(stderr, "unkown width\n");
			return (-1);
			break;
		};
	} else {
		switch (t->width) {
		case 8:
			*val = (uint8_t) t->num.int8;
			break;
		case 16:
			*val = (uint16_t) t->num.int16;
			break;
		case 32:
			*val = (uint32_t) t->num.int32;
			break;
		default:
			fprintf(stderr, "unkown width\n");
			return (-1);
			break;
		};
	}

	return (0);
}

#if 0
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

	printf("(%s %d) + \n", a.signd ? "signed" : "unsigned", a.width);
	printf("(%s %d)", b.signd ? "signed" : "unsigned", b.width);

	/* https://www.securecoding.cert.org/confluence/display/seccode/INT02-C.+Understand+integer+conversion+rules */
	if (a.signd == b.signd) {
		if (a.width == b.width)
			return (bn_new_bnum_tok(val_a + val_b, a.width, a.signd));
		else if (a.width > b.width)
			return (bn_new_bnum_tok(val_a + val_b, a.width, a.signd));
		else
			return (bn_new_bnum_tok(val_a + val_b, b.width, a.signd));
	} else {
		if ((!a.signd) && (a.width >= b.width))
			return (bn_new_bnum_tok(val_a + val_b, a.width, 0));
		else if ((!b.signd) && (b.width >= a.width))
			return (bn_new_bnum_tok(val_a + val_b, b.width, 0));
		else if ((a.signd) && (a.width > b.width))
			return (bn_new_bnum_tok(val_a + val_b, a.width, 1));
		else if ((b.signd) && (b.width > a.width))
			return (bn_new_bnum_tok(val_a + val_b, b.width, 1));
		else if (a.signd)
			return (bn_new_bnum_tok(val_a + val_b, a.width, 0));
		else if (b.signd)
			return (bn_new_bnum_tok(val_a + val_b, b.width, 0));
		else
			fprintf(stderr, "ERROR: impossible integer conversion\n");
	}
}
#endif

int
bn_new_bn_num(struct bn_num *bnum, int64_t num, uint8_t width, uint8_t signd)
{
	if (num > INT32_MAX)
		fprintf(stderr, "number was too large\n");

	memset(bnum, 0, sizeof(bnum));
	bnum->tok.type = BN_TYPE_NUM;
	bnum->signd = signd;
	bnum->width = width;

	switch(width) {
	case 8:
		if (signd) {
			bnum->num.int8 = num;
		} else {
			bnum->num.uint8 = num;
		}
		break;
	case 16:
		if (signd) {
			bnum->num.int16 = num;
		} else {
			bnum->num.uint16 = num;
		}
		break;
	case 32:
		if (signd) {
			bnum->num.int32 = num;
		} else {
			bnum->num.uint32 = num;
		}
		break;
	default:
		fprintf(stderr, "unknown number width\n");
	};

	return (0);
}


/*
 * Caller must free. (modified from HGD)
 */
int
bn_num_to_hex(char **hex, struct bn_num *b)
{
	int i, hex_len, len;

	len = b->width / 8;
	hex_len = len * 2 + 1;
	*hex = xmalloc(hex_len); /* two hex chars for each byte */

	memset(*hex, 0, hex_len);
	for (i = len-1; i >= 0; i--) {
		snprintf(*hex, hex_len, "%s%02x",
		    *hex, ((unsigned char *) (&(b->num.uint64)))[i]);
	}

	return (0);
}

int
bn_type_string(char **ret, struct bn_num *t)
{
	if (t->signd)
		xasprintf(ret, "int%d_t", (int) t->width);
	else
		xasprintf(ret, "uint%d_t", (int) t->width);

	if (!*ret) {
		fprintf(stderr, "asprintf failed\n");
		return (-1);
	}

	return (0);
}

void
bn_to_bin(char **ret, struct bn_num *bn)
{
	int			 i;
	char			*ret_d;

	*ret = xmalloc(bn->width + 1);
	ret_d = *ret;

	for (i = 0; i < bn->width; i++) {
		switch (bn->width) {
		case 8:
			if (bn->signd)
				ret_d[bn->width - i - 1] = ((bn->num.int8 >> i) & 0x1) ? '1' : '0';
			else
				ret_d[bn->width - i - 1] = ((bn->num.uint8 >> i) & 0x1) ? '1' : '0';
			break;
		case 16:
			if (bn->signd)
				ret_d[bn->width - i - 1] = ((bn->num.int16 >> i) & 0x1) ? '1' : '0';
			else
				ret_d[bn->width - i - 1] = ((bn->num.uint16 >> i) & 0x1) ? '1' : '0';
			break;
		case 32:
			if (bn->signd)
				ret_d[bn->width - i - 1] = ((bn->num.int32 >> i) & 0x1) ? '1' : '0';
			else
				ret_d[bn->width - i - 1] = ((bn->num.uint32 >> i) & 0x1) ? '1' : '0';
			break;
		}
	}

	ret_d[bn->width] = 0;
}

#define BN_OUTPUT_FMT	"  (%6s):  (%8d)  0x(%16s)  0(%8o)  0b(%s)\n"
void
bn_print_num(struct bn_num *bn)
{
	int64_t		 val;
	char		*hex, *bin;
	char		*type;

	val = bn->num.int64;
	bn_type_string(&type, bn);
	bn_to_bin(&bin, bn);
	bn_num_to_hex(&hex, bn);

	switch (bn->width) {
	case 8:
		if (bn->signd)
			printf(BN_OUTPUT_FMT,
			    type, bn->num.int8, hex, bn->num.int8, bin);
		else
			printf(BN_OUTPUT_FMT,
			    type, bn->num.uint8, hex, bn->num.uint8, bin);
		break;
	case 16:
		if (bn->signd)
			printf(BN_OUTPUT_FMT,
			    type, bn->num.int16, hex, bn->num.int16, bin);
		else
			printf(BN_OUTPUT_FMT,
			    type, bn->num.uint16, hex, bn->num.uint16, bin);

		break;
	case 32:
		if (bn->signd)
			printf(BN_OUTPUT_FMT,
			    type, bn->num.int32, hex, bn->num.int32, bin);
		else
			printf(BN_OUTPUT_FMT,
			    type, bn->num.uint32, hex, bn->num.uint32, bin);

		break;

	default:
		fprintf(stderr, "Cant print integer of this width\n");
	};

	free(bin);
	free(hex);
	free(type);
}

void *
xmalloc(size_t sz)
{
	void			*ptr;

	ptr = malloc(sz);
	if (!ptr)
		errx(errno, "malloc");

	return (ptr);

}


void *
xcalloc(size_t sz, size_t size)
{
	void			*ptr;

	ptr = calloc(sz, size);
	if (!ptr)
		errx(errno, "calloc");

	return (ptr);
}

void *
xrealloc(void *old_p, size_t sz)
{
	void			*ptr;

	ptr = realloc(old_p, sz);
	if (!ptr)
		errx(errno, "realloc");

	return (ptr);
}

char *
xstrdup(const char *s)
{
	char *dup = strdup(s);

	if (dup == NULL)
		errx(errno, "strdup");

	return (dup);
}

int
xasprintf(char **buf, char *fmt, ...)
{
	va_list			ap;
	int			ret;

	va_start(ap, fmt);
	ret = vasprintf(buf, fmt, ap);

	if (ret == -1)
		errx(errno, "vasprintf");

	return (ret);
}

int
bn_parse_number_to_int64(char *num_str, int64_t *numpart)
{
	char			*ep;

	*numpart = strtoll(num_str, &ep, 0);
	if (num_str[0] == '\0' || *ep != '\0') {
		fprintf(stderr, "NaN\n"); /* XXX clean up */
		return (-1);
	}
	if (errno == ERANGE && (*numpart == LONG_MAX || *numpart == LONG_MIN)) {
		fprintf(stderr, "Range error\n"); /* XXX clean up */
		return (-1);
	}

	return (0);
}


/*
 * keep as simple as possible -- no lex/yacc plz
 */
#define RE_WHITE		"[ \t\r\n]*"
int
bn_parse(char *line, struct bn_tok **b)
{
	regex_t			 rx1; /* rx2; */
	regmatch_t		 matches[3];
	char			*tok1; /* *tok2; */
	int64_t			 numpart;
	struct bn_num		*b_num;

	/* form 1: num */
	if (regcomp(&rx1, RE_WHITE "[0-9]+" RE_WHITE, REG_EXTENDED))
		errx(1, "regcomp");

	/* form 2: cast num */
#if 0
	if (regcomp(&rx2, RE_WHITE "\\(.*\\)" RE_WHITE "[0-9]+" RE_WHITE, REG_EXTENDED))
		errx(1, "regcomp");
#endif

	/* XXX: form 3: operators */

	if (regexec(&rx1, line, 1, matches, 0) == 0) {

		tok1 = xcalloc(1, matches[0].rm_eo - matches[0].rm_so + 1);
		strncpy(tok1, &line[matches[0].rm_so], matches[0].rm_eo - matches[0].rm_so);

		/* parse number */
		if (bn_parse_number_to_int64(tok1, &numpart) < 0)
			return (-1); /* XXX clean up */


		b_num = calloc(1, sizeof(struct bn_num));
		bn_new_bn_num(b_num, numpart, 32, 1); /* default: signed 32  */
		free(tok1);

		*b = (struct bn_tok *) b_num;
		return (0);
	}

	return (-1);

#if 0
	if (regexec(&rx2, line, 2, matches, 0) == 0) {

		tok1 = xcalloc(1, matches[0].rm_eo - matches[0].rm_so + 1);
		strncpy(tok1, &line[matches[0].rm_so], matches[0].rm_eo - matches[0].rm_so);

		tok2 = xcalloc(1, matches[1].rm_eo - matches[1].rm_so + 1);
		strncpy(tok2, &line[matches[1].rm_so], matches[1].rm_eo - matches[1].rm_so);

		printf("CAST NUM: %s %s\n", tok1, tok2);
		b = b; /* XXX */

		free(tok1);
		free(tok2);
	}

	return (0);
#endif
}

int
main(void)
{
	char			*line;
	struct bn_tok		*tok;
	struct bn_num		*bnum;

	while((line = linenoise("bn> ")) != NULL) {
		if (line[0] != '\0') {
			linenoiseHistoryAdd(line);
		}
		if (bn_parse(line, &tok) < 0) {
			fprintf(stderr, "*parse error\n");
			free(line);
			continue;
		}

		if (tok->type == BN_TYPE_NUM) {
			bnum = (struct bn_num *) tok;
			bn_print_num(bnum);
		}

		free(line);
	}

	return (EXIT_SUCCESS);

}
