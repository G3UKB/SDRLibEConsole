IDIR = /usr/local/include
CC=g++
CFLAGS = -I$(IDIR)
OUTPUTFILE  = FLTKConsole
INSTALLDIR  = ../Linux

ODIR=obj
LDIR =	~/Projects/SDRLibE/wdsp_uni/wdsp_uni/Linux\
	~/Projects/SDRLibE/server/Linux\
	/usr/lib/x86_64-linux-gnu\
	/usr/local/lib 

LIBS=-lpthreads -lfftw3 -lportaudio -lwdsp_uni -lSDRLibE -lfontconfig -lfltk -lXext -lXinerama -lXft -lX11 -lXi -lm

$(OUTPUTFILE) : Main.o radio_interface.o vfo.o 
	$(CC) $(CFLAGS) $(LDIR) -o FLTKConsole Main.o radio_interface.o vfo.o\
	$(LIBS)
                   
Main.o : Main/Main.cpp Includes/extern.h Includes/radio_interface.h Includes/vfo.h
	$(CC) $(CFLAGS) -c Main/Main.cpp
        
radio_interface.o : RadioInterface/radio_interface.cpp Includes/extern.h Includes/radio_interface.h
	$(CC) $(CFLAGS) -c RadioInterface/radio_interface.cpp
        
vfo.o : Ui/Components/vfo.cpp Includes/radio_interface.h Includes/vfo.h
	$(CC) $(CFLAGS) -c Ui/Components/vfo.cpp

.PHONY: install
install:
	mkdir -p $(INSTALLDIR)
	cp -p $(OUTPUTFILE) $(INSTALLDIR)

.PHONY: clean
clean :
	rm Main/Main.o RadioInterface/radio_interface.o Ui/Components/vfo.o
	
           
           