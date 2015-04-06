% Autor:
% Datum: 2015-04-02

item([],_,_):-fail.
item([H|_],0,H):-!.
item([_|T],N,R):-N2 is N-1,item(T,N2,R).
fz([],OL,OL).
fz([H|T],TL,OL):-cons(H,TL,TTL),fz(T,TTL,OL).
fz(T,OL):-fz(T,[],OL).
index([OBJ|_],OBJ,N,N).
index([_|T],OBJ,N,R):-N2 is N+1,index(T,OBJ,N2,R).
index(T,OBJ,R):-index(T,OBJ,0,R).
del([_|T],HL,0,OL):-fz(HL,HL2),append(HL2,T,OL).
del([],_,_,_):-write('del error').
del([H|T],HL,N,OL):-cons(H,HL,HL2),N2 is N-1,del(T,HL2,N2,OL),!.
del(T,N,R):-del(T,[],N,R).
remove(L,OBJ,R):-index(L,OBJ,N),del(L,N,R).
list(A,R):-cons(A,[],R).
cons(A,T,[A|T]):-!.
append([],Y,Y):-!.
append([H|T],Y,Z):-append(T,Y,G),cons(H,G,Z).
index2([],_,_,_):-fail. %这个考虑到多用途，如果到时候出完备性问题再单独划分出去
index2(［OBJ|_]|_],OBJ,N,N):-!.
index2([_|T],OBJ,N,R):-N2 is N+1,index2(T,OBJ,N2,R).
index2(T,OBJ,R):-index2(T,OBJ,0,R).
do:-write(do).
la:-write(la).
mi:-write(mi).
fa:-write(fa).
so:-write(so).
la:-write(la).
xi:-write(xi).

link(a,f).
link(f,a).
link(f,b).
link(b,f).
door(a,c).
door(c,a).
link(c,d).
link(d,c).
location(key,b).
location(box,d).
location(sword,a).

ini(L):-add([],i,a,L).
add(L,Word,Item,R):-cons(Item,[],NL),cons(Word,NL,FL),cons(FL,L,R),!.
exchange(L,Word,Item,R):-index2(L,Word,Count),del(L,Count,NewL),add(NewL,Word,Item,R),!.
in(L,R):-index2(L,i,X),item(L,X,ML),item(ML,1,R),!.
se:-ini(L),act(Any,L,Infomation,12,[]),write(great ),fz(Infomation,FI),write(FI).

actr(A,B,C,N,E):-X is N-1,cwrite(N),act(A,B,C,X,E).
act(_,_,_,0,_):-write(t),!,fail.
%act(cry,I,O,N,B):-write(c),actr(Any,I,O,N).
act(move,I,O,N,B):-in(I,X),link(X,Y),exchange(I,i,Y,TI),write(move),cons([move,X,Y],B,BI),actr(Any,TI,O,N,BI).
act(pick,I,O,N,B):-in(I,X),location(Y,X),\+(index2(I,Y,_)),add(I,Y,t,TI),write(pick),cons([pick,Y],B,BI),actr(Any,TI,O,N,BI).
act(movedoor,I,O,N,B):-in(I,X),door(X,Y),index2(I,key,_),exchange(I,i,Y,TI),write(movedoor),cons([movedoor,X,Y],B,BI),actr(Any,TI,O,N,BI).
act(victory,I,B,N,B):-in(I,c),index2(I,box,_).

cwrite(TXT):-TXT > 1,write(TXT).
cwrite(TXT).
g1(N):-!,fail.
g1(0):-write(0).