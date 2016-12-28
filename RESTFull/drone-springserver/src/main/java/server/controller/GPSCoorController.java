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
import org.springframework.web.bind.annotation.CrossOrigin;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;
import server.domain.GPSCoor;
import server.domain.Park;
import server.service.GPSCoorService;
import server.service.ParkService;

/**
 *
 * @author acardoso
 */
@RestController
public class GPSCoorController {
    
    @Autowired
    private GPSCoorService serviceBean;       // Bean that execute the Service
    @Autowired
    private ParkService parkBean;
    
    @CrossOrigin
    @RequestMapping(
            value = "/api/gpscoor/park/{idPark}", 
            method = RequestMethod.POST,
            consumes = MediaType.APPLICATION_JSON_VALUE,
            produces = MediaType.APPLICATION_JSON_VALUE)
    public ResponseEntity< GPSCoor > createObj(
            @RequestBody GPSCoor coor,
            @PathVariable("idPark") Integer idPark) {   // id  of Park that this capture is assosiate with.
        
        coor.setParkidPark( parkBean.findOne( idPark ) );
        
        return new ResponseEntity<>( serviceBean.create(coor), HttpStatus.CREATED);
    }
    
    @RequestMapping( 
            value = "/api/gpscoor", 
            method = RequestMethod.GET,
            produces = MediaType.APPLICATION_JSON_VALUE )
    public ResponseEntity< Collection< GPSCoor> > getAll() {
        
        return new ResponseEntity<>( serviceBean.findAll(), HttpStatus.OK);
    }
    
    @RequestMapping(
            value = "/api/gpscoor/{id}", 
            method = RequestMethod.GET,
            produces = MediaType.APPLICATION_JSON_VALUE)
    public ResponseEntity< GPSCoor > getOne(@PathVariable("id") Integer id) {
        
        GPSCoor coor = serviceBean.findOne( id );
        if (coor == null) {
            return new ResponseEntity<>(HttpStatus.NOT_FOUND);
        }
        return new ResponseEntity<  >(coor, HttpStatus.OK);
    }
    
    @RequestMapping(
            value = "/api/gpscoor/park/{idPark}", 
            method = RequestMethod.GET,
            produces = MediaType.APPLICATION_JSON_VALUE)
    public ResponseEntity< Collection< GPSCoor > > getGPSCoorByPark (@PathVariable("idPark") Integer idPark) {
        
        Park park = parkBean.findOne( idPark );
        if (park == null) {
            return new ResponseEntity<>(HttpStatus.NOT_FOUND);
        }
        
        return new ResponseEntity<  >(park.getGPSCoorCollection(), HttpStatus.OK);
    }
    
    @RequestMapping(
            value = "/api/gpscoor/{id}", 
            method = RequestMethod.PUT,
            consumes = MediaType.APPLICATION_JSON_VALUE,
            produces = MediaType.APPLICATION_JSON_VALUE)
    public ResponseEntity< GPSCoor > updateObj(@RequestBody GPSCoor coor) {
        
        GPSCoor newCoor = serviceBean.update(coor);
        if (newCoor == null)
            return new ResponseEntity<>(HttpStatus.INTERNAL_SERVER_ERROR);

        return new ResponseEntity<>(newCoor, HttpStatus.OK);
    }
    
    @RequestMapping(
            value = "/api/gpscoor/{id}", 
            method = RequestMethod.DELETE,
            consumes = MediaType.APPLICATION_JSON_VALUE )
    public ResponseEntity< GPSCoor > deleteObj(@PathVariable("id") Integer id, @RequestBody GPSCoor coor) {
        
        //serviceBean.delete(id);     // update active to false
        //return new ResponseEntity<>(HttpStatus.OK);
        return new ResponseEntity<>(HttpStatus.NOT_ACCEPTABLE);
    }
      @CrossOrigin  
      @RequestMapping(
            value = "/api/gpscoor/park/{id}", 
            method = RequestMethod.DELETE )
    public ResponseEntity<Boolean> deleteObj(@PathVariable("id") Integer id) {
        //System.out.println("remove all gpscoords from park " + id);
        Park park = parkBean.findOne(id);
        if (park == null) {
              return new ResponseEntity<>(false,HttpStatus.NOT_FOUND);
        }
        Collection<GPSCoor> gpscoords = park.getGPSCoorCollection();
        for (GPSCoor gpsCoor : gpscoords) {
            serviceBean.deletePerm(gpsCoor.getIdGPSCoor());
            
        }
        return new ResponseEntity<>(true,HttpStatus.OK);
    }
    
}
