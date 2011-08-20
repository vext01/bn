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
 * This file was based upon code from Logic2CNF, original license follows:
 */

/*
 * Logic2CNF
 * $Id: grammar.y 524 2010-08-18 09:38:13Z eb771 $
 *
 * Copyright (c) 2010, Edd Barrett (University of Kent)
 * <e.barrett@kent.ac.uk>
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

%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "bn.h"
#include "y.tab.h"

/* not using the built in one because it makes noise about unused funcs */
int	my_yylineno = 1;
char	*err_tmp;	/* temproary buffer for building a yyerror string */

void
l(char *m)
{
	if (bn_yyll_debug)
		fprintf(stderr, "lex: %s\n", m);
}

%}

%option nounput
%option noyywrap

%%

[\r\t ]		{ l("ignore"); }
"-"		{ l("MINUS"); return MINUS; }
"+"		{ l("PLUS"); return PLUS; }
"\n"		{ l("END"); return END; }
"(int8_t)"	{ l("CAST8"); return CAST8; }
"(uint8_t)"	{ l("CASTU8"); return CASTU8; }
"(int16_t)"	{ l("CAST16"); return CAST16; }
"(uint16_t)"	{ l("CASTU16"); return CASTU16; }
"(int32_t)"	{ l("CAST32"); return CAST32; }
"(uint32_t)"	{ l("CASTU32"); return CASTU32; }
-*[0-9]*	{ l("RAWNUM");
		  yylval.yd_rawnum = strtoimax(yytext, NULL, 0);
		  return RAWNUM; }
-*0x[0-9a-f]*	{ l("RAWNUM (hex)");
		  yylval.yd_rawnum = strtoimax(yytext, NULL, 0);
		  return RAWNUM; }
"("		{ l("LBRA"); return LBRA; }
")"		{ l("RBRA"); return RBRA; }
.		{ l("Syntax Error"); }

%%

