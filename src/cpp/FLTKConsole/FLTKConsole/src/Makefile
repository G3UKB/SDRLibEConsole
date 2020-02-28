IDIR =./includes /usr/local/include
CC=g++
CFLAGS=-I$(IDIR)

ODIR=obj
LDIR =	~/Projects/SDRLibE/wdsp_uni/wdsp_uni/Linux\
	~/Projects/SDRLibE/server/Linux\
	/usr/lib/x86_64-linux-gnu\
	/usr/local/lib 

LIBS=-lpthreads -lfftw3 -lportaudio -lwdsp_uni -lSDRLibE -lfontconfig -lfltk -lXext -lXinerama -lXft -lX11 -lXi -lm

#_DEPS = hellomake.h
#DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = Main/Main.cpp RadioInterface/radio_interface.cpp Ui/Components/vfo.cpp
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

consolemake: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 