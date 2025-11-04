CC = gcc
CFLAGS += -g

wordgen : wordgen.c
	${CC} ${CFLAGS} -o $@ $^

.PHONY: clean
clean:
	@rm -rvf *.o