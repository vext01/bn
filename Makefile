all: bn

bn: linenoise bn.c bn.h
	${CC} ${CFLAGS} ${LDFLAGS} -o bn \
		linenoise/linenoise.o linenoise/utf8.o bn.c

.PHONY: linenoise
linenoise:
	cd linenoise && ${MAKE} CFLAGS="${CFLAGS}"
