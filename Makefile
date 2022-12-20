_DEPS = freq_analysis TrieNode WordGraph BoardIO

CC = g++

CFLAGS =
LIBS =

ODIR = obj
IDIR = includes

DEPS = $(patsubst %,$(IDIR)/%.hpp,$(_DEPS))

SRC = wordhunter.cpp

_OBJ = wordhunter
OBJ = $(patsubst %,$(ODIR)/%.o,$(_DEPS))

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

analysis : $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o client