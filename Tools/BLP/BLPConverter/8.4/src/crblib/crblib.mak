#----- makefile for crblib -----
#
# Simple UNIX makefile, tweak as necessary.
#
# Build this before building in the "ppmz" directory.
#
SHELL		= /bin/sh
CC			= gcc
CFLAGS		= -O3 -Wall

INCLUDES	= -I.

OBJS		= arithc.o bbitio.o cacmari.o chshutil.o cindcatr.o crbconv.o \
			  crbeqlib.o crc32.o equtil.o context.o fileutil.o mtf.o o1coder.o \
			  huffman2.o lbitio.o matchpat.o mempool.o memutil.o huffa.o \
			  o0coder.o sortnods.o strutil.o timer.o runtrans.o dbf.o

all: prep libcrb.a

# create a "crbinc" link so the <crbinc/foo.h> includes will work
prep:
	@if [ -h crbinc ]; then rm -f crbinc; fi
	@ln -s . crbinc

.c.o:
	$(CC) -c $(CFLAGS) $(INCLUDES) $<

libcrb.a: $(OBJS)
	ar ruv $@ $(OBJS)

clean:
	rm -f libcrb.a *.o core *~

