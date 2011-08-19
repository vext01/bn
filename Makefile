CFLAGS=-I/usr/local/include
LDFLAGS=-L/usr/local/lib -lreadline -ltermcap -ltermcap
LEX=flex
YACC=yacc

all: bn lex.yy.c y.tab.h y.tab.c

bn: bn.c bn.h y.tab.h lex.yy.c
	${CC} ${CFLAGS} ${LDFLAGS} -o bn \
		lex.yy.c y.tab.c bn.c

lex.yy.c: grammar.l
	${LEX} $?

y.tab.h y.tab.c: grammar.y
	${YACC} -v -d -o y.tab.c $?

