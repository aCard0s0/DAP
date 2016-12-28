/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package server.controller;

import com.google.common.io.Files;
import drone.communication.PhotoWithBytes;
import java.io.File;
import java.io.IOException;
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
import server.domain.Photo;
import server.service.CaptureService;
import server.service.PhotoService;

/**
 *
 * @author acardoso
 */
@RestController
public class PhotoController {

    @Autowired
    private PhotoService serviceBean;       // Bean that execute the Service
    @Autowired
    private CaptureService captureBean;

    @RequestMapping(
            value = "/api/photo/capture/{idCapture}",
            method = RequestMethod.POST)
    public ResponseEntity<Photo> createObj(
            @RequestBody PhotoWithBytes photoWithBytes,
            @PathVariable("idCapture") Integer idCapture) {   // id  of Park that this capture is assosiate with.

        if (serviceBean.getPhoto(photoWithBytes, idCapture))
            return new ResponseEntity<>( HttpStatus.CREATED );
        
        return new ResponseEntity<>(HttpStatus.UNSUPPORTED_MEDIA_TYPE);
    }

    @RequestMapping(
            value = "/api/photo",
            method = RequestMethod.GET,
            produces = MediaType.APPLICATION_JSON_VALUE)
    public ResponseEntity< Collection< Photo>> getAll() {

        return new ResponseEntity<>(serviceBean.findAll(), HttpStatus.OK);
    }

    @RequestMapping(
            value = "/api/photo/{id}",
            method = RequestMethod.GET,
            produces = MediaType.APPLICATION_JSON_VALUE)
    public ResponseEntity< Photo> getOne(@PathVariable("id") Integer id) {

        Photo photo = serviceBean.findOne(id);
        if (photo == null) {
            return new ResponseEntity<>(HttpStatus.NOT_FOUND);
        }
        return new ResponseEntity<>(photo, HttpStatus.OK);
    }

    @RequestMapping(
            value = "/api/photo/capture/{idCapture}",
            method = RequestMethod.GET,
            produces = MediaType.APPLICATION_JSON_VALUE)
    public ResponseEntity< Collection< Photo>> getPhotosByCapture(
            @PathVariable("idCapture") Integer idCapture) {

        Capture capture = captureBean.findOne(idCapture);
        if (capture == null) {
            return new ResponseEntity<>(HttpStatus.NOT_FOUND);
        }
        return new ResponseEntity<>(capture.getPhotoCollection(), HttpStatus.OK);
    }

    @RequestMapping(
            value = "/api/photo/{id}",
            method = RequestMethod.PUT,
            consumes = MediaType.APPLICATION_JSON_VALUE,
            produces = MediaType.APPLICATION_JSON_VALUE)
    public ResponseEntity< Photo> updateObj(@RequestBody Photo photo) {

        Photo newPhoto = serviceBean.update(photo);
        if (newPhoto == null) {
            return new ResponseEntity<>(HttpStatus.INTERNAL_SERVER_ERROR);
        }

        return new ResponseEntity<>(newPhoto, HttpStatus.OK);
    }

    @RequestMapping(
            value = "/api/photo/{id}",
            method = RequestMethod.DELETE,
            consumes = MediaType.APPLICATION_JSON_VALUE)
    public ResponseEntity< Photo> deleteObj(@PathVariable("id") Integer id, @RequestBody Photo photo) {

        //serviceBean.delete(id);     // update active to false
        //return new ResponseEntity<>(HttpStatus.OK);
        return new ResponseEntity<>(HttpStatus.NOT_ACCEPTABLE);
    }
}
