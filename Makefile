default: all

PROGS:= watchme

.PHONY: all
all: $(PROGS)

$(PROGS) : LDFLAGS+= -g -lstdc++ -lfam
$(PROGS) : CXXFLAGS+=

$(PROGS) : %: %.o $(PROGS:=.o)

.PHONY: clean
clean:
	rm -rf $(PROGS)

#