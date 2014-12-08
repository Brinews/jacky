//  Stack class test program
//
//  Tests: default constructor
//

#include "string.hpp"
#include "stack.hpp"
#include "stack.cpp"
#include <cassert>
#include <iostream>

//===========================================================================
int main() {
    {
        //------------------------------------------------------
        // SETUP FIXTURE
        Stack<int> s;
        // TEST
        // VERIFY
        assert(s.isEmpty() == true);
        assert(s.getSize() == 0);
    }
    {
        //------------------------------------------------------
        // SETUP FIXTURE
        Stack<String> s;
        // TEST
        // VERIFY
        assert(s.isEmpty() == true);
        assert(s.getSize() == 0);
    }
    std::cout << "Done testing default constructor." << std::endl;
    return 0;
}
