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
import server.domain.Drone;
import server.service.DroneService;
import java.io.IOException;
import java.util.concurrent.TimeoutException;
/**
 *
 * @author acardoso
 */
@RestController
public class DroneController {
    
    @Autowired
    private DroneService serviceBean;       // Bean that execute the Service
    
    @RequestMapping(
            value = "/api/drone", 
            method = RequestMethod.POST,
            consumes = MediaType.APPLICATION_JSON_VALUE,
            produces = MediaType.APPLICATION_JSON_VALUE)
    public ResponseEntity< Drone > createObj(@RequestBody Drone drone) {
        
        return new ResponseEntity<>(serviceBean.create(drone), HttpStatus.CREATED);
    }
    
    @RequestMapping(
            value = "/api/drone/{id}/park/{parkId}/send",
            method = RequestMethod.POST)
    public ResponseEntity<Integer> sendDrone(
            @PathVariable("id") Integer id, 
            @PathVariable("parkId") Integer parkId) {

        try {
            serviceBean.sendDrone(id, parkId);

        } catch (IOException | TimeoutException ex) {
            return new ResponseEntity<>(HttpStatus.FAILED_DEPENDENCY);
        }

        return new ResponseEntity<>(1, HttpStatus.CREATED);
    }
    
    @RequestMapping( 
            value = "/api/drone", 
            method = RequestMethod.GET,
            produces = MediaType.APPLICATION_JSON_VALUE )
    public ResponseEntity< Collection< Drone> > getAll() {
        
        return new ResponseEntity<>( serviceBean.findAll(), HttpStatus.OK);
    }
    
    @RequestMapping(
            value = "/api/drone/{id}", 
            method = RequestMethod.GET,
            produces = MediaType.APPLICATION_JSON_VALUE)
    public ResponseEntity< Drone > getOne(@PathVariable("id") Integer id) {
        
        Drone drone = serviceBean.findOne( id );
        if (drone == null) {
            return new ResponseEntity<>(HttpStatus.NOT_FOUND);
        }
        return new ResponseEntity<  >(drone, HttpStatus.OK);
    }
    
    @RequestMapping(
            value = "/api/drone/{id}", 
            method = RequestMethod.PUT,
            consumes = MediaType.APPLICATION_JSON_VALUE,
            produces = MediaType.APPLICATION_JSON_VALUE)
    public ResponseEntity< Drone > updateObj(@RequestBody Drone capture) {
        
        Drone newDrone = serviceBean.update(capture);
        if (newDrone == null)
            return new ResponseEntity<>(HttpStatus.INTERNAL_SERVER_ERROR);

        return new ResponseEntity<>(newDrone, HttpStatus.OK);
    }
    
    @RequestMapping(
            value = "/api/drone/{id}", 
            method = RequestMethod.DELETE,
            consumes = MediaType.APPLICATION_JSON_VALUE )
    public ResponseEntity< Drone > deleteObj(@PathVariable("id") Integer id, @RequestBody Drone drone) {
        
        serviceBean.delete(id);     // update active to false
        return new ResponseEntity<>(HttpStatus.OK);
    }
}
