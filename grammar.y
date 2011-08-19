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
#include "bn.h"

%}

%start start

%token PLUS MINUS
%token NUM
%token CAST8 CASTU8
%token CAST16 CASTU16
%token CAST32 CASTU32
%token CAST64 CASTU64
%token END

%union {
	struct bnum_tok		*numt;
	uint64_t		 rawnum;
}

%left PLUS
%left MINUS

%type <rawnum> NUM
%type <numt> num
%type <numt> expr

%%

start: expr END		{ bn_print($1); YYACCEPT;}
     ;

expr: num		{ $$ = $1; yydbg("num");}
    | num PLUS expr	{ $$ = bn_add($1, $3); yydbg("num PLUS num");}

num: NUM		{ $$ = bn_new_bnum_tok($1, sizeof(int), 1); yydbg("NUM");}
   | CAST8 NUM		{ $$ = bn_new_bnum_tok($2, 1, 1); yydbg("NUM");}
   | CASTU8 NUM		{ $$ = bn_new_bnum_tok($2, 1, 0); yydbg("NUM");}
   | CAST16 NUM		{ $$ = bn_new_bnum_tok($2, 2, 1); yydbg("NUM");}
   | CASTU16 NUM	{ $$ = bn_new_bnum_tok($2, 2, 0); yydbg("NUM");}
   | CAST32 NUM		{ $$ = bn_new_bnum_tok($2, 4, 1); yydbg("NUM");}
   | CASTU32 NUM	{ $$ = bn_new_bnum_tok($2, 4, 0); yydbg("NUM");}
   | CAST64 NUM		{ $$ = bn_new_bnum_tok($2, 8, 1); yydbg("NUM");}
   | CASTU64 NUM	{ $$ = bn_new_bnum_tok($2, 8, 0); yydbg("NUM");}

%%

void yyerror(const char *e)
{
	printf("yyerror: %s\n", e);
}

void
yydbg(const char *msg)
{
	//if (l_debug)
		fprintf(stderr, "YYDBG: %s\n", msg);
}
