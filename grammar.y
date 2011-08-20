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
	struct bnum_tok		*yd_num;
	uint64_t		 yd_rawnum;
	struct bn_cast		 yd_cast;
}

%left PLUS
%left MINUS

%type <yd_rawnum> RAWNUM
%type <yd_num> num
%type <yd_num> expr
%type <yd_cast> cast

%%

start: expr END		{ y("start: expr END"); bn_print($1); YYACCEPT; }
     ;

expr: expr PLUS expr	{ y("expr: num PLUS num"); $$ = bn_add($1, $3); }
    | LBRA expr RBRA	{ y("expr: LBRA expr RBRA"); $$ = $2; }
    | cast expr		{ y("expr: cast expr");}
    | num		{ y("expr: num"); $$ = $1; }

num: RAWNUM		{ y("num: RAWNUM");
			  $$ = bn_new_bnum_tok($1, sizeof(int), 1); }

cast: CASTU8		{ y("cast: CASTU8"); $$.width = 1; $$.signd = 0; }
      CAST8		{ y("cast: CAST8"); $$.width = 1; $$.signd = 1; }
      CASTU16		{ y("cast: CASTU16"); $$.width = 2; $$.signd = 0; }
      CAST16		{ y("cast: CAST16"); $$.width = 2; $$.signd = 1; }
      CASTU32		{ y("cast: CASTU32"); $$.width = 4; $$.signd = 0; }
      CAST32		{ y("cast: CAST32"); $$.width = 4; $$.signd = 1; }

%%

void
y(const char *msg)
{
	//if (l_debug)
	fprintf(stderr, "yacc: %s\n", msg);
}

void yyerror(char *m)
{
	y(m);
}
