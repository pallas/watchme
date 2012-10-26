default: all

PROGS:= watchme

.PHONY: all
all: $(PROGS)

$(PROGS) : LDFLAGS+= -g -lfam
$(PROGS) : CXXFLAGS+=

.PHONY: clean
clean:
	rm -rf $(PROGS)

#