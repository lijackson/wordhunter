CC = g++

ODIR = obj
IDIR = includes

FLAGS = -O3

$(ODIR)/%.o: %.cpp
	$(CC) $(FLAGS) -c -o $@ $<

_WORDHUNTER_OBJS = wordhunter WordGraph TrieNode BoardIO
WORDHUNTER_OBJS = $(patsubst %, $(ODIR)/%.o, $(_WORDHUNTER_OBJS))
wordhunter: $(WORDHUNTER_OBJS)
	$(CC) $(FLAGS) -o wordhunter $(WORDHUNTER_OBJS)

_ANALYSIS_OBJS = freq_analysis WordGraph TrieNode BoardIO
ANALYSIS_OBJS = $(patsubst %, $(ODIR)/%.o, $(_ANALYSIS_OBJS))
analysis: $(ANALYSIS_OBJS)
	$(CC) $(FLAGS) -o analysis $(ANALYSIS_OBJS)

.PHONY:
	clean

clean:
	rm -f $(ODIR)/*.o