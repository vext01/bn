/* ------------------
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

#if 0
%type <node> expr
%type <chars> LIT 
%type <node> start
%type <chars> REP_CMMT
#endif

%}

%start start

%token PLUS MINUS
%token NUM
%token CAST8 CASTU8
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
    | num PLUS num	{ yydbg("num PLUS num");}

num: NUM		{ $$ = new_bnum_tok($1, sizeof(int), 1); yydbg("NUM");}
   | CAST8 NUM		{ $$ = new_bnum_tok($2, 1, 1); yydbg("NUM");}
   | CASTU8 NUM		{ $$ = new_bnum_tok($2, 1, 0); yydbg("NUM");}
   | CAST64 NUM		{ $$ = new_bnum_tok($2, 8, 1); yydbg("NUM");}
   | CASTU64 NUM	{ $$ = new_bnum_tok($2, 8, 0); yydbg("NUM");}


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
