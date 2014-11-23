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

        // TEST
        String  str('A');

        // VERIFY
        assert(str == 'A');
    }

    {
        //------------------------------------------------------
        // SETUP FIXTURE

        // TEST
        String  str("STRING");

        // VERIFY
        assert(str == "STRING");
    }

    {
        //------------------------------------------------------
        // SETUP FIXTURE

        // TEST
		String str1 ("STRING");
        String  str(str1);

        // VERIFY
        assert(str == "STRING");
    }

    {
        //------------------------------------------------------
        // SETUP FIXTURE

        // TEST
		String str1("STRING");
        String  str = str1;

        // VERIFY
        assert(str == "STRING");
    }

    {
        //------------------------------------------------------
        // SETUP FIXTURE

        // TEST
        String  str(10, "LONGWORD");

        // VERIFY
        assert(str == "LONGWORD");
    }

    // ADD ADDITIONAL TESTS AS NECESSARY
    
    std::cout << "Done testing ctor." << std::endl;

}

