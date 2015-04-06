% Autor:
% Datum: 2015-03-31

love(zhangxueyou,wanfei).
love(zhangxueyou,zouhuimin).
love(wanfei,xietinfen).
love(zouhuimin,zhangxueyou).
love(xietinfen,wanfei).
love(xietinfen,zouhuimin).
love(liudehua,zouhuimin).
lovers(X,Y):-love(X,Y),love(Y,X).
