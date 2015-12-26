/*创建Grading数据库*/
create database [Grading];
use [Grading];

/*-----------------------------------------------*/
/*------------------用户表-----------------------*/
/*-----------------------------------------------*/
/*创建用户表*/
create table [User](
/*用户名*/
[userid] varchar(20) primary key ,
[username] varchar(20) not null,
/*密码*/
[password] varchar(100) not null,
[useremail] varchar(50) not null,
/*角色*/
[role] varchar(50) not null
);

/*功能模块表*/
create table [FunctionModular](
/*模块编码*/
[id] varchar(100) primary key,
/*模块名称*/
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
/*--------------------作业-----------------------*/
/*-----------------------------------------------*/
create table [Assignment](
[id]   int  identity(1, 1)  primary key, /*ID自增做主键*/
[name] nvarchar(64) not null,      /*名称*/
[desc] nvarchar(255) not null,   /*内容*/
[objective] nvarchar(255) not null,         /*目标*/
[critical] nvarchar(255) not null, /*标准*/
[score] int not null,                    /*分数*/
[weight] int not null,	/* 权重 */
[average_score] float,
);
create index [asgn_name]
on [Assignment] ([name] ASC);

/*-----------------------------------------------*/
/*--------------------问题-----------------------*/
/*-----------------------------------------------*/
create table [Questions](
[id]   int  identity(1, 1)  primary key, /*ID自增做主键*/
[asgn_id] int foreign key references [Assignment]([id]), /*属于哪个作业*/
[content] nvarchar(255) not null,/*问题内容*/
);

/*-----------------------------------------------*/
/*--------------------分派-----------------------*/
/*-----------------------------------------------*/
create table [Distribution](
[asgn_id] int foreign key references [Assignment]([id]), /*作业ID*/
[stu_id] varchar(20) foreign key references [User]([userid]),
[is_submit] varchar(1) not null,/*是否提交*/
[myscore] int,
[review] varchar(255) not null
);

/*-----------------------------------------------*/
/*--------------------回答-----------------------*/
/*-----------------------------------------------*/
create table [Answers](
[question_id] int foreign key references [Questions]([id]),	/*问题ID*/
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

