% Autor:
% Datum: 2015-04-02

select_element(Goal, [Head | Tail], Selected) :-
    select_element(Goal, Tail, Head, Selected).


select_element(_Goal, [], Selected, Selected).

select_element(Goal, [Head | Tail], Current, FinalSelected) :-
    call(Goal, Head, Current, Selected),
    select_element(Goal, Tail, Selected, FinalSelected).

%Lets say you define a predicate

get_bigger_number(N1, N2, N) :-
    N is max(N1, N2).

%Now you can execute:

%?- select_element(get_bigger_number, [5, 1, -2, 10, 3.2, 0], Selected).

%Selected = 10
