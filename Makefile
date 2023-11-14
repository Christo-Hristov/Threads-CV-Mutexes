TARGETS = test

PATH_TO_FILE = function.cc
ifneq ("$(wildcard $(PATH_TO_FILE))","")
    TARGETS += function interrupt interrupt2 interrupt3 static  #two_stacks (needs updates for new Stack)
endif
#ARCH = -m32
CXXBASE = clang++-10
#CXXBASE = g++
CXX = $(CXXBASE) $(ARCH) -std=c++20
CC = $(CXX)
CXXFLAGS = -ggdb -O -Wall -Werror

OBJS = stack.o sync.o test.o thread.o timer.o
HEADERS = stack.hh thread.hh timer.hh sync.hh

all:: $(TARGETS)

$(OBJS): $(HEADERS)

test: $(OBJS) $(LIB)
	$(CXX) -o $@ $(OBJS)

function: function.cc
	$(CXX) $(CXXFLAGS) function.cc -o function

interrupt: interrupt.o timer.o
	$(CXX) -o $@ interrupt.o timer.o

interrupt2: interrupt2.o timer.o
	$(CXX) -o $@ interrupt2.o timer.o

interrupt3: interrupt3.o timer.o
	$(CXX) -o $@ interrupt3.o timer.o

static: static.cc
	$(CXX) $(CXXFLAGS) -o $@ static.cc

two_stacks: two_stacks.o stack.o
	$(CXX) -o $@ two_stacks.o stack.o

clean::
	rm -f $(TARGETS) $(EXTRAS) *.o *.s *~ .*~

.SUFFIXES: .cc

.PHONY: all clean


# The following target is useful for debugging Makefiles; it
# prints the value of a make variable.
print-%:
	@echo $* = $($*)
