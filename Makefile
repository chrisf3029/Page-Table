CC := g++
CXX := g++ 
CPPFLAGS := -g -O2 -std=c++11 -Wall -pedantic

C_SRC   := memory.c process.c swapfile.c sim.c
CPP_SRC := pagetable.cpp pagefault_handler.cpp
HDR	:= global.h page-splitting.h process.h swapfile.h types.h memory.h pagetable.h statistics.h useful.h

OBJ	:= $(patsubst %.c, %.o, $(C_SRC)) $(patsubst %.cpp, %.o, $(CPP_SRC))

all: vm-sim

vm-sim: $(OBJ)
	$(CXX) -o $@ $(OBJ)

%.o: %.cpp $(HDR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f vm-sim *.o
