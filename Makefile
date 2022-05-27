# makefile for modimizer, developed on Richard's Mac.

CFLAGS= -O3
#CFLAGS= -g			# for debugging

ALL=composition rotate

DESTDIR=~/bin

all: $(ALL)

install:
	cp $(ALL) $(DESTDIR)

clean:
	$(RM) *.o *~ $(ALL)
	\rm -r *.dSYM

### object files

UTILS_OBJS=array.o utils.o
UTILS_HEADERS=utils.h array.h
$(UTILS_OBJS): utils.h $(UTILS_HEADERS)

seqio.o: seqio.c seqio.h 
	$(CC) $(CFLAGS) -c $^

### programs

composition: composition.c seqio.o $(UTILS_OBJS)
	$(CC) $(CFLAGS) $^ -o $@ -lz

rotate: rotate.c seqio.o $(UTILS_OBJS)
	$(CC) $(CFLAGS) $^ -o $@ -lz

### end of file
