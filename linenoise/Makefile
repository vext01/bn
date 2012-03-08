.PHONY: all
all: linenoise.o utf8.o

linenoise.o: linenoise.c linenoise.h
	${CC} ${CFLAGS} -c linenoise.c -o linenoise.o

utf8.o: utf8.c utf8.h
	${CC} ${CFLAGS} -c utf8.c -o utf8.o

.PHONY: clean
clean:
	rm -f linenoise.o utf8.o
