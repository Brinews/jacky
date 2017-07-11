CXX = g++-5
CXXFLAGS = -std=c++14 -Wall -Werror -MMD
EXEC = a4q2b
OBJECTS = main.o cell.o grid.o subject.o textdisplay.o window.o graphicdisplay.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC} -lX11

-include ${DEPENDS}

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}
.PHONY: clean
