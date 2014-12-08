//  Stack class test program
//
//  Tests: empty
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
        assert(s.getSize() == 0);
        s.push(1);
        assert(s.getSize() == 1);
        s.push(2);
        assert(s.getSize() == 2);
        s.pop();
        assert(s.getSize() == 1);
        s.pop();
        assert(s.getSize() == 0);
    }
    {
        //------------------------------------------------------
        // SETUP FIXTURE
        Stack<String> s;
        // TEST
        // VERIFY
        assert(s.getSize() == 0);
        s.push("a");
        assert(s.getSize() == 1);
        s.push("b");
        assert(s.getSize() == 2);
        s.pop();
        assert(s.getSize() == 1);
        s.pop();
        assert(s.getSize() == 0);
    }
    std::cout << "Done testing size." << std::endl;
    return 0;
}
