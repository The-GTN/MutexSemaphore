CC      = gcc

CFLAGS  = -Wall -Wextra -pedantic -m32 -fno-pic
CFLAGS += -D_XOPEN_SOURCE=500
CFLAGS += -g

LDFLAGS = -g -m32 -fno-pic

ROOTDIR=/home/enseign/ASE
INCS    = -I$(ROOTDIR)/include
LIBS    = -L$(ROOTDIR)/lib -lhardware


EXE = prodCons philo
HEADDIR = header
SRC = src
LIB = lib

all: ${EXE}

switch.o: ${LIB}/switch.c ${HEADDIR}/*
	${CC} ${INCS} ${CFLAGS} -c $<

# Règles implicites pour la compilation et l’édition de liens
%.o: ${SRC}/%.c
	${CC} ${INCS} ${CFLAGS} -c $<

%: %.o switch.o
	${CC} ${LDFLAGS} -o $@ $^ ${LIBS}

# Règles de nettoyage
clean:
	rm -f core *.o *.exe *.exe.* ${EXE}

.PHONY: all clean realclean
