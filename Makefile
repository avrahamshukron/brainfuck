PROGNAME = brainfuck

CC=gcc
CFLAGS="-I. -Wall"

DEPS = file_reader.h bf.h debug.h

ODIR=obj
_OBJS = main.o file_reader.o bf.o
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))

CHECKPATCH = ~/Development/C/checkpatch.pl
CHECKPATCH_FLAGS = --no-tree -f

$(ODIR):
	mkdir $@

$(ODIR)/%.o: %.c $(DEPS) $(ODIR)
	$(CC) -c -o $@ $< $(CFLAGS)

$(PROGNAME): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ $(PROGNAME)

check:
	$(CHECKPATCH) $(CHECKPATCH_FLAGS) ./*.[ch]
