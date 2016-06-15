/*drop database db_book; */
CREATE database db_book;

use db_book;

DROP TABLE IF EXISTS `my_student`;

/* user information */
CREATE TABLE  `my_student` (
  `sid` int(11) NOT NULL auto_increment,
  `sno` char(10) NOT NULL, /* ���Ŀ��� */
  `sname` char(50) NOT NULL, /* �������� */
  `ssex` char(10) default NULL, /* �������Ա�*/
  PRIMARY KEY  (`sid`, `sno`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 ; 

insert into `my_student` (`sid`, `sno`, `sname`, `ssex`) VALUES 
 (1, '1641001', 'user1', 'male');

insert into `my_student` (`sid`, `sno`, `sname`, `ssex`) VALUES 
 (2, '1641002', 'user2', 'male');

insert into `my_student` (`sid`, `sno`, `sname`, `ssex`) VALUES 
 (3, '1641003', 'user3', 'female');

insert into `my_student` (`sid`, `sno`, `sname`, `ssex`) VALUES 
 (4, 'admin', 'admin', 'male');

DROP TABLE IF EXISTS `my_book`;

CREATE TABLE  `my_book` (
  `bid` int(11) NOT NULL auto_increment,
  `bno` char(10) NOT NULL, /* ��� */
  `bname` char(50) NOT NULL, /* ���� */
  `bauthor` char(50) default NULL,  /* ���� */
  `bprint` char(50) default NULL, /* ������ */
  `bprice` int(16) default NULL, /* �۸� */
  PRIMARY KEY  (`bid`, `bno`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 ; 

insert into my_book values(1, 'TP0001', 'Programming Art', 'Meyer', 'Science Print', 56);
insert into my_book values(2, 'TC0003', 'Communication pricinble', 'Meyer', 'Science Print', 66);
insert into my_book values(3, 'FA0001', 'History of China', 'YiZhongTian', 'HePin Print', 36);

DROP TABLE IF EXISTS `my_record`;

CREATE TABLE  `my_record` (
  `rid` int(11) NOT NULL auto_increment,
  `rsno` char(10) NOT NULL, /* �����˿��� */
  `rbno` char(10) NOT NULL, /* �������� */
  `fromdate` datetime default NULL, /* ���ʱ�䡡*/
  `todate` datetime default NULL, /* ����ʱ�䡡*/
  PRIMARY KEY  (`rid`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 ; 

