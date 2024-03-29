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
%token RAWNUM
%token CAST8 CASTU8
%token CAST16 CASTU16
%token CAST32 CASTU32
%token LBRA RBRA
%token END

%union {
	struct bnum_tok		 yd_num;
	uint64_t		 yd_rawnum;
	struct bn_cast		 yd_cast;
}

%left MINUS PLUS

%type <yd_rawnum> RAWNUM
%type <yd_num> num
%type <yd_num> expr
%type <yd_num> expr1
%type <yd_cast> cast

%%

start: expr END		{ y("start: expr END"); bn_print($1); YYACCEPT; }
     | END		{ YYACCEPT; }
     | 			{ YYACCEPT; }
     ;

expr: expr PLUS expr	{ y("expr: expr PLUS expr"); $$ = bn_add($1, $3); }
    | expr1		{ y("expr: expr1"); $$ = $1; }

num: RAWNUM		{ y("num: RAWNUM");
			  $$ = bn_new_bnum_tok($1, sizeof(int) * 8, 1); }

cast: LBRA CASTU8 RBRA	{ y("cast: CASTU8"); $$.width = 8; $$.signd = 0;}
    | LBRA CAST8 RBRA	{ y("cast: CAST8"); $$.width = 8; $$.signd = 1; }
    | LBRA CASTU16 RBRA	{ y("cast: CASTU16"); $$.width = 16; $$.signd = 0; }
    | LBRA CAST16 RBRA	{ y("cast: CAST16"); $$.width = 16; $$.signd = 1; }
    | LBRA CASTU32 RBRA	{ y("cast: CASTU32"); $$.width = 32; $$.signd = 0; }
    | LBRA CAST32 RBRA	{ y("cast: CAST32"); $$.width = 32; $$.signd = 1; }

expr1: num		{ $$ = $1; }
     | LBRA expr RBRA	{ $$ = $2; }
     | cast expr1	{ y("expr: cast expr"); $$ = bn_cast($2, $1); }

%%

void
y(const char *msg)
{
	if (bn_yyll_debug)
		fprintf(stderr, "yacc: %s\n", msg);
}

void yyerror(char *msg)
{
	fprintf(stderr, "Parse error %s\n", msg);
}
