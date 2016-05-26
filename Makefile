PROGNAME = brainfuck

CC=gcc
CFLAGS="-I. -Wall"

DEPS = file_reader.h bf.h debug.h

ODIR=obj
_OBJS = file_reader.o bf.o
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(PROGNAME): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ $(PROGNAME)
