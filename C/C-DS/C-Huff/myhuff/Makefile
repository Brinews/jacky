BIN_NAMES = HuffCompress HuffUncompress HuffTreeTest

all: $(BIN_NAMES)

CXX = g++

%.o: %.cpp
	g++ -c $^

HuffTreeTest: HuffTree.o HuffNode.o HuffTreeTest.o BitInput.o BitOutput.o
	$(CXX) -o $@ $^

HuffCompress: HuffCompress.o HuffTree.o HuffNode.o BitInput.o BitOutput.o
	$(CXX) -o $@ $^

HuffUncompress: HuffUncompress.o HuffTree.o HuffNode.o BitInput.o BitOutput.o
	$(CXX) -o $@ $^
