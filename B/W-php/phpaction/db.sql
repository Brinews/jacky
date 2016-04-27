DROP database mytask;
CREATE database mytask;

use mytask;

DROP TABLE IF EXISTS `my_user`;

/* user information */
CREATE TABLE  `my_user` (
  `uid` int(11) NOT NULL auto_increment,
  `uemail` char(50) NOT NULL,
  `upwd` char(50) NOT NULL,
  `uname` char(20) default NULL,
  `uphone` char(15) default NULL,
  `utype` char(2) default NULL, /* 00: normal user, 01: admin */
  PRIMARY KEY  (`uid`, `uemail`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 ; 

insert into `my_user` (`uid`, `uemail`, `uname`, `upwd`, `uphone`, `utype`) VALUES 
 (1, 'user@gmail.com', 'user', '123', '931234', '00');

insert into `my_user` (`uid`, `uemail`, `upwd`, `uname`, `uphone`, `utype`) VALUES 
 (2, 'admin@gmail.com', 'admin', '123', '931236', '01');

insert into `my_user` (`uid`, `uemail`, `upwd`, `uname`, `uphone`, `utype`) VALUES 
 (3, 'user2@gmail.com', 'user2', '123', '931236', '00');

DROP TABLE IF EXISTS `my_task`;

CREATE TABLE  `my_task` (
  `uid` int(11) NOT NULL auto_increment,
  `utaskname` char(50) NOT NULL,
  `utaskdesc` char(50) NOT NULL,
  `uspace` int(16) default NULL, /* space number */
  `ulocation` char(50) default NULL,
  `ubegin` datetime default NULL,
  `uend` datetime default NULL,
  `ustate` char(2) default NULL, /* 00: open, 01: closed */
  PRIMARY KEY  (`uid`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 ; 

insert into my_task values(1, 'test', 'hello everyone', 1, 'North',
							'2016-03-24', '2016-04-26', '00');

DROP TABLE IF EXISTS `my_attend`;

CREATE TABLE  `my_attend` (
  `uid` int(11) NOT NULL auto_increment,
  `userid` int(11) NOT NULL,
  `taskid` int(11) NOT NULL,
  `registerdate` datetime default NULL,
  PRIMARY KEY  (`uid`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 ; 

