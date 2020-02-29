IDIR =./includes /usr/local/include
CC=g++
CFLAGS=-I$(IDIR)

OUTPUTFILE  = FLTKConsole
INSTALLDIR  = ../Linux

ODIR=obj
LDIR =	~/Projects/SDRLibE/wdsp_uni/wdsp_uni/Linux\
	~/Projects/SDRLibE/server/Linux\
	/usr/lib/x86_64-linux-gnu\
	/usr/local/lib 

LIBS=-lpthreads -lfftw3 -lportaudio -lwdsp_uni -lSDRLibE -lfontconfig -lfltk -lXext -lXinerama -lXft -lX11 -lXi -lm

$(OUTPUTFILE) : Main/Main.o RadioInterface/radio_interface.o Ui/Components/vfo.o 
	$(CC) $(IDIR) $(LDIR) -o FLTKConsole Main/Main.o RadioInterface/radio_interface.o Ui/Components/vfo.o\
	$(LIBS)
                   
Main.o : Main/Main.c extern.h radio_interface.h vfo.h
	cc -c Main/Main.c
        
radio_interface.o : RadioInterface/radio_interface.c extern.h radio_interface.h
	cc -c RadioInterface/radio_interface.c
        
vfo.o : Ui/Components/vfo.c radio_interface.h vfo.h
	cc -c Ui/Components/vfo.c

.PHONY: install
install:
	mkdir -p $(INSTALLDIR)
	cp -p $(OUTPUTFILE) $(INSTALLDIR)

.PHONY: clean
clean :
	rm Main/Main.o RadioInterface/radio_interface.o Ui/Components/vfo.o
	
           