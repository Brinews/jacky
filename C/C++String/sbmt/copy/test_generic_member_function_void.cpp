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
		String str1("ilice");

        // TEST
        str.swap(str1);

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

    // ADD ADDITIONAL TESTS AS NECESSARY
    std::cout << "Done testing return_void." << std::endl;

}

