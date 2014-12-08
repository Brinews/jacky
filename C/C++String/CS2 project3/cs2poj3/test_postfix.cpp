//  Stack class test program
//
//  Tests: postfix
//

#include "infix2postfix.hpp"
#include <cassert>

//===========================================================================
int main() {
    {
        //------------------------------------------------------
        // SETUP FIXTURE
        String infix = "( AX + ( B * C ) ) ;";
        String postfix = "AX B C * +";
        // TEST
        // VERIFY
        assert(in2Post(infix) == postfix);
    }
    {
        //------------------------------------------------------
        // SETUP FIXTURE
        String infix = "AX + B * C ;";
        String postfix = "AX B C * +";
        // TEST
        // VERIFY
        assert(in2Post(infix) == postfix);
    }
    {
        //------------------------------------------------------
        // SETUP FIXTURE
        String infix = "( ( AX + ( B * CY ) ) / ( D - E ) ) ;";
        String postfix = "AX B CY * + D E - /";
        // TEST
        // VERIFY
        assert(in2Post(infix) == postfix);
    }
    {
        //------------------------------------------------------
        // SETUP FIXTURE
        String infix = "( ( A + B ) * ( C + E ) ) ;";
        String postfix = "A B + C E + *";
        // TEST
        // VERIFY
        assert(in2Post(infix) == postfix);
    }
    {
        //------------------------------------------------------
        // SETUP FIXTURE
        String infix = "( AX * ( BX * ( ( ( CY + AY ) + BY ) * CX ) ) ) ;";
        String postfix = "AX BX CY AY + BY + CX * * *";
        // TEST
        // VERIFY
        assert(in2Post(infix) == postfix);
    }
    {
        //------------------------------------------------------
        // SETUP FIXTURE
        String infix = "( ( H * ( ( ( ( A + ( ( B + C ) * D ) ) * F ) * G ) * E ) ) + J ) ;";
        String postfix = "H A B C + D * + F * G * E * * J +";
        // TEST
        // VERIFY
        assert(in2Post(infix) == postfix);
    }
    std::cout << "Done testing postfix." << std::endl;
    return 0;
}
