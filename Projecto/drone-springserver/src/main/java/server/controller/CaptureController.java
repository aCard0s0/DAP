/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package server.controller;

import java.util.Collection;
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
import server.domain.Park;
import server.service.CaptureService;
import server.service.DroneService;
import server.service.ParkService;

/**
 *
 * @author acardoso
 */
@RestController
public class CaptureController {
    
    @Autowired
    private CaptureService serviceBean;       // Bean that execute the Service
    @Autowired
    private ParkService parkBean;
    @Autowired
    private DroneService droneBean;
    
    
    @RequestMapping(
            value = "/api/capture/park/{idPark}/drone/{idDrone}", 
            method = RequestMethod.POST,
            consumes = MediaType.APPLICATION_JSON_VALUE,
            produces = MediaType.APPLICATION_JSON_VALUE)
    public ResponseEntity< Capture > createObj(
            @RequestBody Capture capture,       // object body to insert
            @PathVariable("idPark") Integer idPark,
            @PathVariable("idDrone") Integer idDrone) {   // id  of Park that this capture is assosiate with.
        
        capture.setParkidPark( parkBean.findOne( idPark ) );
        capture.setDroneidDrone( droneBean.findOne( idDrone ) );
        
        return new ResponseEntity<>(serviceBean.create(capture), HttpStatus.CREATED);
    }
    
    @RequestMapping( 
            value = "/api/capture", 
            method = RequestMethod.GET,
            produces = MediaType.APPLICATION_JSON_VALUE )
    public ResponseEntity< Collection< Capture> > getAll() {
        
        return new ResponseEntity<>( serviceBean.findAll(), HttpStatus.OK);
    }
    
    @RequestMapping(
            value = "/api/capture/{id}", 
            method = RequestMethod.GET,
            produces = MediaType.APPLICATION_JSON_VALUE)
    public ResponseEntity< Capture > getOne(@PathVariable("id") Integer id) {
        
        Capture capture = serviceBean.findOne( id );
        if (capture == null) {
            return new ResponseEntity<>(HttpStatus.NOT_FOUND);
        }
        return new ResponseEntity<  >(capture, HttpStatus.OK);
    }
    
    @RequestMapping(
            value = "/api/capture/park/{idPark}", 
            method = RequestMethod.GET,
            produces = MediaType.APPLICATION_JSON_VALUE)
    public ResponseEntity< Collection< Capture > > getCaptureByPark(@PathVariable("idPark") Integer idPark) {
        
        Park park = parkBean.findOne(idPark);
        
        if (park == null) {
            return new ResponseEntity<>(HttpStatus.NOT_FOUND);
        }
        return new ResponseEntity<  >( park.getCaptureCollection() , HttpStatus.OK);
    }
    
    @RequestMapping(
            value = "/api/capture/{id}", 
            method = RequestMethod.PUT,
            consumes = MediaType.APPLICATION_JSON_VALUE,
            produces = MediaType.APPLICATION_JSON_VALUE)
    public ResponseEntity< Capture > updateObj(@RequestBody Capture capture) {
        
        Capture newCapture = serviceBean.update(capture);
        if (newCapture == null)
            return new ResponseEntity<>(HttpStatus.INTERNAL_SERVER_ERROR);

        return new ResponseEntity<>(newCapture, HttpStatus.OK);
    }
    
    @RequestMapping(
            value = "/api/capture/{id}", 
            method = RequestMethod.DELETE,
            consumes = MediaType.APPLICATION_JSON_VALUE )
    public ResponseEntity< Capture > deleteObj(@PathVariable("id") Integer id) {
        
        if (serviceBean.delete(id) == null )     // update ao status
            return new ResponseEntity<>(HttpStatus.NOT_FOUND);
        return new ResponseEntity<>(HttpStatus.OK);
    }
    
}
