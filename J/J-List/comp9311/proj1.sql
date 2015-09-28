-- COMP9311 15s2 Project 1
--
-- MyMyUNSW Solution Template

-- Q1: ...
create or replace view Q1(familyName)
as
select family from (select people.family, length(people.family) as lfam from students left join people on students.id = people.id and strpos(people.family, ' ') = 0 and strpos(people.family, '-') = 0 group by people.family having count(*)=1 ) T  where lfam >= (select max(lfam) from (select people.family, length(people.family) as lfam from students left join people on students.id = people.id and strpos(people.family, ' ') = 0 and strpos(people.family, '-') = 0 group by people.family having count(*)=1 ) T1)
;


-- Q2: ...
create or replace view Q2(subject,semester)
as
select subjects.code, substring(concat(semesters.year, semesters.term) from 3) from courses , subjects, semesters where  courses.id in (select course from course_enrolments where grade='A' or grade='B' or grade='C') and courses.subject = subjects.id and courses.semester = semesters.id
;


-- Q3: ...
create or replace view Q3(ratio,nsubjects)
as
select cast(uoc/eftsload as numeric(4,1)), count(*) from subjects where eftsload>0 group by cast(uoc/eftsload as numeric(4, 1))
;


-- Q4: ...
create or replace view Q4(orgunit)
as
select distinct(longname) from orgunits where id not in (select member from orgunit_groups)
;


-- Q5: ...

create or replace view Q5(code, title)
as
select code, longname from subjects where substring(code from 1 for 4)='COMP' and id in (select distinct(courses.subject) from courses, semesters where courses.semester= semesters.id and (semesters.year = 2008) and courses.subject not in (select distinct(courses.subject) from courses, semesters where courses.semester= semesters.id and (semesters.year = 2009 or semesters.year=2010)))
;


-- Q6: ...
create type EvalRecord as (code text, title text, rating numeric(4,2));
create type MidRecord as (courseId integer, avgEval numeric(4,2));

create or replace function Q6_1(_year integer, _term text)
    returns setof MidRecord
as $$
declare
    rec MidRecord;
begin
    for rec in select course, cast(avg(stueval) as numeric(4,2)) from course_enrolments group by course having count(student) > 10 and count(stueval)*3 > count(student) and course in (select courses.id from courses where courses.semester in (select distinct(id) from semesters where year=_year and term=_term))
    loop
        return next rec;
    end loop;

    return;
end;
$$ language plpgsql
;


create or replace function Q6(_year integer, _term text) 
    returns setof EvalRecord 
as $$
declare
    rec EvalRecord;
    avgMax numeric(4,2) := 0;

begin

    select into avgMax max(avgEval) from Q6_1(_year,_term);

    for rec in select code, name, q.avgEval from Q6_1(_year, _term) q, subjects, courses where q.avgEval = avgMax and courses.id = q.courseId and subjects.id=courses.subject
    loop
        return next rec;
    end loop;

    return;
end;
$$ language plpgsql
;

