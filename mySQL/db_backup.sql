-- MySQL dump 10.13  Distrib 5.5.47, for debian-linux-gnu (x86_64)
--
-- Host: localhost    Database: dbdrone
-- ------------------------------------------------------
-- Server version	5.5.47-0ubuntu0.14.04.1

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `capture`
--

DROP TABLE IF EXISTS `capture`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `capture` (
  `idCapture` int(11) NOT NULL AUTO_INCREMENT,
  `Park_idPark` int(11) NOT NULL,
  `Drone_idDrone` int(11) NOT NULL,
  `startedTime` datetime NOT NULL,
  `finishTime` datetime DEFAULT NULL,
  `nOccupiedLots` int(11) DEFAULT NULL,
  `valid` tinyint(1) DEFAULT NULL,
  PRIMARY KEY (`idCapture`),
  KEY `fk_Task_ParkingLot1_idx` (`Park_idPark`),
  KEY `fk_Capture_Drone1_idx` (`Drone_idDrone`),
  CONSTRAINT `fk_Capture_Drone1` FOREIGN KEY (`Drone_idDrone`) REFERENCES `drone` (`idDrone`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_Task_ParkingLot1` FOREIGN KEY (`Park_idPark`) REFERENCES `park` (`idPark`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=23 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `capture`
--

LOCK TABLES `capture` WRITE;
/*!40000 ALTER TABLE `capture` DISABLE KEYS */;
INSERT INTO `capture` VALUES (1,9,4,'2015-12-12 00:00:00',NULL,NULL,NULL),
(2,9,4,'2015-12-12 00:00:00',NULL,NULL,NULL),
(3,9,4,'2015-12-12 00:00:00',NULL,NULL,NULL),
(4,9,4,'2015-12-02 00:00:00',NULL,NULL,NULL),
(5,9,4,'2015-12-02 00:00:00',NULL,NULL,NULL),
(6,9,4,'2015-12-02 00:00:00',NULL,NULL,NULL),
(7,9,4,'2015-12-02 00:00:00',NULL,NULL,NULL),
(8,9,4,'2015-12-02 00:00:00',NULL,NULL,NULL),
(9,9,4,'2015-12-02 00:00:00',NULL,NULL,NULL),
(10,9,4,'2015-12-02 00:00:00',NULL,NULL,NULL),
(11,9,4,'2015-12-02 00:00:00',NULL,NULL,NULL),
(12,9,4,'2015-12-02 00:00:00',NULL,NULL,NULL),
(13,9,4,'2015-12-02 00:00:00',NULL,NULL,NULL),
(14,9,4,'2015-12-02 00:00:00',NULL,NULL,NULL),
(15,9,4,'2015-12-02 00:00:00',NULL,NULL,NULL),
(16,9,4,'2015-12-02 00:00:00',NULL,NULL,NULL),
(17,9,4,'2015-12-02 00:00:00',NULL,NULL,NULL),
(18,9,4,'2015-12-02 00:00:00',NULL,NULL,NULL),
(19,9,4,'2015-12-02 00:00:00',NULL,NULL,NULL),
(20,9,4,'2015-12-02 00:00:00',NULL,NULL,NULL),
(21,9,4,'2015-12-02 00:00:00',NULL,NULL,NULL),
(22,9,4,'2015-12-02 00:00:00',NULL,NULL,NULL);
/*!40000 ALTER TABLE `capture` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `drone`
--

DROP TABLE IF EXISTS `drone`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `drone` (
  `idDrone` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(45) DEFAULT NULL,
  `active` tinyint(1) DEFAULT NULL,
  PRIMARY KEY (`idDrone`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `drone`
--

LOCK TABLES `drone` WRITE;
/*!40000 ALTER TABLE `drone` DISABLE KEYS */;
INSERT INTO `drone` VALUES ('bebop 2 ieeta',1);
/*!40000 ALTER TABLE `drone` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `gpscoor`
--

DROP TABLE IF EXISTS `gpscoor`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `gpscoor` (
  `idGPSCoor` int(11) NOT NULL AUTO_INCREMENT,
  `Park_idPark` int(11) NOT NULL,
  `indexItem` int(11) DEFAULT NULL,
  `latitude` varchar(45) DEFAULT NULL,
  `longitude` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`idGPSCoor`),
  KEY `fk_GPSCoor_ParkingLot1_idx` (`Park_idPark`),
  CONSTRAINT `fk_GPSCoor_ParkingLot1` FOREIGN KEY (`Park_idPark`) REFERENCES `park` (`idPark`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=95 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `gpscoor`
--

LOCK TABLES `gpscoor` WRITE;
/*!40000 ALTER TABLE `gpscoor` DISABLE KEYS */;
INSERT INTO `gpscoor` VALUES (10,0,'40.62845603750988',' -8.655896186828613'),(10,1,'40.62835832589364',' -8.6557137966156'),(11,10,2,'40.62831761267801',' -8.655552864074707'),(12,10,3,'40.62831761267801',' -8.655338287353516'),(13,10,4,'40.6283501832525',' -8.655134439468384'),(14,10,5,'40.62826875678649',' -8.655102252960205'),(15,10,6,'40.6282117582012',' -8.654812574386597'),(16,10,7,'40.628285042087626',' -8.654758930206299'),(17,10,8,'40.628578176829166',' -8.654866218566895'),(18,10,9,'40.62892830830633',' -8.65472674369812'),(19,10,10,'40.62911558718009',' -8.654522895812988'),(20,10,11,'40.629262152888835',' -8.654329776763916'),(21,10,13,'40.62950642835527',' -8.654544353485107'),(22,10,14,'40.6296285657534',' -8.65473747253418'),(23,10,15,'40.62949014335196',' -8.654823303222656'),(24,10,16,'40.6292865804757',' -8.654866218566895'),(25,10,17,'40.629156299909226',' -8.654941320419312'),(26,10,18,'40.628960878582966',' -8.655198812484741'),(27,10,20,'40.628651460313485',' -8.655595779418945'),(28,10,19,'40.62875731409333',' -8.655381202697754'),(29,10,12,'40.62927843794773',' -8.654254674911499'),(30,11,0,'40.622918820432126',' -8.65897536277771'),(31,11,1,'40.62329341135873',' -8.657784461975098'),(32,11,4,'40.62313054599661',' -8.656196594238281'),(33,11,3,'40.62333412763721',' -8.656325340270996'),(34,11,2,'40.622902533822455',' -8.65746259689331'),(35,11,5,'40.62223477940632',' -8.658396005630493'),(36,11,6,'40.62289439051612',' -8.65896463394165'),(37,12,0,'40.63029624624795',' -8.652178645133972'),(38,12,1,'40.630284032640326',' -8.65247905254364'),(39,12,3,'40.63117969127585',' -8.652135729789734'),(40,12,2,'40.631012773986406',' -8.652586340904236'),(41,12,6,'40.63027589023398',' -8.652184009552002'),(42,12,5,'40.630967991228005',' -8.652264475822449'),(43,12,4,'40.63104941440276',' -8.652092814445496'),(44,13,1,'40.63015578962521',' -8.65221619606018'),(45,13,2,'40.63010897068533',' -8.652956485748291'),(46,13,4,'40.63056290945796',' -8.65296721458435'),(47,13,0,'40.63029624624795',' -8.652234971523285'),(48,13,6,'40.630231106981405',' -8.652471005916595'),(49,13,8,'40.63028606824176',' -8.652505874633789'),(50,13,5,'40.6302026085323',' -8.652774095535278'),(51,13,9,'40.63029828184903',' -8.652245700359344'),(52,13,7,'40.630231106981405',' -8.652471005916595'),(53,13,3,'40.630556802678804',' -8.653074502944946'),(54,14,2,'40.632339958464435',' -8.656598925590515'),(55,14,4,'40.63215675972237',' -8.656142950057983'),(56,14,5,'40.63200612926902',' -8.65598738193512'),(57,14,3,'40.63228296335409',' -8.656427264213562'),(58,14,0,'40.63181885850184',' -8.65699589252472'),(59,14,1,'40.6321119777312',' -8.656792044639587'),(60,14,6,'40.631713009575016',' -8.655837178230286'),(61,14,7,'40.63168451175836',' -8.65571916103363'),(62,14,9,'40.63141174632644',' -8.655531406402588'),(63,14,11,'40.63124890076129',' -8.655633330345154'),(64,14,8,'40.63156644924551',' -8.655590415000916'),(65,14,10,'40.63136696383562',' -8.655660152435303'),(66,14,12,'40.63115119323158',' -8.65546703338623'),(67,14,13,'40.63097613354994',' -8.65524709224701'),(68,14,16,'40.63065858225847',' -8.654925227165222'),(69,14,17,'40.63062601280985',' -8.654801845550537'),(70,14,19,'40.63044688055852',' -8.654522895812988'),(71,14,15,'40.630760361683',' -8.65496814250946'),(72,14,18,'40.63052016199215',' -8.654683828353882'),(73,14,14,'40.63089063912002',' -8.655118346214294'),(74,14,20,'40.63032474465711',' -8.654394149780273'),(75,14,22,'40.63013339796241',' -8.65471601486206'),(76,14,21,'40.630096757043425',' -8.654603362083435'),(77,14,23,'40.63024332059873',' -8.654839396476746'),(78,14,28,'40.63181885850184',' -8.656994551420212'),(79,14,25,'40.63141174632644',' -8.656454086303711'),(80,14,24,'40.630768504030264',' -8.655531406402588'),(81,14,26,'40.631505382346575',' -8.656647205352783'),(82,14,27,'40.63176593405942',' -8.6569744348526'),(83,14,29,'40.631821911833796',' -8.656991869211197'),(84,14,31,'40.631813769614986',' -8.656997233629227'),(85,14,30,'40.63181580516978',' -8.65699589252472'),(86,9,0,'40.63093867881109',' -8.66098165512085'),(87,9,1,'40.63125622877055',' -8.660573959350586'),(88,9,2,'40.63025471992319',' -8.659061193466187'),(89,9,3,'40.629880168039776',' -8.659576177597046'),(90,9,4,'40.63090610949911',' -8.66099238395691'),(91,9,5,'40.630957609753665',' -8.660944104194641'),(92,9,6,'40.63101257041604',' -8.660906553268433'),(93,9,7,'40.630994250200295',' -8.660909235477448'),(94,9,8,'40.63090610949911',' -8.66099238395691');
/*!40000 ALTER TABLE `gpscoor` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `logger`
--

DROP TABLE IF EXISTS `logger`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `logger` (
  `idLogger` int(11) NOT NULL AUTO_INCREMENT,
  `state` varchar(245) DEFAULT NULL,
  `Park_idPark` int(11) DEFAULT NULL,
  `Drone_idDrone` int(11) DEFAULT NULL,
  `time` datetime DEFAULT NULL,
  `idFlow` int(11) DEFAULT NULL,
  PRIMARY KEY (`idLogger`),
  KEY `fk_logger_park_idx` (`Park_idPark`),
  KEY `fk_logger1_park_idx` (`Park_idPark`),
  KEY `fk_logger1_Drone_idx` (`Drone_idDrone`),
  CONSTRAINT `fk_logger_Drone1` FOREIGN KEY (`Drone_idDrone`) REFERENCES `drone` (`idDrone`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_logger_park2` FOREIGN KEY (`Park_idPark`) REFERENCES `park` (`idPark`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=92 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `logger`
--

LOCK TABLES `logger` WRITE;
/*!40000 ALTER TABLE `logger` DISABLE KEYS */;
INSERT INTO `logger` VALUES (1,'teste post',NULL,NULL,'2012-10-01 01:00:00',1),(2,'teste post 22',9,NULL,'2012-10-01 01:00:00',1),(3,'teste post 3333',9,NULL,'2012-10-01 01:00:00',1),(4,'teste post 444',9,NULL,'2012-10-01 01:00:00',1),(5,'executando',11,NULL,'2016-05-07 01:00:00',1),(6,'nao foi possivel executar o mavlink',11,NULL,'2016-05-07 01:00:00',1),(7,'executando',14,4,'2016-05-07 01:00:00',1),(8,'nao foi possivel executar o mavlink',14,4,'2016-05-07 01:00:00',1),(9,'executando',11,4,'2016-05-10 01:00:00',1),(10,'nao foi possivel executar o mavlink',11,4,'2016-05-10 01:00:00',1),(11,'executando',14,4,'2016-05-10 01:00:00',1),(12,'nao foi possivel executar o mavlink',14,4,'2016-05-10 01:00:00',1),(13,'executando',10,4,'2016-05-10 01:00:00',1),(14,'nao foi possivel executar o mavlink',10,4,'2016-05-10 01:00:00',1),(15,'executando',9,4,'2016-05-10 01:00:00',1),(16,'nao foi possivel executar o mavlink',9,4,'2016-05-10 01:00:00',1),(17,'executando',9,4,'2016-05-10 01:00:00',1),(18,'nao foi possivel executar o mavlink',9,4,'2016-05-10 01:00:00',1),(19,'executando',9,4,'2016-05-10 01:00:00',1),(20,'nao foi possivel executar o mavlink',9,4,'2016-05-10 01:00:00',1),(21,'executando',9,4,'2016-05-10 01:00:00',1),(22,'nao foi possivel executar o mavlink',9,4,'2016-05-10 01:00:00',1),(23,'executando',9,4,'2016-05-10 01:00:00',1),(24,'nao foi possivel executar o mavlink',9,4,'2016-05-10 01:00:00',1),(25,'executando',NULL,NULL,'2016-05-10 01:00:00',1),(26,'nao foi possivel executar o mavlink',NULL,NULL,'2016-05-10 01:00:00',1),(27,'executando',NULL,NULL,'2016-05-10 01:00:00',1),(28,'nao foi possivel executar o mavlink',NULL,NULL,'2016-05-10 01:00:00',1),(29,'executando',NULL,NULL,'2016-05-10 01:00:00',1),(30,'nao foi possivel executar o mavlink',NULL,NULL,'2016-05-10 01:00:00',1),(31,'executando',NULL,NULL,'2016-05-10 01:00:00',1),(32,'nao foi possivel executar o mavlink',NULL,NULL,'2016-05-10 01:00:00',1),(33,'executando',NULL,NULL,'2016-05-10 01:00:00',1),(34,'nao foi possivel executar o mavlink',NULL,NULL,'2016-05-10 01:00:00',1),(35,'executando',NULL,NULL,'2016-05-10 01:00:00',1),(36,'nao foi possivel executar o mavlink',NULL,NULL,'2016-05-10 01:00:00',1),(37,'executando',NULL,NULL,'2016-05-10 01:00:00',1),(38,'nao foi possivel executar o mavlink',NULL,NULL,'2016-05-10 01:00:00',1),(39,'executando',NULL,NULL,'2016-05-10 01:00:00',1),(40,'nao foi possivel executar o mavlink',NULL,NULL,'2016-05-10 01:00:00',1),(41,'executando',NULL,NULL,'2016-05-10 01:00:00',1),(42,'nao foi possivel executar o mavlink',NULL,NULL,'2016-05-10 01:00:00',1),(43,'executando',NULL,NULL,'2016-05-10 01:00:00',1),(44,'nao foi possivel executar o mavlink',NULL,NULL,'2016-05-10 01:00:00',1),(45,'executando',NULL,NULL,'2016-05-10 01:00:00',1),(46,'nao foi possivel executar o mavlink',NULL,NULL,'2016-05-10 01:00:00',1),(47,'executando',NULL,NULL,'2016-05-10 01:00:00',1),(48,'nao foi possivel executar o mavlink',NULL,NULL,'2016-05-10 01:00:00',1),(49,'executando',NULL,NULL,'2016-05-10 01:00:00',1),(50,'nao foi possivel executar o mavlink',NULL,NULL,'2016-05-10 01:00:00',1),(51,'executando',NULL,NULL,'2016-05-10 01:00:00',1),(52,'nao foi possivel executar o mavlink',NULL,NULL,'2016-05-10 01:00:00',1),(53,'executando',NULL,NULL,'2016-05-10 01:00:00',1),(54,'nao foi possivel executar o mavlink',NULL,NULL,'2016-05-10 01:00:00',1),(55,'executando',NULL,NULL,'2016-05-10 01:00:00',1),(56,'nao foi possivel executar o mavlink',NULL,NULL,'2016-05-10 01:00:00',1),(57,'executando',NULL,NULL,'2016-05-10 01:00:00',1),(58,'nao foi possivel executar o mavlink',NULL,NULL,'2016-05-10 01:00:00',1),(59,'executando',10,4,'2016-05-10 01:00:00',1),(60,'executando',NULL,NULL,'2016-05-10 01:00:00',1),(61,'nao foi possivel executar o mavlink',NULL,NULL,'2016-05-10 01:00:00',1),(62,'nao foi possivel executar o mavlink',10,4,'2016-05-10 01:00:00',1),(63,'executando',14,4,'2016-05-10 01:00:00',1),(64,'executando',14,4,'2016-05-10 01:00:00',1),(65,'nao foi possivel executar o mavlink',14,4,'2016-05-10 01:00:00',1),(66,'nao foi possivel executar o mavlink',14,4,'2016-05-10 01:00:00',1),(67,'executando',11,4,'2016-05-10 01:00:00',1),(68,'nao foi possivel executar o mavlink',11,4,'2016-05-10 01:00:00',1),(69,'executando',10,4,'2016-05-10 01:00:00',1),(70,'nao foi possivel executar o mavlink',10,4,'2016-05-10 01:00:00',1),(71,'executando',11,4,'2016-05-10 01:00:00',1),(72,'nao foi possivel executar o mavlink',11,4,'2016-05-10 01:00:00',1),(73,'executando',9,4,'2016-05-10 01:00:00',1),(74,'executando',9,4,'2016-05-10 01:00:00',1),(75,'nao foi possivel executar o mavlink',9,4,'2016-05-10 01:00:00',1),(76,'executando',9,4,'2016-05-10 01:00:00',1),(77,'executando',12,4,'2016-05-10 01:00:00',1),(78,'executando',9,4,'2016-05-14 01:00:00',1),(79,'nao foi possivel executar o mavlink',9,4,'2016-05-14 01:00:00',1),(80,'executando',14,4,'2016-05-14 01:00:00',1),(81,'nao foi possivel executar o mavlink',14,4,'2016-05-14 01:00:00',1),(82,'executando',9,4,'2016-05-18 01:00:00',1),(83,'nao foi possivel executar o mavlink',9,4,'2016-05-18 01:00:00',1),(84,'executando',14,4,'2016-05-18 01:00:00',1),(85,'nao foi possivel executar o mavlink',14,4,'2016-05-18 01:00:00',1),(86,'executando',14,4,'2016-05-18 01:00:00',1),(87,'nao foi possivel executar o mavlink',14,4,'2016-05-18 01:00:00',1),(88,'executando',14,4,'2016-05-18 01:00:00',1),(89,'nao foi possivel executar o mavlink',14,4,'2016-05-18 01:00:00',1),(90,'executando',11,4,'2016-05-18 01:00:00',1),(91,'nao foi possivel executar o mavlink',11,4,'2016-05-18 01:00:00',1);
/*!40000 ALTER TABLE `logger` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `park`
--

DROP TABLE IF EXISTS `park`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `park` (
  `idPark` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(45) DEFAULT NULL,
  `nLots` int(11) DEFAULT NULL,
  `centraLatitude` varchar(45) DEFAULT NULL,
  `centraLongitude` varchar(45) DEFAULT NULL,
  `active` tinyint(1) DEFAULT NULL COMMENT '																						DD',
  `type` varchar(45) DEFAULT NULL,
  `parkImg` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`idPark`)
) ENGINE=InnoDB AUTO_INCREMENT=16 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `park`
--

LOCK TABLES `park` WRITE;
/*!40000 ALTER TABLE `park` DISABLE KEYS */;
INSERT INTO `park` VALUES ('Biblioteca',227,'40.630563','-8.659982',1,'Privado','biblioteca.png'),
('Complexo Pedagogico',150,'40.628835','-8.655053',1,'Publico','Parque_CP.png'),
(11,'ESSUA',350,'40.622905','-8.658261',1,'Publico','Parque_ESSUA.png'),
(12,'ISCAA Público',72,'40.630746','-8.652371',1,'Publico','Parque_ISCAA Público(P13).png'),
(13,'ISCAA Funcionários',34,'40.630175','-8.652590',1,'Privado','Parque_ISCAA_Funcionarios.png'),
(14,'Residências de Santiago',663,'40.631175','-8.655755',1,'Privado','Parque_Residencia_Santiago.png');
/*!40000 ALTER TABLE `park` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `photo`
--
-- Não precisa

DROP TABLE IF EXISTS `photo`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `photo` (
  `idPhoto` int(11) NOT NULL AUTO_INCREMENT,
  `Capture_idCapture` int(11) NOT NULL,
  `src` varchar(100) DEFAULT NULL,
  `latitude` varchar(45) DEFAULT NULL,
  `longitude` varchar(45) DEFAULT NULL,
  `heigth` float DEFAULT NULL,
  PRIMARY KEY (`idPhoto`),
  KEY `fk_Photo_Task1_idx` (`Capture_idCapture`),
  CONSTRAINT `fk_Photo_Task1` FOREIGN KEY (`Capture_idCapture`) REFERENCES `capture` (`idCapture`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `photo`
--

LOCK TABLES `photo` WRITE;
/*!40000 ALTER TABLE `photo` DISABLE KEYS */;
INSERT INTO `photo` VALUES (1,10,'images/10_bannerteste23332.jpg',NULL,NULL,NULL),(2,10,'images/10_bannerteste23332.jpg',NULL,NULL,NULL),(3,10,'public/imgparks/10_bannerteste23332.jpg',NULL,NULL,NULL);
/*!40000 ALTER TABLE `photo` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2016-05-24 22:17:39
