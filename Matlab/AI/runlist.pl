% Autor:
% Datum: 2015-04-02

first([], _).
first([H], F):-F is H.
first([H|_], F):-F is H.

seq([]).
seq([_]).
seq([H|T]):-first(T, F), F>=H.

head([H], [[H]]):-!.
head([H|T], X):-seq([H|T])->head(T, Z), X = [H, Z], !; head(T, Y), X = [[H], Y], !.

runs([], []):-!.
runs([L], [[L]]):-!.
%runs([H|T], X):-H>T-> X=[[H],T],!.
%runs([H|T], [H, T]):-H<T,!.

%runs([H|T], X):-seq([H|T])->runs(T, Z), X = [H, Z], !; runs(T, Y), X = [[H], Y], !.
runs([H, Y|T], [A, B]):-seq([H, Y])->A=[H, Y], runs([T], [B]);A = [H], runs([Y, T], [B]).
%runs([H, Y|T], X):-Y>=H->runs([Y, T], M), X=[H, M],!;runs([Y, T], [B]), X=[[H], B],!.
 
%runs([Head|Tail], [[L]|[Ls]]) :- Head>Tail->L is [Head], runs(Tail, Ls);runs([Head|Tail], [[Ls]]).