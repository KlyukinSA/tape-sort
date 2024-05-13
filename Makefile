CXX = g++
CXXFLAGS = -Wall

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

prog: main.o ExternalSorter.o FileTape.o
	$(CXX) $^ -o $@

all: prog
