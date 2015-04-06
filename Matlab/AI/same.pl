% Autor:
% Datum: 2015-04-01

person(A):-male(A);female(A).

is_father(P, Q):-person(P),person(Q),male(P),parent(P, Q).

father(P, Q):-is_father(P, Q);is_father(Q, P).

same_name(P, Q):-person(P), person(Q), father(P,Q).
same_name(P, Q):-father(P, R), same_name(R, Q).