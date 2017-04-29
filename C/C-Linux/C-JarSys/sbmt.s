/To add two numbers
    ORG 100 /Program starts at addr 100
    LDA A   / AC = 0

    / read number
CIF,SKI
    BUN CIF
    INP
    /OUT

    ADD B  / character to digit
    STA A

CIM,SKI
    BUN CIM
    INP
    /OUT

    / translate into number
    ADD B
    STA C

    LDA A
    /MUL A*10
LOP,CLE
    LDA A
    CIR
    STA A
    SZE
    BUN ONE
    BUN ZRO
ONE,LDA D
    ADD P
    STA P
    CLE
ZRO,LDA D
    CIL
    STA D
    ISZ CTR
    BUN LOP

    LDA P
    ADD C / a*10+b
    STA A

    / do sum
SUM,CLE
    LDA F / begin with F
    CMA
    INC
    ADD A / F = A ?
    SZA
    BUN UEQ
    BUN EQL

UEQ,ADD N / F = A-1 ?
    SZA
    BUN LES
    BUN EQL

LES,LDA F
    ADD S
    STA S
    LDA F
    ADD T
    STA F
    BUN SUM

EQL,LDA F
    ADD S
    STA S / S = sum
    STA U / U = sum, for output

    /OUTPUT CHANGE
SUA,CLE / for thousand
    LDA Z
    ADD U
    SNA
    BUN COA
    BUN THU
COA,STA U
    LDA V
    INC
    STA V
    BUN SUA

THU,LDA V
    ADD W
    OUT

    CLA
    STA V /restore v=0

SUB,CLE / for hundred
    LDA Y
    ADD U
    SNA
    BUN COB
    BUN HUN
COB,STA U
    LDA V
    INC
    STA V
    BUN SUB

HUN,LDA V
    ADD W
    OUT

    CLA
    STA V /restore v=0

SUC,CLE  / for ten
    LDA X
    ADD U
    SNA
    BUN CON
    BUN TEN
CON,STA U
    LDA V
    INC
    STA V
    BUN SUC

TEN,LDA V
    ADD W
    OUT

    LDA U / individual number
    ADD W
    OUT
    
    HLT

CTR,DEC -8
A,  DEC 0
B,  DEC -48
C,  DEC 0

D,  DEC 10
P,  DEC 0

F,  DEC 1
S,  DEC 0
T,  DEC 2
N,  DEC -1

U,  DEC 0
V,  DEC 0
X,  DEC -10
Y,  DEC -100
Z,  DEC -1000
W,  DEC 48

    END

