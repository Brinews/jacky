create database china111;
use china111;

create table account (
		name varchar(20) not null primary key,
		passwd varchar(20) not null,
		tel varchar(20) not null,
		born varchar(20) not null
		);

create table film (
		name varchar(20) not null primary key,
		cinema varchar(20) not null,
		price int(8) not null,
		datetime date not null,
		seat varchar(32) not null default '00000000000000000000000000000000'
		);

insert into account values ('test', '123', '1235678', '1992-04-07');
insert into film values ('Speed', 'Sky', 35, '2017-03-04', '0001110000000000000000000110');

