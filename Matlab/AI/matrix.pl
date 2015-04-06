% Autor:
% Datum: 2015-04-02

%init value
equal(X,Y):-X=Y.
%slice one list to several according to the given column,need loop to slice
slice(M,X,L,H):- nth1(M,L,S),prefix(X,L),append(_,[S],X),append(X,H,L).
%average list
average(A,K):- sumlist(K,Sum),length(K,Lenth),A is Sum/Lenth.
%store last result
judg(N,Store,K):- N=1->Store=K;true.
%main logic:1,slice,2,average sliced list,3,store the last result if in the last loop circle,4,loop
read(M,N,L,R,La):- length(L,P),P>0,N>0,(slice(M,X,L,H),average(A,X)
,append(R,[A],Las),write(N),write(A),write(H),write(Las),judg(N,La,Las)
,T is N-1,read(M,T,H,Las,La));!.
%format output
mean(M,N,L,R):-read(M,N,L,La,R).