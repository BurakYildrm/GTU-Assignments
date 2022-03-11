:-dynamic student/3.
:-dynamic room/5.
:-dynamic course/7.
:-dynamic student_ids/1.
:-dynamic room_ids/1.
:-dynamic course_ids/1.
% courses
% argument order is course id, instructor id, capacity, start hour, finish hour, room, special needs
course(cse341, 1, 100, 10, 12, z10, _).
course(cse343, 2, 50, 15, 18, z10, smart_board).
course(cse222, 3, 70, 14, 16, z23, projector).
course(cse241, 4, 150, 10, 12, z23, _).
course(cse101, 5, 80, 13, 15, z06, _).
% rooms 
% argument order is room id, capacity, hours, special equipments, handicapped access
room(z10, 100, [8, 9, 10, 11, 12, 13 ,14, 15, 16, 17], [smart_board], yes).
room(z23, 170, [8, 9, 10, 11, 12, 13 ,14, 15, 16, 17], [projector], no).
room(z06, 80, [8, 9, 10, 11, 12, 13 ,14, 15, 16, 17], [], yes).
% occupancy info of the rooms
% argument order is room id, course id, hours
occupancy(z10, cse341, [10, 11]).
occupancy(z10, cse343, [15, 16, 17]).
occupancy(z23, cse222, [14, 15]).
occupancy(z23, cse241, [10, 11]).
occupancy(z06, cse101, [13, 14]).
% intructors
% argument order is intructor id, instructor name, courses taugth, preference
instructor(1, yakup_genc, [cse341], _).
instructor(2, habil_kalkan, [cse343], smart_board).
instructor(3, erdogan_sevilgen, [cse222], projector).
instructor(4, yusuf_sinan_akgul, [cse241], _).
instructor(5, mehmet_gokturk, [cse101], _).
% students
% argument order is student id, courses attended, is handicapped
student(1, [cse341], no).
student(2, [cse343], yes).
student(3, [cse222, cse241], no).
student(4, [cse343, cse341], no).
student(5, [cse101], yes).
% ids
student_ids([1, 2, 3, 4, 5]).
room_ids([z10, z23, z06]).
course_ids([cse341, cse343, cse222, cse241, cse101]).

% if the student id is not already taken, the student is added to the system
addStudent(ID, Courses, IsHandicapped) :-
    student_ids(IDList),
    not(member(ID, IDList)),
    assertz(student(ID, Courses, IsHandicapped)),
    retract(student_ids(IDList)),
    assertz(student_ids([ID | IDList])).

% the course is added to the system if the start and finish times of the course is not conflicted with the instructors courses and room is available between given start
% and finish hours and the room is equiped with the instructor's preference and the capacity of the course is less than or equal to the capacity of the room.
addCourse(CourseID, InstructorID, Cap, Start, Finish, RoomID, SpecN) :-
    course_ids(IDList),
    not(member(CourseID, IDList)),
    LastHour is Finish - 1,
    checkAvailability(RoomID, _, Start, LastHour),
    instructor(InstructorID, _, Courses, Preference),
    instructor(InstructorID, _, Courses, _),
    forall(member(E, Courses), checkAvailability(_, E, Start, LastHour)),
    room(RoomID, RoomCap, _, SpecEq, _),
    member(SpecN, SpecEq),
    member(Preference, SpecEq),
    integer(Cap),
    Cap =< RoomCap,
    assertz(course(CourseID, InstructorID, Cap, Start, Finish, RoomID, SpecN)),
    retract(course_ids(IDList)),
    assertz(course_ids([CourseID | IDList])).

% checks if the instructors courses conflicts with the given start and finish times.
checkAvailability(RoomID, CourseID, Start, Finish) :-
    forall(occupancy(RoomID, CourseID, Occupied), forall(between(Start, Finish, H), not(member(H, Occupied)))).

% if the room id is not already taken, the room is added to the system.
addRoom(ID, Cap, Hours, SpecEq, Access) :-
    room_ids(IDList),
    not(member(ID, IDList)),
    assertz(room(ID, Cap, Hours, SpecEq, Access)),
    retract(room_ids(IDList)),
    assertz(room_ids([ID | IDList])).

% checks if the given two courses conflict by checking if they are in the same room in conflicting hours.
conflict(CourseID1, CourseID2) :-
    CourseID1 \= CourseID2,
    course(CourseID1, _ ,_, _, _, Room1, _),
    course(CourseID2, _ ,_, _, _, Room2, _),
    Room1 = Room2,
    occupancy(Room1, CourseID1, Hours1),
    occupancy(Room2, CourseID2, Hours2),
    checkConflict(Hours1, Hours2).

% checks if the second list includes at least one of the elements in the first list.
checkConflict([], List) :- false, !.
checkConflict([H | T], List) :-
    (member(H, List), true, !;
    not(member(H, List)), checkConflict(T, List)).