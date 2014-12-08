#============================================================================
#  stack class makefile
#===========================================================================

CPP          = clang++
CPP_FLAGS    = -g -Wall -W -Wunused -Wuninitialized -Wshadow 
INCLUDE_OPT  = -iquote . -iquote ./string
TEST_1 = test_ctor_default
TEST_3 = test_assign
TEST_5 = test_peek
TEST_7 = test_swap
TEST_8 = test_empty
TEST_9 = test_size


#-------------------------------------------------------------------------------
STRING_DIR = ./string/

#===========================================================================
msg:
	@echo 'Targets for compiling test programs:'
	@echo '   tests'
	@echo '   clean'
	@echo '   postfix'
	@echo '   assembler'

#===========================================================================
# Compile string.o from Project 2.

string.o:	${STRING_DIR}string.hpp  ${STRING_DIR}string.cpp
	${CPP} ${CPP_FLAGS} ${INCLUDE_OPT} -c ${STRING_DIR}string.cpp

#===========================================================================
# No compile for stack is needed - it is a template.


#===========================================================================
# Compile test programs
#----------------------------------------------------------------------------
test_%: string.o test_%.o
	${CPP} ${CPP_FLAGS} string.o test_$*.o -o test_$*

test_%.o: ${STRING_DIR}string.hpp stack.hpp test_%.cpp
	${CPP} ${CPP_FLAGS} ${INCLUDE_OPT} -c test_$*.cpp


#===========================================================================
# Compile postfix

postfix.o: postfix.cpp ${STRING_DIR}string.hpp stack.hpp
	${CPP} ${CPP_FLAGS} ${INCLUDE_OPT} -c postfix.cpp


postfix: postfix.o string.o
	${CPP} ${CPP_FLAGS} postfix.o string.o -o postfix

#===========================================================================
# Compile assembler

assembler.o: assembler.cpp ${STRING_DIR}string.hpp stack.hpp
	${CPP} ${CPP_FLAGS} ${INCLUDE_OPT} -c assembler.cpp

assembler: assembler.o string.o
	${CPP} ${CPP_FLAGS} string.o assembler.o -o assembler

#===========================================================================
# Run test programs

tests : stack.hpp ${STRING_DIR}string.hpp ${TEST_1} ${TEST_3}  ${TEST_5}  ${TEST_7}  ${TEST_8}  ${TEST_9}  
	./test_ctor_default
	./test_assign
	./test_peek
	./test_swap
	./test_empty
	./test_size
#============================================================================
clean:
	rm -f *.o
	rm -f postfix 
	rm -f assembler
	rm -f test_xxx
	rm -f test_xxx
	rm -f test_xxx

