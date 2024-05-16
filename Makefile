CXX = g++
CXXFLAGS = -Wall

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

prog: main.o FileTape.o FileTapeConfig.o MergingTape.o FileTapeFactory.o
	$(CXX) $^ -o $@

all: prog

clean:
	rm *.o prog
