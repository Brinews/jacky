% Autor:
% Datum: 2015-04-02

longest([L], L) :- !.
longest([H|T], H) :- length(H, N), longest(T, X), length(X, M), N > M,!.
longest([H|T], X) :- longest(T, X),!.