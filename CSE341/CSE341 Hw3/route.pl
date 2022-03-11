/*canakkale flights*/
flight(canakkale, erzincan, 6).
/*erzincan flights*/
flight(erzincan, canakkale, 6).
flight(erzincan, antalya, 3).
/*antalya flights*/
flight(antalya, erzincan, 3).
flight(antalya, izmir, 2).
flight(antalya, diyarbakir, 4).
/*diyarbakir flights*/
flight(diyarbakir, antalya, 4).
flight(diyarbakir, ankara, 8).
/*izmir flights*/
flight(izmir, antalya, 2).
flight(izmir, ankara, 6).
flight(izmir, istanbul, 2).
/*istanbul flights*/
flight(istanbul, izmir, 2).
flight(istanbul, ankara, 1).
flight(istanbul, rize, 4).
/*rize flights*/
flight(rize, istanbul, 4).
flight(rize, ankara, 5).
/*van flights*/
flight(van, ankara, 4).
flight(van, gaziantep, 3).
/*gaziantep flights*/
flight(gaziantep, van, 3).
/*ankara flights*/
flight(ankara, diyarbakir, 8).
flight(ankara, izmir, 6).
flight(ankara, istanbul, 1).
flight(ankara, rize, 5).
flight(ankara, van, 4).
/*to get the first element of the list*/
head_of_list([H | _], H).

%checks if there is a direct route between X and Y with a cost of C.
direct_route(X, Y, C) :- flight(X, Y, C).

%checks if there is route between X and Y with a cost of C.
route(X, Y, C) :- find_route(X, Y, C, [], []).

%finds all the possible routes between X and Y with a cost of C.
find_route(X, Y, C, Visited, Cost) :- 
    direct_route(X, Z, D),
    not(member(Z, Visited)),
    length(Cost, Len),
    (Y = Z, (Len = 0, C = D; head_of_list(Cost, H), C is H + D), reverse([Z | [X | Visited]], Reversed), print_list(Reversed);
    (Len = 0, find_route(Z, Y, C, [X | Visited], [D | Cost]);
    head_of_list(Cost, H), find_route(Z, Y, C, [X | Visited], [D + H | Cost]))).

%prints the list.
print_list([]).
print_list([H | T]) :-
    length(T, L),
    (L = 0, format("~w~n", [H]); L \= 0, format("~w -> ", [H])),
    print_list(T).
