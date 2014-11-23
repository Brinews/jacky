//  String class test program
//
//  Name:  XXX
//  Tests: XXX
//

#include <cassert>
#include <iostream>
#include "string.hpp"

//===========================================================================
int main ()
{
    {
        //------------------------------------------------------
        // SETUP FIXTURE
        String  left("left");
        String  right("right");

        // TEST
        String result = left + right;

        // VERIFY
        assert(result == "leftright");
        assert(left   == "left");
        assert(right  == "right");
    }

    {
        //------------------------------------------------------
        // SETUP FIXTURE
        String  left("ok");
        String  right("ok");

        // TEST
        bool result = (left == right);

        // VERIFY
        assert(result == true);
        assert(left   == "ok");
        assert(right  == "ok");
    }

    {
        //------------------------------------------------------
        // SETUP FIXTURE
        String  left("A");
        String  right("B");

        // TEST
        bool result = (left < right);

        // VERIFY
        assert(result == true);
        assert(left   == "A");
        assert(right  == "B");
    }

    {
        //------------------------------------------------------
        // SETUP FIXTURE
        String  left("1");
        String  right("2");

        // TEST
        bool result = (left > right);

        // VERIFY
        assert(result == false);
        assert(left   == "1");
        assert(right  == "2");
    }

    {
        //------------------------------------------------------
        // SETUP FIXTURE
        String  left('A');
        String  right("A");

        // TEST
        bool result = (left == right);

        // VERIFY
        assert(result == true);
        assert(left   == 'A');
        assert(right  == "A");
    }

    {
        //------------------------------------------------------
        // SETUP FIXTURE
        String  left("WORD");
        String  right('S');

        // TEST
        String result = (left + right);

        // VERIFY
        assert(result == "WORDS");
        assert(left   == "WORD");
        assert(right  == 'S');
    }
    

    // ADD ADDITIONAL TESTS AS NECESSARY
    
    std::cout << "Done testing binary_op." << std::endl;
}

