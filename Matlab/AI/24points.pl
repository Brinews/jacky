% Autor:
% Datum: 2015-04-02

/* RPN = Reverse Polish notation */

operator(add).
operator(sub).
operator(mul).
operator(div).

doop(add, A, B, C) :- C = A + B.
doop(sub, A, B, C) :- C = A - B.
doop(mul, A, B, C) :- C = A * B.
doop(div, A, B, C) :- B1 is B, B1 \= 0, C = A / B.

/* rpn2exp(Stack: List, Prog: List, Result: Int, Expr) */
rpn2exp([X], [], X).
rpn2exp(Stack, [Head|Tail], Res) :-
integer(Head),
rpn2exp([Head|Stack], Tail, Res).
rpn2exp([X,Y|Others], [Head|Tail], Res) :-
operator(Head),
doop(Head, Y, X, X1),
rpn2exp([X1|Others], Tail, Res).

eval_rpn(Prog, Res) :- rpn2exp([], Prog, Exp), Res is Exp.

try_prog([A, B, C, D], [A, B, X, C, Y, D, Z], N) :- eval_rpn([A, B, X, C, Y, D, Z], N).
try_prog([A, B, C, D], [A, B, X, C, D, Y, Z], N) :- eval_rpn([A, B, X, C, D, Y, Z], N).
try_prog([A, B, C, D], [A, B, C, X, D, Y, Z], N) :- eval_rpn([A, B, C, X, D, Y, Z], N).
try_prog([A, B, C, D], [A, B, C, X, Y, D, Z], N) :- eval_rpn([A, B, C, X, Y, D, Z], N).
try_prog([A, B, C, D], [A, B, C, D, X, Y, Z], N) :- eval_rpn([A, B, C, D, X, Y, Z], N).

solve24order(Numbers, Prog) :- try_prog(Numbers, Prog, 24).
solve24order(Numbers, Prog) :- try_prog(Numbers, Prog, 24.0).

solve24(Numbers, Prog) :- setof(T, lists:perm(Numbers, T), Bag), lists:member(T1, Bag), solve24order(T1, Prog), rpn2exp([], Prog, E), write(E), nl, fail.