INCDIR = -I../jpeg-6b -I.. -L..
CCFLAGS = -Wall -O2 -m32
PROGNAME = blpconvert
CFILES = ../ijg.c ../blp.c blpconvert.c

all:
	gcc $(CCFLAGS) $(INCDIR) $(CFILES) ../jpeg-6b/libjpeg.a -o $(PROGNAME)
clean:
	rm -f *.o $(PROGNAME)
