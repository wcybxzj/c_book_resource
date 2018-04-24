include ../Make.defines

PROGS =	tcpcli01 tcpcli02 tcpcli03 tcpcli_04_my

all:	${PROGS}

tcpcli01:	tcpcli01.o strcliselect01.o
		${CC} ${CFLAGS} -o $@ tcpcli01.o strcliselect01.o ${LIBS}

tcpcli02:	tcpcli02.o strcliselect02.o
		${CC} ${CFLAGS} -o $@ tcpcli02.o strcliselect02.o ${LIBS}

tcpcli03:	tcpcli03.o
		${CC} ${CFLAGS} -o $@ tcpcli03.o ${LIBS}

tcpcli_04_my: tcpcli_04_my.o
		${CC} ${CFLAGS} -o $@ tcpcli_04_my.o strcliselect01.o ${LIBS}

clean:
		rm -f ${PROGS} ${CLEANFILES}
