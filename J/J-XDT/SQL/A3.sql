
-- KXX131 Data Management
-- 2015 Assignment 3
-- SQL Queries


select "ZhangQian"
as qzhang14;



use KXX131RockRoll;


-- Query 0 is an example.
select "What is the date today?"
as "Query 0";

select curdate() as "Today's Date"; 


select "What was the date three years ago?" 
as "Query 1";

select date_add(curdate(),interval -3 year);


select "List, in alphabetic order, the family names of musicians where the given name is five letters long." 
as "Query 2";

select * from artist where length(name)='5';

select "List all the album titles with four or fewer words.  
Sort the list by title." 
as "Query 3";

select title from album 
where title not like '% % % % %'
order by title;

select "How many albums have been released by each label?  
Do not display any NULL values.
Sort the ouput in ascending order of number of albums, and ascending order of label." 
as "Query 4";



select "What is the full name (the family name followed by the given name) and age (in whole years) of all living musician who were born in the USA in the 1960s?  
The youngest musician should be at the top of the list."  
as "Query 5";



select "List the full details of any albums for which the label is not known and there is no record of gigs played for the album." 
as "Query 6";



select "List the full names (in one column headed Whole_Name) of musicians who have played on at least two albums.
Order the list by Whole_Name."
as "Query 7";



select "List the full name of each musician who played on an album that was released after the year in which the musician died.  
Order the results table by the given name followed by the family name."
as "Query 8";

select concat(a.given_name,',',a.family_name) from musician a, gig b, album c where a.musician_id = b.musician_id and b.album_id = c.album_id and a.death_date!='' and c.year!='' and year(a.death_date) < c.year order by a.given_name, a.family_name;


select "List the titles of the album, and the name of the artist who released the album, where a musician who played on the album has the same name as the artist who released the album.
Sort the list by artist name."
as "Query 9";

select b.title, a.name from artist a, album b, gig c, musician d where a.artist_id = b.artist_id and b.album_id = c.album_id and c.musician_id = d.musician_id and lower(a.name) = lower(concat(d.given_name,' ',d.family_name)) order by a.name;


select "Amongst the Australian musicians, who has played on the most albums?
Display the full names of the musician and the number of albums played on."
as "Query 10";

select * from (select a.musician_id as mid, count(*) as cnt from gig a, musician b where a.musician_id = b.musician_id and b.birth_country = 'Australia' group by a.musician_id) as p where p.cnt = (select max(cnt) from (select a.musician_id as mid, count(*) as cnt from gig a, musician b where a.musician_id = b.musician_id and b.birth_country = 'Australia' group by a.musician_id) as t);


select mid, cnt from (select a.musician_id as mid, count(*) as cnt from gig a, musician b where a.musician_id = b.musician_id and b.birth_country = 'Australia' group by a.musician_id) t having cnt >= max(cnt);


select "Which musicians were born on the same day?
The results table should have three columns: Musician, both born on, Musician (in that order).  Musicians should not be paired with themselves, and musician pairings should be unique.
Dates of birth should be displayed in the format 'dd Mmm YYYY' where dd is the day of the month as a two-digit number and Mmm is the abbreviated month name and YYYY is the birth year displayed as four digits. 
Order the output by the dates of birth."
as "Query 11";

select concat(a.given_name, ' ', a.family_name), date_format(a.birth_date,'%d %b %Y'), concat(b.given_name, ' ', b.family_name) from musician a inner join musician b where a.musician_id>b.musician_id and a.birth_date = b.birth_date;



