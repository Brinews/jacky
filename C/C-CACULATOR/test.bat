main.exe --tokens "9-8*7+ 90- 34" "9+(8-2)*3 + 5" "sqrt(9 + 16) + 22-12*2"
main.exe --infix "9-8*7+ 90- 34" "9+(8-2)*3 + 5" "sqrt(9 + 16) + 22-12*2"
main.exe --infix2rpn "9-8*7+ 90- 34" "9+(8-2)*3 + 5" "sqrt(9 + 16) + 22-12*2"
main.exe --postfix "9 8 7 * - 90 + 34 -" "9 8 2 - 3 * + 5 +" "9 16 + sqrt 22 + 12 2 * -"
main.exe --infix2rpn "9-8*7+ 90- 34" "9+(8-2)*3 + 5" "sqrt(9 + 16) + 22-12*2" | main.exe --postfix
main.exe "9-8*7+ 90- 34" "9+(8-2)*3 + 5" "sqrt(9 + 16) + 22-12*2"

main.exe --approximate "9-8.1*7+ 90- 34" "9+(8-2)*3 + 5000.99" "sqrt(9 + 16) + 22-12*2"
main.exe --infix2rpn "9-8.1*7+ 90- 34" "9+(8-2)*3 + 5000.99" "sqrt(9 + 16) + 22-12*2" | main.exe --postfix --approximate
main.exe --postfix --approximate "9 8.1 7 * - 90 + 34 -" "9 8 2 - 3 * + 5 +" "9 16 + sqrt 22 + 12 2 * -"
