% Autor:
% Datum: 2015-04-03

append([],T,T).
append(T,[],T).  % 说明空表是单位元
append([X|H],T,[X|R]):-append(H,T,R).

size([],0).  % 函数的返回之定义为最有一个变量空表的长度为0
size([X|H],RETURN):-RETURN = size(H) + 1.


reverse([],[]).
reverse(H,[X|T]):-append(R,[X],H),reverse(R,T).

%reverse([X,Y|H],T):-reverse([Y|H],R), append(R,[X],T).


delete(X,H,R):-append(M,[X|N],H), append(M,N,R).

permutation([X],[X]). % 单个元素的排列
permutation([X|H],T):-permutation(H,R), append(M,N,R), append(M,[X|N],T).


value = symbol
node = node(value,node,node)

null = node(nil,_,_)

enumerator = value*
nodelist = node*


isnull(node)
splitnode(node(a,node(b,null,null),null),VALUE,LEFT,RIGHT):-VALUE = a,LEFT = node(b,null,null),RIGHT = null

% 前序
preorder(null,[]):-!.  % 空表的处理
        preorder(node(VALUE,LEFT,RIGHT),E):-
          preorder(LEFT,EL),
          preorder(RIGHT,ER),
          append([E|EL],ER,E).

%中序
 inorder(null,[]):-!.
inorder(node(VALUE,LEFT,RIGHT)):-
          inorder(LEFT,EL),
          inorder(RIGHT,ER),
          append(EL,[VALUE|ER],E).


%后序
postorder(node,enumerator) %将以后序遍历以node的为节点的树并将结果依 次置入表中于是
        postorder(null,[]).
        postorder(node(VALUE,LEFT,RIGHT),E):-
          postorder(LEFT,EL),
          postorder(RIGHT,ER),
          append(EL,ER,EE),
          append(EE,[VALUE],E).

%深度优先
deepsearch(V,NODE,NODE):-  % 刚好查找到的情况
          not(isnull(NODE)),
          splitnode(NODE,VALUE,_,_),
          V = VALUE.
        deepsearch(V,NODE,N):-   % 向左子树查找的情况
          not(isnull(NODE)),
splitnode(NODE,VALUE,LEFT,_),
          deepsearch(V,LEFT,N).
        deepsearch(V,NODE,N):-   % 向右子树查找的情况
          not(isnull(NODE)),
          splitnode(NODE,VALUE,_,RIGHT),
          deepsearch(V,RIGHT,N).

%广度优先
widesearch(V,ROOT,N):-
          wide(V,[ROOT],_,N).
        wide(V,[NODE|E],NE,NODE):-  % 刚好找到的情况
          not(isnull(NODE)),
          splitnode(NODE,VALUE,_,_),
          V = VALUE.
        wide(V,[NODE|E],NE,N):-
          not(isnull(NODE)),
          splitnode(NODE,VALUE,LEFT,RIGHT),
          aqqend(E,[LEFT],EL),  % 节点入队列
          aqqend(EL,[RIGHT],ER), % 节点入队列
          delnull(ER,EE),  % 删除所有空节点
          wide(V,EE,NE,N).

delnull([],[]):-!.
        delnull([null|E],R):-
          delnull(E,R).
        delnull([NODE|E],[NODE|R]):-
          delnull(E,R).


%归并排序
msort(array H,array T)将表H排序成为表T则
      msort([],[]).    % 对空表的处理
      msort([X],[X]).  % 单个元素的处理
      msort([X,Y|H],T):-  % 两个以上元素的处理
        split([X,Y|H],L,R),
        msort(L,SL),
        msort(R,SR),
        merge(SL,SR,T).

split(H,L,R):-
        S = size(H) div 2,  % 一半
        splitby(S,H,L,R).
      splitby(0,X,[],X).  % 特殊情况的处理
      splitby(N,[X|H],[X|L],R):-
        N  0,
        N1 = N - 1,
        splitby(N1,H,L,R).

merge([],R,R).
      merge(L,[],L).
      merge([X|L],[Y|R],[X|T]):-
        X = Y,
        merge(L,[Y|R],T).
      merge([X|L],[Y|R],[Y|T]):-
        X  Y,
        merge([X|L],R,T).

%快速排序
qsort([],[]).
      qsort([X],[X]).

      qsort([X|H],T):-
        partition(X,[X|H],L,R),
        qsort(L,SL),
        qsort(R,SR),
        append(SL,SR,T).

partition(X,[],[],[]).
      partition(X,[Y|H],[Y|L],R):-  % 小于等于X的元素放入表L
        Y = X,
        partition(X,H,L,R).
      partition(X,[Y|H],L,[Y|R]):-  % 大于X的元素放入表R
        Y  X,
        partition(X,H,L,R).


%后缀转前缀
pos2pre(Pos, Pre) :-
    append([A, B, [O]], Pos), isop(O), A \= [], B \= [],
    pos2pre(A, APre),
    pos2pre(B, BPre),
    !, append([[O], APre, BPre], Pre).
pos2pre([P], [P]).

