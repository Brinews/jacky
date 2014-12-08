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
        assert(s.isEmpty() == true);
        s.push(1);
        assert(s.isEmpty() == false);
        s.pop();
        assert(s.isEmpty() == true);
    }
    {
        //------------------------------------------------------
        // SETUP FIXTURE
        Stack<String> s;
        // TEST
        // VERIFY
        assert(s.isEmpty() == true);
        s.push("a");
        assert(s.isEmpty() == false);
        s.pop();
        assert(s.isEmpty() == true);
    }
    std::cout << "Done testing empty." << std::endl;
    return 0;
}
