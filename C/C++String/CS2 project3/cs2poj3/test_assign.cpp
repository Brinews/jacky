//  Stack class test program
//
//  Tests: assign operator
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
        Stack<int> s2 = s;
        // VERIFY
        assert(s.isEmpty() == true);
        assert(s.getSize() == 0);
        assert(s2.isEmpty() == true);
        assert(s2.getSize() == 0);
    }
    {
        //------------------------------------------------------
        // SETUP FIXTURE
        Stack<int> s;
        // TEST
        s.push(1);
        s.push(2);
        s.push(3);
        Stack<int> s2 = s;
        // VERIFY
        assert(s.isEmpty() == false);
        assert(s.getSize() == 3);
        assert(s2.isEmpty() == false);
        assert(s2.getSize() == 3);
    }

    {
        //------------------------------------------------------
        // SETUP FIXTURE
        Stack<String> s;
        // TEST
        Stack<String> s2 = s;
        // VERIFY
        assert(s.isEmpty() == true);
        assert(s.getSize() == 0);
        assert(s2.isEmpty() == true);
        assert(s2.getSize() == 0);
    }
    {
        //------------------------------------------------------
        // SETUP FIXTURE
        Stack<String> s;
        // TEST
        s.push("a");
        s.push("b");
        s.push("c");
        Stack<String> s2 = s;
        // VERIFY
        assert(s.isEmpty() == false);
        assert(s.getSize() == 3);
        assert(s2.isEmpty() == false);
        assert(s2.getSize() == 3);
    }
    std::cout << "Done testing assign operator." << std::endl;
    return 0;
}
