DROP DATABASE equip;
create database equip;
USE equip;

-- ----------------------------
-- Table structure for `UserInfo` 
-- ----------------------------
DROP TABLE IF EXISTS `UserInfo`;
CREATE TABLE `UserInfo` (
  `Id` varchar(50) NOT NULL ,
  `Name` varchar(30) NOT NULL,
  `IsStudent` bool NOT NULL,
  `PhoneNumber` varchar(30),
  `JobNumber` varchar(50) NOT NULL,
  PRIMARY KEY (`Id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for `History` 
-- ----------------------------
DROP TABLE IF EXISTS `History`;
CREATE TABLE `History` (
  `RecordId` int(5) NOT NULL AUTO_INCREMENT,
  `UserId` varchar(50) NOT NULL,
  `DeviceId` varchar(50) NOT NULL,
  `Date` date NOT NULL,
  PRIMARY KEY (`RecordId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for `DeviceInfo` 
-- ----------------------------
DROP TABLE IF EXISTS `DeviceInfo`;
CREATE TABLE `DeviceInfo` (
  `Id` varchar(50) NOT NULL ,
  `DeviceName` varchar(50) NOT NULL,
  `LocationId` varchar(50) NOT NULL,
  `DeviceInfo` Text,
  PRIMARY KEY (`Id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for `LocationInfo` 
-- ----------------------------
DROP TABLE IF EXISTS `LocationInfo`;
CREATE TABLE `LocationInfo` (
  `Id` varchar(50) NOT NULL ,
  `LocationInfo` Text,
  `LocationName` varchar(50),
  PRIMARY KEY (`Id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


insert into `UserInfo` values ('A001','Sky','1','58598901','S0001');
insert into `UserInfo` values ('A002','Earth','1','58598902','S0002');
insert into `UserInfo` values ('A003','Moon','1','58598903','S0003');
insert into `UserInfo` values ('A004','River','0','58598904','T0004');
insert into `UserInfo` values ('A005','Moutain','1','58598905','S0005');
insert into `UserInfo` values ('A006','Stone','0','58598906','T0006');
insert into `UserInfo` values ('A007','Mood','1','58598907','S0007');

insert into `DeviceInfo` values('DI001', 'Bubble', 'LC001','simple');
insert into `DeviceInfo` values('DI002', 'TeleVison', 'LC002','simple');
insert into `DeviceInfo` values('DI003', 'Watch', 'LC003','simple');
insert into `DeviceInfo` values('DI004', 'BroadCast', 'LC004','simple');

insert into `History` values('1', 'A001', 'DI001', '2014-03-19');
insert into `History` values('2', 'A002', 'DI001', '2014-06-19');
insert into `History` values('3', 'A003', 'DI001', '2014-03-19');
insert into `History` values('4', 'A004', 'DI001', '2014-04-19');
insert into `History` values('5', 'A003', 'DI002', '2014-07-19');
insert into `History` values('6', 'A004', 'DI002', '2014-03-19');
insert into `History` values('7', 'A004', 'DI003', '2014-08-19');
insert into `History` values('8', 'A005', 'DI003', '2014-04-19');
insert into `History` values('9', 'A007', 'DI004', '2014-09-19');
insert into `History` values('10', 'A003', 'DI003', '2014-08-19');
insert into `History` values('11', 'A007', 'DI004', '2014-09-19');
insert into `History` values('12', 'A003', 'DI002', '2014-09-19');
insert into `History` values('13', 'A004', 'DI002', '2014-03-19');
insert into `History` values('14', 'A002', 'DI004', '2014-04-19');
insert into `History` values('15', 'A007', 'DI001', '2014-07-19');
