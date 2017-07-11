CXX = g++
CXXFLAGS = -std=c++14 -Wall -Werror -MMD -g
EXEC = a4q2a
OBJECTS = main.o cell.o grid.o subject.o textdisplay.o 
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}
.PHONY: clean
