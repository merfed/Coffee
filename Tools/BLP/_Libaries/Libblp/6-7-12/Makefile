# Makefile

# variable informant du mode debug ou non
modedebug=1

cdeCompilation=/usr/bin/gcc

compilFlags+=-x c -pipe -Wno-trigraphs

# commande de compilation
ifeq ($(modedebug), 1)
compilFlags+=-g -O0
else
compilFlags+=-DNDEBUG -fasm-blocks -O2
endif

# répertoire courant du projet
home=.

# répertoire des sources
sourcesDir=./

# include files
includeDirs=-I./ -I../jpeg-6b

# répertoire des fichiers compiles
buildDir=./


# liste des fichiers obj pour les graphes
listeObj=$(buildDir)ijg.o $(buildDir)blp.o

libName=libblp.a

VPATH=$(sourcesDir)

all: lib

lib: $(listeObj)
	ar rc $(libName) $^
	ranlib $(libName)

clean: 
	rm -f $(buildDir)*.o
	rm -f $(libName)

$(buildDir)ijg.o : ijg.c
	$(cdeCompilation) $(compilFlags) $(includeDirs) -c $^ -o $@

$(buildDir)blp.o : blp.c
	$(cdeCompilation) $(compilFlags) $(includeDirs) -c $^ -o $@
