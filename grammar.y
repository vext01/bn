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
#include "logic2cnf.h"

%}

%start start

%token AND OR NOT XOR
%token LIMPL RIMPL BIMPL
%token LBRA RBRA
%token DEF LIT NUM
%token REPORT_LITERALS REPORT_DEC REP_CMMT
%token UQ_ELIM RESOLVE
%token FIND_MAX FIND_MIN
%token END

%union {
	lnode			*node;
	char			*chars;
	unsigned long int	num;
}

%type <node> expr
%type <chars> LIT 
%type <node> start
%type <chars> REP_CMMT
%type <num> NUM


%left LIMPL RIMPL BIMPL
%left OR
%left XOR
%left AND
%right NOT
%right DEF

%%

start: expr END		{}
     | def start	{}
     | uq_elim start	{}
     | resolve start	{}
     | report start	{}
     | findmaxmin start	{}
     ;

def:   DEF lit_list END		{ yydbg("DEF lit_list"); };

uq_elim:
       UQ_ELIM uq_lit_list END	{ yydbg("UQ_ELIM uq_lit_list"); };

uq_lit_list: uq_lit_list LIT	{
					yydbg("uq_lit_list LIT");
					add_uq_elim($2);
					free($2);
				}
       | 			{}  /* NOTHING */
       ;

findmaxmin: FIND_MAX find_max_lit_list END
	  		{
				yydbg("FIND_MAX find_lit_list");
				new_maxmin_statement();
			}
	  | FIND_MIN find_min_lit_list END
			{
				yydbg("FIND_MIN find_lit_list");
				new_maxmin_statement();
			}
	  ;


find_max_lit_list: find_max_lit_list LIT	{
	     					yydbg("find_max_lit_list LIT");
						add_maxmin_lit($2, MAXMIN_MAX);
					}
	     |				{} /* NOTHING */
	     ;
	
find_min_lit_list: find_min_lit_list LIT	{
	     					yydbg("find_min_lit_list LIT");
						add_maxmin_lit($2, MAXMIN_MIN);
					}
	     |				{} /* NOTHING */
	     ;
	
resolve:
       RESOLVE resolve_lit_list END	{ yydbg("RESOLVE resolve_lit_list"); };

resolve_lit_list: resolve_lit_list LIT
				{
					yydbg("resolve_lit_list LIT");
					add_resolve_lit($2);
					free($2);
				}
       | 			{}  /* NOTHING */
       ;

report:	REPORT_LITERALS REP_CMMT report_lit_list END
				{
					yydbg("REPORT_LITERALS REP_CMMT lit_list");
					add_sat_report(REPORT_LITERALS, $2);
					free($2);
				}
      | REPORT_LITERALS report_lit_list END
				{
					yydbg("REPORT_LITERALS lit_list");
					add_sat_report(REPORT_LITERALS, NULL);
				}
      | REPORT_DEC REP_CMMT report_lit_list END
				{
					yydbg("REPORT_DEC REP_CMMT lit_list");
					add_sat_report(REPORT_DEC, $2);
					free($2);
				}
      | REPORT_DEC report_lit_list END
				{
					yydbg("REPORT_DEC lit_list");
					add_sat_report(REPORT_DEC, NULL);
				}
      ;

report_lit_list: report_lit_list LIT	{
						yydbg("report_lit_list LIT");
						add_lit_to_sat_report($2);
						free($2);
					}
               | 			{}  /* NOTHING */
               ;

lit_list: lit_list LIT		{
					yydbg("DEF LIT");
					define_lit($2, 0);
					free($2);
				}
	| 			{ yydbg("DEF LIT EMPTY"); } /* NOTHING */
	;


expr: NOT expr		{
    				yydbg("NOT expr");
				$$ = new_not_node($2);
			}
    | expr AND expr	{
				yydbg("expr AND expr");
				$$ = new_and_node($1, $3);
			}
    | expr OR expr	{
				yydbg("expr OR expr");
				$$ = new_or_node($1, $3);
			}
    | expr LIMPL expr	{
				yydbg("expr LIMPL expr");
				/* give them the old switcheroo
				 * so as to make a RIMPL */
				$$ = new_imply_node($3, $1);
			}
    | expr RIMPL expr	{
				yydbg("expr RIMPL expr");
				$$ = new_imply_node($1, $3);
			}
    | expr BIMPL expr	{
				yydbg("expr BIMPL expr");
				$$ = new_bi_imply_node($1, $3);
			}
    | expr XOR expr	{
				yydbg("expr XOR expr");
				$$ = new_xor_node($1, $3);
			}
    | LBRA expr RBRA	{
				yydbg("LBRA expr RBRA");
				$$ = $2;
			}
    | LIT		{
				yydbg("LIT:");
				yydbg($1);
				$$ = new_lit_node($1);
				free($1);
			}
    ;

%%

void yyerror(const char *e)
{
	lerr(1, "%s line %d:\n\t%s", yyin_filename, my_yylineno, e);
}

void
yydbg(const char *msg)
{
	if (l_debug)
		fprintf(stderr, "YYDBG: %s\n", msg);
}
