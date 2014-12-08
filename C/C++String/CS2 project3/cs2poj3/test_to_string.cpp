//  Stack class test program
//
//  Tests: toString
//

#include "string.hpp"
#include "stack.hpp"
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
        assert(s.toString() == "");
        s.push(1);
        assert(s.toString() == "1");
        s.push(2);
        assert(s.toString() == "2 1");
        s.push(3);
        assert(s.toString() == "3 2 1");
        s.pop();
        assert(s.toString() == "2 1");
        s.pop();
        assert(s.toString() == "1");
        s.pop();
        assert(s.toString() == "");
    }
    {
        //------------------------------------------------------
        // SETUP FIXTURE
        Stack<String> s;
        // TEST
        // VERIFY
        assert(s.toString() == "");
        s.push("a");
        assert(s.toString() == "a");
        s.push("b");
        assert(s.toString() == "b a");
        s.push("c");
        assert(s.toString() == "c b a");
        s.pop();
        assert(s.toString() == "b a");
        s.pop();
        assert(s.toString() == "a");
        s.pop();
        assert(s.toString() == "");
    }
    std::cout << "Done testing toString." << std::endl;
    return 0;
}
