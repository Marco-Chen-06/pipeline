CC = gcc
CFLAGS += -g -Wall

PROGRAMS = wordgen pager wordsearch launcher 

${PROGRAMS}: % : %.c
	${CC} ${CFLAGS} -o $@ $<

.PHONY: clean all

all: ${PROGRAMS}

clean:
	@rm -rvf ${PROGRAMS} *.o