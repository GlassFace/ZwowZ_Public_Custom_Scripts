SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for Account_Predator
-- ----------------------------
DROP TABLE IF EXISTS `Account_Predator`;
CREATE TABLE `Account_Predator` (
  `Id` int(11) NOT NULL,
  `CharacterName` varchar(200) DEFAULT NULL,
  `Predator` int(11) DEFAULT NULL,
  `Warnings` int(11) DEFAULT NULL,
  `Comment` varchar(200) DEFAULT NULL,
  PRIMARY KEY (`Id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
