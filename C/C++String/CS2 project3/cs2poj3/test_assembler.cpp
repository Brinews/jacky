//  Stack class test program
//
//  Tests: postfix
//

#include "infix2assembler.hpp"
#include <cassert>

//===========================================================================
int main() {
    {
        //------------------------------------------------------
        // SETUP FIXTURE
        String infix = "( AX + ( B * C ) ) ;";
        String assembler = "Infix Expression: ( AX + ( B * C ) ) ;\n"
                "Postfix Expression: AX B C * +\n"
                "\n"
                "   LDR  B\n"
                "   MUL  C\n"
                "   STR  TMP1\n"
                "   LDR  AX\n"
                "   ADD  TMP1\n"
                "   STR  TMP2\n";
        // TEST
        // VERIFY
        assert(infix2Assembler(infix) == assembler);
    }
    {
        //------------------------------------------------------
        // SETUP FIXTURE
        String infix = "AX + B * C ;";
        String assembler = "Infix Expression: AX + B * C ;\n"
                "Postfix Expression: AX B C * +\n"
                "\n"
                "   LDR  B\n"
                "   MUL  C\n"
                "   STR  TMP1\n"
                "   LDR  AX\n"
                "   ADD  TMP1\n"
                "   STR  TMP2\n";
        // TEST
        // VERIFY
        assert(infix2Assembler(infix) == assembler);
    }
    {
        //------------------------------------------------------
        // SETUP FIXTURE
        String infix = "( ( AX + ( B * CY ) ) / ( D - E ) ) ;";
        String assembler = "Infix Expression: ( ( AX + ( B * CY ) ) / ( D - E ) ) ;\n"
                "Postfix Expression: AX B CY * + D E - /\n"
                "\n"
                "   LDR  B\n"
                "   MUL  CY\n"
                "   STR  TMP1\n"
                "   LDR  AX\n"
                "   ADD  TMP1\n"
                "   STR  TMP2\n"
                "   LDR  D\n"
                "   SUB  E\n"
                "   STR  TMP3\n"
                "   LDR  TMP2\n"
                "   DIV  TMP3\n"
                "   STR  TMP4\n";
        // TEST
        // VERIFY
        assert(infix2Assembler(infix) == assembler);
    }
    {
        //------------------------------------------------------
        // SETUP FIXTURE
        String infix = "( ( A + B ) * ( C + E ) ) ;";
        String assembler = "Infix Expression: ( ( A + B ) * ( C + E ) ) ;\n"
                "Postfix Expression: A B + C E + *\n"
                "\n"
                "   LDR  A\n"
                "   ADD  B\n"
                "   STR  TMP1\n"
                "   LDR  C\n"
                "   ADD  E\n"
                "   STR  TMP2\n"
                "   LDR  TMP1\n"
                "   MUL  TMP2\n"
                "   STR  TMP3\n";
        // TEST
        // VERIFY
        assert(infix2Assembler(infix) == assembler);
    }
    {
        //------------------------------------------------------
        // SETUP FIXTURE
        String infix = "( AX * ( BX * ( ( ( CY + AY ) + BY ) * CX ) ) ) ;";
        String assembler = "Infix Expression: ( AX * ( BX * ( ( ( CY + AY ) + BY ) * CX ) ) ) ;\n"
                "Postfix Expression: AX BX CY AY + BY + CX * * *\n"
                "\n"
                "   LDR  CY\n"
                "   ADD  AY\n"
                "   STR  TMP1\n"
                "   LDR  TMP1\n"
                "   ADD  BY\n"
                "   STR  TMP2\n"
                "   LDR  TMP2\n"
                "   MUL  CX\n"
                "   STR  TMP3\n"
                "   LDR  BX\n"
                "   MUL  TMP3\n"
                "   STR  TMP4\n"
                "   LDR  AX\n"
                "   MUL  TMP4\n"
                "   STR  TMP5\n";
        // TEST
        // VERIFY
        assert(infix2Assembler(infix) == assembler);
    }
    {
        //------------------------------------------------------
        // SETUP FIXTURE
        String infix = "( ( H * ( ( ( ( A + ( ( B + C ) * D ) ) * F ) * G ) * E ) ) + J ) ;";
        String assembler = "Infix Expression: ( ( H * ( ( ( ( A + ( ( B + C ) * D ) ) * F ) * G ) * E ) ) + J ) ;\n"
                "Postfix Expression: H A B C + D * + F * G * E * * J +\n"
                "\n"
                "   LDR  B\n"
                "   ADD  C\n"
                "   STR  TMP1\n"
                "   LDR  TMP1\n"
                "   MUL  D\n"
                "   STR  TMP2\n"
                "   LDR  A\n"
                "   ADD  TMP2\n"
                "   STR  TMP3\n"
                "   LDR  TMP3\n"
                "   MUL  F\n"
                "   STR  TMP4\n"
                "   LDR  TMP4\n"
                "   MUL  G\n"
                "   STR  TMP5\n"
                "   LDR  TMP5\n"
                "   MUL  E\n"
                "   STR  TMP6\n"
                "   LDR  H\n"
                "   MUL  TMP6\n"
                "   STR  TMP7\n"
                "   LDR  TMP7\n"
                "   ADD  J\n"
                "   STR  TMP8\n";
        // TEST
        // VERIFY
        assert(infix2Assembler(infix) == assembler);
    }
    {
        //------------------------------------------------------
        // SETUP FIXTURE
        String infix = "( A + ( B * C ) ) / ( D - E ) ) ;";
        String assembler = "Infix Expression: ( A + ( B * C ) ) / ( D - E ) ) ;\n"
                "Postfix Expression: A B C * + D E - /\n"
                "\n"
                "   LDR  B\n"
                "   MUL  C\n"
                "   STR  TMP1\n"
                "   LDR  A\n"
                "   ADD  TMP1\n"
                "   STR  TMP2\n"
                "   LDR  D\n"
                "   SUB  E\n"
                "   STR  TMP3\n"
                "   LDR  TMP2\n"
                "   DIV  TMP3\n"
                "   STR  TMP4\n";
        // TEST
        // VERIFY
        assert(infix2Assembler(infix) == assembler);
    }
    std::cout << "Done testing postfix." << std::endl;
    return 0;
}
