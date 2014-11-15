HEADER = $(wildcard *.hpp)
SRC = String.cpp
OBJ = $(patsubst %.cpp,%.o,$(SRC))
FLAGS = -g -ansi -Wall -pedantic

EXE = test_partABC test_justify

all : $(EXE)

$(OBJ) : $(HEADER)
test_%.o : test_%.cpp
test_% : $(OBJ) test_%.o
	g++ -o $@ $^ $(FLAGS)

clean:
	-rm *.o $(EXE) *.exe

tests:
	test_partABC

justify:
	test_justify 10 50 data2-1.txt
	test_justify  2 50 data2-1.txt
