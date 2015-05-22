###############################################################
# String & Oracle
#
# CS II Kent State University
# Make file for string class and testing oracle 
# J. Maletic 2014
#
#

###############################################################

GCCINC=-I "C:\Program Files\DEV-CPP\Include"
CPPINC=-I "C:\Program Files\DEV-CPP\Include\c++\3.4.2"
CXXINC=-I "C:\Program Files\DEV-CPP\Include\c++\3.4.2\mingw32"

# Variables
CPP     = clang++ $(GCCINC) $(CPPINC)
OPTIONS = -g -Wall -W -Wunused -Wuninitialized -Wshadow 
#CPP     = g++


# Names of your test files - add them in as you build them.
# Names must start with "test_"
#MYTESTS = test_default_ctor test_ctor_charArray 
MYTESTS = test_generic_ctor test_generic_binary_op test_generic_member_function_void test_generic_member_function_return


# Names of test oracle files 
CTOR = testoracle_ctor_default testoracle_ctor_int testoracle_ctor_char testoracle_ctor_charArray_int testoracle_ctor_charArray
REL  = testoracle_equal testoracle_lessThan
COPY = testoracle_ctor_copy testoracle_assign
OPS  = testoracle_concat

###############################################################
# Compile just the string
# Compile and run all provided test oracles on string
# Compile and run all your tests on string
# Compile and run the project
msg:
	@echo 'Targets are:'
	@echo '  string'
	@echo '  oracle'
	@echo '  tests'
	@echo '  justify'
	@echo '  clean'



###############################################################
# Compile string 
#
string.o: string.hpp string.cpp
	$(CPP) -c $(OPTIONS) string.cpp -o string.o

string: string.o
	

###############################################################
# Run all of your tests
# 
# You will need to ADD your other below:
# For example: ./test_plus
#
tests: $(MYTESTS)
	./test_generic_ctor
	./test_generic_binary_op
	./test_generic_member_function_void
	./test_generic_member_function_return
    
    #ADD YOUR TESTS HERE ***************
    
    
    
###############################################################
# Compile all test programs
#
test_%: string.o test_%.o
	$(CPP) string.o test_$*.o -o test_$*

test_%.o: string.hpp test_%.cpp
	$(CPP) $(OPTIONS) -c test_$*.cpp



###############################################################
# Run test oracle
# Comment out one's you don't want.
#
oracle: $(CTOR) $(REL) $(COPY) $(OPS)
	./testoracle_ctor_default
	./testoracle_ctor_int
	./testoracle_ctor_char 
	./testoracle_ctor_charArray
	./testoracle_ctor_charArray_int 
	./testoracle_equal 
	./testoracle_lessThan
	./testoracle_ctor_copy 
	./testoracle_assign
	./testoracle_concat
    

###############################################################
# Compile all test oracles
#
testoracle_%: string.o testoracles/testoracle_%.o
	$(CPP) string.o testoracles/testoracle_$*.o -o testoracle_$*
    



###############################################################
# Compile Project 
#

justify.o: justify.cpp string.hpp
	$(CPP) -c $(OPTIONS) justify.cpp -o justify.o
    
justify: justify.o string.o 
	$(CPP) $(OPTIONS) justify.o string.o  -o justify


###############################################################
# clean 
# Removes all .o files and all executables
#
clean:
	rm -f *.o
	rm -f $(CTOR) $(REL) $(COPY) $(OPS)
	rm -f $(MYTESTS)
	rm -f justify


