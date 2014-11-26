//  String class test program
//
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
        String  str("word in sentence");

        // TEST
        String result = str.substr(0, 4);

        // VERIFY
        assert(str    == "word in sentence");
        assert(result == "word");
    }

    {
        //------------------------------------------------------
        // SETUP FIXTURE
        String  str("word");

        // TEST
        int result = str.findstr("ord", 0);

        // VERIFY
        assert(str    == "word");
        assert(result == 1);
    }

    {
        //------------------------------------------------------
        // SETUP FIXTURE
        String  str("word");

        // TEST
        int result = str.findchar('o', 0);

        // VERIFY
        assert(str    == "word");
        assert(result == 1);
    }

    {
        //------------------------------------------------------
        // SETUP FIXTURE
        String  str("wor d");

        // TEST
        int result = str.findNextBlank(0);

        // VERIFY
        assert(str    == "wor d");
        assert(result == 3);
    }

    {
        //------------------------------------------------------
        // SETUP FIXTURE
        String  str("word in word");

        // TEST
        int result = str.findNextNonBlank(4);

        // VERIFY
        assert(str    == "word in word");
        assert(result == 5);
    }

    {
        //------------------------------------------------------
        // SETUP FIXTURE
        String  str("word");

        // TEST
        String result = str.insertAt("-", 2);

        // VERIFY
        assert(str    == "wo-rd");
        assert(result == "wo-rd");
    }

    // ADD ADDITIONAL TESTS AS NECESSARY
    
    std::cout << "Done testing return_function." << std::endl;
}

