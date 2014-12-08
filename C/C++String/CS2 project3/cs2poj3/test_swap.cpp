//  Stack class test program
//
//  Tests: swap
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
        Stack<int> s1;
        Stack<int> s2;
        // TEST
        // VERIFY
        assert(s1.isEmpty() == true);
        assert(s1.getSize() == 0);
        assert(s2.isEmpty() == true);
        assert(s2.getSize() == 0);
        
        s1.swap(s2);
        assert(s1.isEmpty() == true);
        assert(s1.getSize() == 0);
        assert(s2.isEmpty() == true);
        assert(s2.getSize() == 0);
    }

    {
        //------------------------------------------------------
        // SETUP FIXTURE
        Stack<String> s1;
        Stack<String> s2;
        // TEST
        // VERIFY
        assert(s1.isEmpty() == true);
        assert(s1.getSize() == 0);
        assert(s2.isEmpty() == true);
        assert(s2.getSize() == 0);

        s1.swap(s2);
        assert(s1.isEmpty() == true);
        assert(s1.getSize() == 0);
        assert(s2.isEmpty() == true);
        assert(s2.getSize() == 0);
    }
    {
        //------------------------------------------------------
        // SETUP FIXTURE
        Stack<int> s1;
        Stack<int> s2;
        // TEST
        s1.push(1);
        s1.push(2);
        s2.push(3);
        s2.push(4);
        s2.push(5);
        // VERIFY
        assert(s1.isEmpty() == false);
        assert(s1.getSize() == 2);
        assert(s2.isEmpty() == false);
        assert(s2.getSize() == 3);

        s1.swap(s2);

        assert(s1.isEmpty() == false);
        assert(s1.getSize() == 3);
        assert(s2.isEmpty() == false);
        assert(s2.getSize() == 2);
    }

    {
        //------------------------------------------------------
        // SETUP FIXTURE
        Stack<String> s1;
        Stack<String> s2;
        // TEST
        s1.push("a");
        s1.push("b");
        s2.push("c");
        s2.push("d");
        s2.push("e");
        // VERIFY
        assert(s1.isEmpty() == false);
        assert(s1.getSize() == 2);
        assert(s2.isEmpty() == false);
        assert(s2.getSize() == 3);

        s1.swap(s2);

        assert(s1.isEmpty() == false);
        assert(s1.getSize() == 3);
        assert(s2.isEmpty() == false);
        assert(s2.getSize() == 2);
    }
    std::cout << "Done testing swap." << std::endl;
    return 0;
}
