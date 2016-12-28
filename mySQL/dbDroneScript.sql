-- MySQL Workbench Forward Engineering

SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL,ALLOW_INVALID_DATES';

-- -----------------------------------------------------
-- Schema mydb
-- -----------------------------------------------------
-- -----------------------------------------------------
-- Schema dbdrone
-- -----------------------------------------------------

-- -----------------------------------------------------
-- Schema dbdrone
-- -----------------------------------------------------
CREATE SCHEMA IF NOT EXISTS `dbdrone` DEFAULT CHARACTER SET utf8 ;
USE `dbdrone` ;

-- -----------------------------------------------------
-- Table `dbdrone`.`Drone`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `dbdrone`.`Drone` ;

CREATE TABLE IF NOT EXISTS `dbdrone`.`Drone` (
  `idDrone` INT(11) NOT NULL AUTO_INCREMENT,
  `name` VARCHAR(45) NULL DEFAULT NULL,
  `active` TINYINT(1) NULL DEFAULT NULL,
  PRIMARY KEY (`idDrone`))
ENGINE = InnoDB
AUTO_INCREMENT = 3
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `dbdrone`.`Park`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `dbdrone`.`Park` ;

CREATE TABLE IF NOT EXISTS `dbdrone`.`Park` (
  `idPark` INT(11) NOT NULL AUTO_INCREMENT,
  `name` VARCHAR(45) NULL DEFAULT NULL,
  `nLots` INT(11) NULL DEFAULT NULL,
  `centraLatitude` VARCHAR(45) NULL DEFAULT NULL,
  `centraLongitude` VARCHAR(45) NULL DEFAULT NULL,
  `active` TINYINT(1) NULL DEFAULT NULL COMMENT '																						DD',
  `type` VARCHAR(45) NULL DEFAULT NULL,
  `parkImg` VARCHAR(45) NULL DEFAULT NULL,
  PRIMARY KEY (`idPark`))
ENGINE = InnoDB
AUTO_INCREMENT = 3
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `dbdrone`.`Capture`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `dbdrone`.`Capture` ;

CREATE TABLE IF NOT EXISTS `dbdrone`.`Capture` (
  `idCapture` INT(11) NOT NULL AUTO_INCREMENT,
  `Park_idPark` INT(11) NOT NULL,
  `Drone_idDrone` INT(11) NOT NULL,
  `startedTime` DATETIME NOT NULL,
  `nOccupiedLots` INT(11) NULL DEFAULT NULL,
  `valid` TINYINT(1) NULL DEFAULT NULL,
  PRIMARY KEY (`idCapture`),
  INDEX `fk_Task_ParkingLot1_idx` (`Park_idPark` ASC),
  INDEX `fk_Capture_Drone1_idx` (`Drone_idDrone` ASC),
  CONSTRAINT `fk_Capture_Drone1`
    FOREIGN KEY (`Drone_idDrone`)
    REFERENCES `dbdrone`.`Drone` (`idDrone`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Task_ParkingLot1`
    FOREIGN KEY (`Park_idPark`)
    REFERENCES `dbdrone`.`Park` (`idPark`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
AUTO_INCREMENT = 11
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `dbdrone`.`GPScoor`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `dbdrone`.`GPScoor` ;

CREATE TABLE IF NOT EXISTS `dbdrone`.`GPScoor` (
  `idGPSCoor` INT(11) NOT NULL AUTO_INCREMENT,
  `Park_idPark` INT(11) NOT NULL,
  `index` INT(11) NULL DEFAULT NULL,
  `latitude` VARCHAR(45) NULL DEFAULT NULL,
  `longitude` VARCHAR(45) NULL DEFAULT NULL,
  `indexItem` INT(11) NULL DEFAULT NULL,
  PRIMARY KEY (`idGPSCoor`),
  INDEX `fk_GPSCoor_ParkingLot1_idx` (`Park_idPark` ASC),
  CONSTRAINT `fk_GPSCoor_ParkingLot1`
    FOREIGN KEY (`Park_idPark`)
    REFERENCES `dbdrone`.`Park` (`idPark`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
AUTO_INCREMENT = 5
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `dbdrone`.`Logger`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `dbdrone`.`Logger` ;

CREATE TABLE IF NOT EXISTS `dbdrone`.`Logger` (
  `idLogger` INT(11) NOT NULL AUTO_INCREMENT,
  `state` VARCHAR(245) NULL DEFAULT NULL,
  `Park_idPark` INT(11) NULL DEFAULT NULL,
  `Drone_idDrone` INT(11) NULL DEFAULT NULL,
  `time` DATETIME NULL DEFAULT NULL,
  `idFlow` INT(11) NULL DEFAULT NULL,
  PRIMARY KEY (`idLogger`),
  INDEX `fk_logger_park_idx` (`Park_idPark` ASC),
  INDEX `fk_logger1_park_idx` (`Park_idPark` ASC),
  INDEX `fk_logger1_Drone_idx` (`Drone_idDrone` ASC),
  CONSTRAINT `fk_logger_Drone1`
    FOREIGN KEY (`Drone_idDrone`)
    REFERENCES `dbdrone`.`Drone` (`idDrone`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_logger_park2`
    FOREIGN KEY (`Park_idPark`)
    REFERENCES `dbdrone`.`Park` (`idPark`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `dbdrone`.`Photo`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `dbdrone`.`Photo` ;

CREATE TABLE IF NOT EXISTS `dbdrone`.`Photo` (
  `idPhoto` INT(11) NOT NULL AUTO_INCREMENT,
  `Capture_idCapture` INT(11) NOT NULL,
  `src` VARCHAR(100) NULL DEFAULT NULL,
  `latitude` VARCHAR(45) NULL DEFAULT NULL,
  `longitude` VARCHAR(45) NULL DEFAULT NULL,
  `heigth` FLOAT NULL DEFAULT NULL,
  PRIMARY KEY (`idPhoto`),
  INDEX `fk_Photo_Task1_idx` (`Capture_idCapture` ASC),
  CONSTRAINT `fk_Photo_Task1`
    FOREIGN KEY (`Capture_idCapture`)
    REFERENCES `dbdrone`.`Capture` (`idCapture`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
AUTO_INCREMENT = 10
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `dbdrone`.`User`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `dbdrone`.`User` ;

CREATE TABLE IF NOT EXISTS `dbdrone`.`User` (
  `idUser` INT NOT NULL,
  `email` VARCHAR(45) NULL,
  `password` VARCHAR(45) NULL,
  `isAdmin` TINYINT(1) NULL,
  PRIMARY KEY (`idUser`))
ENGINE = InnoDB;


SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
