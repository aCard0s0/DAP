/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package server.controller;

import java.util.Collection;
import java.util.Optional;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.MediaType;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.CrossOrigin;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;
import server.domain.Capture;
import server.domain.Park;
import server.service.ParkService;

/**
 *
 * @author acardoso
 */
@RestController
public class ParkController {
    
    @Autowired
    private ParkService serviceBean;       // Bean that execute the Service
    
    
    @RequestMapping(
            value = "/api/park", 
            method = RequestMethod.POST,
            consumes = MediaType.APPLICATION_JSON_VALUE,
            produces = MediaType.APPLICATION_JSON_VALUE)
    public ResponseEntity< Park > createObj(@RequestBody Park park) {
        
        return new ResponseEntity<>(serviceBean.create(park), HttpStatus.CREATED);
    }
    
    @RequestMapping( 
            value = "/api/park", 
            method = RequestMethod.GET,
            produces = MediaType.APPLICATION_JSON_VALUE )
    public ResponseEntity< Collection< Park> > getAll() {
        
        return new ResponseEntity<>( serviceBean.findAll(), HttpStatus.OK);
    }
    
    @RequestMapping(
            value = "/api/park/{id}", 
            method = RequestMethod.GET,
            produces = MediaType.APPLICATION_JSON_VALUE)
    public ResponseEntity< Park > getOne(@PathVariable("id") Integer id) {
        
        Park park = serviceBean.findOne( id );
        if (park == null) {
            return new ResponseEntity<>(HttpStatus.NOT_FOUND);
        }
        return new ResponseEntity<  >(park, HttpStatus.OK);
    }
    
    @RequestMapping(
            value = "/api/park/{idPark}/last", 
            method = RequestMethod.GET,
            produces = MediaType.APPLICATION_JSON_VALUE)
    public ResponseEntity< Capture > getLastCaptureByPark (@PathVariable("idPark") Integer idPark) {
        
        Park park = serviceBean.findOne( idPark );
        int count = park.getCaptureCollection().size();
        
        if (count == 0) {
            return new ResponseEntity<>(HttpStatus.NOT_FOUND);
        }
        
        Optional<Capture> capture =  park.getCaptureCollection().stream().skip( count-1 ).findFirst();
        
        if (!capture.isPresent()) {
            return new ResponseEntity<>(HttpStatus.NOT_FOUND);
        }
        
        return new ResponseEntity<  >( capture.get() , HttpStatus.OK);
    }
    
    @CrossOrigin
    @RequestMapping(
            value = "/api/park/{id}", 
            method = RequestMethod.PUT,
            consumes = MediaType.APPLICATION_JSON_VALUE,
            produces = MediaType.APPLICATION_JSON_VALUE)
    public ResponseEntity< Park > updateObj(@RequestBody Park park) {
        
        Park newPark = serviceBean.update(park);
        if (newPark == null)
            return new ResponseEntity<>(HttpStatus.INTERNAL_SERVER_ERROR);

        return new ResponseEntity<>(newPark, HttpStatus.OK);
    }
    
    @CrossOrigin  
    @RequestMapping(
            value = "/api/park/{idPark}", 
            method = RequestMethod.DELETE,
            consumes = MediaType.APPLICATION_JSON_VALUE )
    public ResponseEntity< Park > deleteObj(@PathVariable("idPark") Integer idPark) {
        
        if (serviceBean.delete( idPark ) == null )     // update ao status
            return new ResponseEntity<>(HttpStatus.NOT_FOUND);
        return new ResponseEntity<>(HttpStatus.OK);
    }
}
