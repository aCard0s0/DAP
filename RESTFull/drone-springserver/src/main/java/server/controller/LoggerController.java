/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package server.controller;

import java.util.Collection;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;
import java.util.Optional;
import java.util.stream.Collectors;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.MediaType;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;
import server.domain.Capture;
import server.domain.Logger;
import server.domain.Park;
import server.service.DroneService;
import server.service.LoggerService;
import server.service.ParkService;

/**
 *
 * @author acardoso
 */
@RestController
public class LoggerController {
    
    @Autowired
    private LoggerService loggerBean;       // Bean that execute the Service
    @Autowired
    private ParkService parkBean;
    @Autowired
    private DroneService droneBean;
    
    @RequestMapping(
            value = "/api/logger/park/{idPark}/drone/{idDrone}", 
            method = RequestMethod.POST,
            consumes = MediaType.APPLICATION_JSON_VALUE,
            produces = MediaType.APPLICATION_JSON_VALUE)
    public ResponseEntity< Logger > createObj(
            @RequestBody Logger logger,       // object body to insert
            @PathVariable("idPark") Integer idPark,
            @PathVariable("idDrone") Integer idDrone) {   // id  of Park that this capture is assosiate with.
        
        logger.setParkidPark( parkBean.findOne( idPark ) );
        logger.setDroneidDrone( droneBean.findOne( idDrone ) );
    
        return new ResponseEntity<>(loggerBean.create(logger), HttpStatus.CREATED);
    }
    
    @RequestMapping(
            value = "/api/logger/last/{number}", 
            method = RequestMethod.GET,
            produces = MediaType.APPLICATION_JSON_VALUE)
    public ResponseEntity< Collection< Logger> > getLastResults (@PathVariable("number") Integer number) {
        
      
        int count = loggerBean.findAll().size();
        
        if (count == 0) {
            return new ResponseEntity<>(HttpStatus.NOT_FOUND);
        }
        
        List<Logger> resultsTemp =  (List<Logger>) loggerBean.findAll();
        Collections.sort(resultsTemp, (Logger o1, Logger o2) -> o2.getIdLogger()- o1.getIdLogger());
        List<Logger> results =  resultsTemp.stream().limit(number).collect(Collectors.toList());
        
        return new ResponseEntity<  >( results , HttpStatus.OK);
    }
    
    @RequestMapping( 
            value = "/api/logger", 
            method = RequestMethod.GET,
            produces = MediaType.APPLICATION_JSON_VALUE )
    public ResponseEntity< Collection< Logger> > getAll() {
        
        return new ResponseEntity<>( loggerBean.findAll(), HttpStatus.OK);
    }
    
    
    
  
    
}
