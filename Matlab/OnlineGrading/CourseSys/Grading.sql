/*����Grading���ݿ�*/
create database [Grading];
use [Grading];

/*-----------------------------------------------*/
/*------------------�û���-----------------------*/
/*-----------------------------------------------*/
/*�����û���*/
create table [User](
/*�û���*/
[userid] varchar(20) primary key ,
[username] varchar(20) not null,
/*����*/
[password] varchar(100) not null,
[useremail] varchar(50) not null,
/*��ɫ*/
[role] varchar(50) not null
);

/*����ģ���*/
create table [FunctionModular](
/*ģ�����*/
[id] varchar(100) primary key,
/*ģ������*/
[name] varchar(50) not null,
[role] varchar(32) not null,
);

insert into [User] values('T001','admin','123','1@hotmail.com','teacher');
insert into [User] values('S002','stu','123','2@qq.com','student');

insert into [FunctionModular] values ('StuSys','student management', 'teacher');
insert into [FunctionModular] values ('AssignSys','assignment management', 'teacher');
insert into [FunctionModular] values ('ScoreSys','mark management', 'teacher');
insert into [FunctionModular] values ('SubmitSys','assignement submit', 'student');
insert into [FunctionModular] values ('SummarySys','summary view', 'student');
insert into [FunctionModular] values ('PassSys','Password Change', 'student');

/*-----------------------------------------------*/
/*--------------------��ҵ-----------------------*/
/*-----------------------------------------------*/
create table [Assignment](
[id]   int  identity(1, 1)  primary key, /*ID����������*/
[name] nvarchar(64) not null,      /*����*/
[desc] nvarchar(255) not null,   /*����*/
[objective] nvarchar(255) not null,         /*Ŀ��*/
[critical] nvarchar(255) not null, /*��׼*/
[score] int not null,                    /*����*/
[weight] int not null,	/* Ȩ�� */
[average_score] float,
);
create index [asgn_name]
on [Assignment] ([name] ASC);

/*-----------------------------------------------*/
/*--------------------����-----------------------*/
/*-----------------------------------------------*/
create table [Questions](
[id]   int  identity(1, 1)  primary key, /*ID����������*/
[asgn_id] int foreign key references [Assignment]([id]), /*�����ĸ���ҵ*/
[content] nvarchar(255) not null,/*��������*/
);

/*-----------------------------------------------*/
/*--------------------����-----------------------*/
/*-----------------------------------------------*/
create table [Distribution](
[asgn_id] int foreign key references [Assignment]([id]), /*��ҵID*/
[stu_id] varchar(20) foreign key references [User]([userid]),
[is_submit] varchar(1) not null,/*�Ƿ��ύ*/
[myscore] int,
[review] varchar(255) not null
);

/*-----------------------------------------------*/
/*--------------------�ش�-----------------------*/
/*-----------------------------------------------*/
create table [Answers](
[question_id] int foreign key references [Questions]([id]),	/*����ID*/
[stu_id] varchar(20) foreign key references [User]([userid]), 
[answer] varchar(255),
[review] varchar(255),
);


/*-----------------------------------------------*/
/*--------------------GPA-----------------------*/
/*-----------------------------------------------*/
create table [Gpa](
[stu_id] varchar(20) foreign key references [User]([userid]),
[gpa] float not null,
[level] char,
);

