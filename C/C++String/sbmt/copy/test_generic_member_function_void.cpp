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
        String  str("bob");

        // TEST
        str.swap("ilice");

        // VERIFY
        assert(str == "ilice");
    }

    {
        //------------------------------------------------------
        // SETUP FIXTURE
        String  str("small");

        // TEST
        str.resetCapacity(100);

        // VERIFY
        assert(str == "small");
    }

    {
        //------------------------------------------------------
        // SETUP FIXTURE
        String  str(X);

        // TEST
        str.OP();

        // VERIFY
        assert(XXX == YYY);
    }

    // ADD ADDITIONAL TESTS AS NECESSARY
    std::cout << "Done testing return_void." << std::endl;

}

