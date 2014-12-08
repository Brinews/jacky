Infix Expression: Infix Expression: ( AX + ( B * C ) ) ;
Postfix Expression: Infix Expression: AX B C * +

   LDR  B
   MUL  C
   STR  TMP1
   LDR  AX
   ADD  TMP1
   STR  TMP2

Infix Expression: Postfix Expression: AX B C * +
Postfix Expression: Postfix Expression: AX B C * +

   LDR  B
   MUL  C
   STR  TMP1
   LDR  AX
   ADD  TMP1
   STR  TMP2

Infix Expression: Infix Expression: ( ( AX + ( B * CY ) ) / ( D - E ) ) ;
Postfix Expression: Infix Expression: AX B CY * + D E - /

   LDR  B
   MUL  CY
   STR  TMP1
   LDR  AX
   ADD  TMP1
   STR  TMP2
   LDR  D
   SUB  E
   STR  TMP3
   LDR  TMP2
   DIV  TMP3
   STR  TMP4

Infix Expression: Postfix Expression: AX B CY * + D E - /
Postfix Expression: Postfix Expression: AX B CY * D E + / -

   LDR  B
   MUL  CY
   STR  TMP1
   LDR  D
   ADD  E
   STR  TMP2
   LDR  TMP1
   DIV  TMP2
   STR  TMP3
   LDR  AX
   SUB  TMP3
   STR  TMP4

Infix Expression: Infix Expression: ( ( A + B ) * ( C + E ) ) ;
Postfix Expression: Infix Expression: A B + C E + *

   LDR  A
   ADD  B
   STR  TMP1
   LDR  C
   ADD  E
   STR  TMP2
   LDR  TMP1
   MUL  TMP2
   STR  TMP3

Infix Expression: Postfix Expression: A B + C E + *
Postfix Expression: Postfix Expression: A B C E + * +

   LDR  C
   ADD  E
   STR  TMP1
   LDR  B
   MUL  TMP1
   STR  TMP2
   LDR  A
   ADD  TMP2
   STR  TMP3

Infix Expression: Infix Expression: ( AX * ( BX * ( ( ( CY + AY ) + BY ) * CX ) ) ) ;
Postfix Expression: Infix Expression: AX BX CY AY + BY + CX * * *

   LDR  CY
   ADD  AY
   STR  TMP1
   LDR  TMP1
   ADD  BY
   STR  TMP2
   LDR  TMP2
   MUL  CX
   STR  TMP3
   LDR  BX
   MUL  TMP3
   STR  TMP4
   LDR  AX
   MUL  TMP4
   STR  TMP5

Infix Expression: Postfix Expression: AX BX CY AY + BY + CX * * *
Postfix Expression: Postfix Expression: AX BX CY AY BY + CX * * * +

   LDR  AY
   ADD  BY
   STR  TMP1
   LDR  TMP1
   MUL  CX
   STR  TMP2
   LDR  CY
   MUL  TMP2
   STR  TMP3
   LDR  BX
   MUL  TMP3
   STR  TMP4
   LDR  AX
   ADD  TMP4
   STR  TMP5

Infix Expression: Infix Expression: ( ( H * ( ( ( ( A + ( ( B + C ) * D ) ) * F ) * G ) * E ) ) + J ) ;
Postfix Expression: Infix Expression: H A B C + D * + F * G * E * * J +

   LDR  B
   ADD  C
   STR  TMP1
   LDR  TMP1
   MUL  D
   STR  TMP2
   LDR  A
   ADD  TMP2
   STR  TMP3
   LDR  TMP3
   MUL  F
   STR  TMP4
   LDR  TMP4
   MUL  G
   STR  TMP5
   LDR  TMP5
   MUL  E
   STR  TMP6
   LDR  H
   MUL  TMP6
   STR  TMP7
   LDR  TMP7
   ADD  J
   STR  TMP8

Infix Expression: Postfix Expression: H A B C + D * + F * G * E * * J +
Postfix Expression: Postfix Expression: H A B C D * + F G * E * * J * + +

   LDR  C
   MUL  D
   STR  TMP1
   LDR  B
   ADD  TMP1
   STR  TMP2
   LDR  F
   MUL  G
   STR  TMP3
   LDR  TMP3
   MUL  E
   STR  TMP4
   LDR  TMP2
   MUL  TMP4
   STR  TMP5
   LDR  TMP5
   MUL  J
   STR  TMP6
   LDR  A
   ADD  TMP6
   STR  TMP7
   LDR  H
   ADD  TMP7
   STR  TMP8

