% Autor:
% Datum: 2015-04-02

cons(A,T,[A|T]):-!.
append([],Y,Y):-!.
append([H|T],Y,Z):-append(T,Y,G),cons(H,G,Z).
ca(plus,X,Y,Z):-Z is X+Y.
ca(minus,X,Y,Z):-Z is X-Y.
ca(time,X,Y,Z):-Z is X*Y.
ca(over,X,Y,Z):-Z is X/Y.
vic(24).
point(A,B,C,D,C1,C2,C3):-ca(C1,A,B,V1),ca(C2,V1,C,V2),ca(C3,V2,D,V3),vic(V3).
tio:-tell('pp.txt'),write('nimanima'),told.
fz([],OL,OL).
fz([H|T],TL,OL):-cons(H,TL,TTL),fz(T,TTL,OL).
fz(T,OL):-fz(T,[],OL).
index([OBJ|_],OBJ,N,N).
index([_|T],OBJ,N,R):-N2 is N+1,index(T,OBJ,N2,R).
index(T,OBJ,R):-index(T,OBJ,0,R).
item([],_,_):-write('item error').
item([H|_],0,H).
item([_|T],N,R):-N2 is N-1,item(T,N2,R).
del([_|T],HL,0,OL):-fz(HL,HL2),append(HL2,T,OL).
del([],_,_,_):-write('del error').
del([H|T],HL,N,OL):-cons(H,HL,HL2),N2 is N-1,del(T,HL2,N2,OL).
del(T,N,R):-del(T,[],N,R).
remove(L,OBJ,R):-index(L,OBJ,N),del(L,N,R).
list(A,R):-cons(A,[],R).
pl([],[],BL,R):-list(BL,R).
pl([],_,_,[]).
pl([H|T],SL,BL,R):-pl(T,SL,BL,ML2),remove(SL,H,CL),cons(H,BL,BL2),pl(CL,CL,BL2,ML3),append(ML2,ML3,R).
pl(T,R):-pl(T,T,[],R).
foror([]):-fail.
foror(£ÛA,B,C,D]|T]):-foror(T);point(A,B,C,D,X,Y,Z),write(A),write(X),write(B),write(Y),write(C),write(Z),write(D),nl.
point24(T):-pl(T,T2),foror(T2).
p24(A,B,C,D):-point24([A,B,C,D]).