/* ------------------
 * Logic2CNF
 * $Id: grammar.l 504 2010-06-10 15:01:13Z eb771 $
 *
 * Copyright (c) 2010, Edd Barrett (University of Kent)
 * <eddbarrett at googlemail dot com>
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

#include "bn.h"
#include "y.tab.h"

//[0-9]*		{ yylval.num.int64 = strtoll(yytext, NULL, 0); return NUM; }

/* not using the built in one because it makes noise about unused funcs */
int	my_yylineno = 1;
char	*err_tmp;	/* temproary buffer for building a yyerror string */

%}

%option nounput
%option noyywrap

%%

[\t ]		{}
"-"		{ return MINUS; }
"+"		{ return PLUS; }
"\n"		{ return END; }
-*[0-9]*		{ yylval.rawnum = strtoimax(yytext, NULL, 0); return NUM; }
-*0x[0-9a-f]*	{ yylval.rawnum = strtoimax(yytext, NULL, 0); return NUM; }
"(8)"		{ return CAST8; }
"(u8)"		{ return CASTU8; }
"(16)"		{ return CAST16; }
"(u16)"		{ return CASTU16; }
"(32)"		{ return CAST32; }
"(u32)"		{ return CASTU32; }
"(64)"		{ return CAST64; }
"(u64)"		{ return CASTU64; }
.		{
			fprintf(stderr, "syntax error\n");
		}

%%
