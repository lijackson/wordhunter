_DEPS = wordhunter TrieNode WordGraph

CC = g++

CFLAGS =
LIBS =

ODIR = obj
IDIR = includes

DEPS = $(patsubst %,$(IDIR)/%.hpp,$(_DEPS))

SRC = Client.cpp

_OBJ = Client
OBJ = $(patsubst %,$(ODIR)/%.o,$(_DEPS))

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

solver : $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o client