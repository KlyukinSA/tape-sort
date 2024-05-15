CXX = g++
CXXFLAGS = -Wall

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

prog: main.o ExternalSorter.o FileTape.o FileTapeConfig.o
	$(CXX) $^ -o $@

all: prog

clean:
	rm *.o prog
