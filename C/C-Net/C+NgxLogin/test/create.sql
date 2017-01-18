CREATE TABLE IF NOT EXISTS `userinfo` (
	ui_uid int NOT NULL AUTO_INCREMENT,
	ui_uname varchar(32) NOT NULL,
	ui_passwd varchar(32),
	ui_nickname varchar(32),
	ui_picpath varchar(256),
	PRIMARY KEY(ui_uid, ui_uname)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

insert into userinfo values (1, 'test', '123', 'test', '/www/pic/test.jpg');
