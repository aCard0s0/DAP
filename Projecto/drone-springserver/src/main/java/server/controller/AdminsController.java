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
import server.domain.Admins;
import server.service.AdminsService;
/**
 *
 * @author acardoso
 */
@RestController
public class AdminsController {
    
    @Autowired
    private AdminsService serviceBean;       // Bean that execute the Service
    
    @RequestMapping(
            value = "/api/user", 
            method = RequestMethod.POST,
            consumes = MediaType.APPLICATION_JSON_VALUE,
            produces = MediaType.APPLICATION_JSON_VALUE)
    public ResponseEntity< Admins > createObj(@RequestBody Admins user) {
        
        return new ResponseEntity<>(serviceBean.create(user), HttpStatus.CREATED);
    }
    
    @RequestMapping( 
            value = "/api/user/login/{email}/{pass}", 
            method = RequestMethod.GET,
            produces = MediaType.APPLICATION_JSON_VALUE )
    public ResponseEntity< Boolean > login(
            @PathVariable("email") String email,
            @PathVariable("pass") String pass) {

        if ( serviceBean.login(email, pass) )
            return new ResponseEntity<>(Boolean.TRUE, HttpStatus.OK);

        return new ResponseEntity<>(Boolean.FALSE, HttpStatus.OK);
    }
    
    @RequestMapping( 
            value = "/api/user", 
            method = RequestMethod.GET,
            produces = MediaType.APPLICATION_JSON_VALUE )
    public ResponseEntity< Collection< Admins> > getAll() {
        
        return new ResponseEntity<>( serviceBean.findAll(), HttpStatus.OK);
    }
    
    @RequestMapping(
            value = "/api/user/{id}", 
            method = RequestMethod.GET,
            produces = MediaType.APPLICATION_JSON_VALUE)
    public ResponseEntity< Admins > getOne(@PathVariable("id") Integer id) {
        
        Admins user = serviceBean.findOne( id );
        if (user == null) {
            return new ResponseEntity<>(HttpStatus.NOT_FOUND);
        }
        return new ResponseEntity<  >(user, HttpStatus.OK);
    }
    
    @RequestMapping(
            value = "/api/user/{id}", 
            method = RequestMethod.PUT,
            consumes = MediaType.APPLICATION_JSON_VALUE,
            produces = MediaType.APPLICATION_JSON_VALUE)
    public ResponseEntity< Admins > updateObj(@RequestBody Admins user) {
        
        Admins newUser = serviceBean.update(user);
        if (newUser == null)
            return new ResponseEntity<>(HttpStatus.INTERNAL_SERVER_ERROR);

        return new ResponseEntity<>(newUser, HttpStatus.OK);
    }
    
    @RequestMapping(
            value = "/api/user/{id}", 
            method = RequestMethod.DELETE,
            consumes = MediaType.APPLICATION_JSON_VALUE )
    public ResponseEntity< Admins > deleteObj(@PathVariable("id") Integer id, @RequestBody Drone drone) {
        
        serviceBean.delete(id);     // update active to false
        return new ResponseEntity<>(HttpStatus.OK);
    }
}
