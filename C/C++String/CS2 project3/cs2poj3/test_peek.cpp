//  Stack class test program
//
//  Tests: peek
//

#include "string.hpp"
#include "stack.hpp"
#include "stack.cpp"
#include <cassert>
#include <iostream>
#include <stdexcept>

using namespace std;

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
        try {
            s.peek();
            assert(false);
        } catch (std::runtime_error) {
            assert(true);
        }
    }
    {
        //------------------------------------------------------
        // SETUP FIXTURE
        Stack<String> s;
        // TEST
        // VERIFY
        assert(s.isEmpty() == true);
        assert(s.getSize() == 0);
        try {
            s.peek();
            assert(false);
        } catch (std::runtime_error) {
            assert(true);
        }
    }

    {
        //------------------------------------------------------
        // SETUP FIXTURE
        Stack<int> s;
        // TEST
        // VERIFY
        s.push(1);
        assert(s.peek() == 1);
        assert(s.getSize() == 1);
        s.push(2);
        assert(s.peek() == 2);
        assert(s.getSize() == 2);
        s.push(3);
        assert(s.peek() == 3);
        assert(s.getSize() == 3);
    }

    {
        //------------------------------------------------------
        // SETUP FIXTURE
        Stack<String> s;
        // TEST
        // VERIFY
        s.push("a");
        assert(s.peek() == "a");
        assert(s.getSize() == 1);
        s.push("b");
        assert(s.peek() == "b");
        assert(s.getSize() == 2);
        s.push("c");
        assert(s.peek() == "c");
        assert(s.getSize() == 3);
    }
    std::cout << "Done testing peek." << std::endl;
    return 0;
}
