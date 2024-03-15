CC      = gcc

CFLAGS  = -Wall -Wextra -pedantic -m32 -fno-pic
CFLAGS += -D_XOPEN_SOURCE=500
CFLAGS += -g

LDFLAGS = -g -m32 -fno-pic

EXE = pingpong pingpongpang pingponginfinite pingpongpanginfinite

all: ${EXE}

switch.o: switch.c switch.h
	${CC} ${CFLAGS} -c $<

# Règles implicites pour la compilation et l’édition de liens
%.o: %.c
	${CC} ${CFLAGS} -c $<

%: %.o switch.o
	${CC} ${LDFLAGS} -o $@ $^
	# rm -f core *.o

%: %.c

# Règles de nettoyage
clean:
	rm -f core *.o *.exe *.exe.* ${EXE}

.PHONY: all clean realclean
