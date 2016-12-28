/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package com.mycompany.drone.client;

import drone.communication.PhotoWithBytes;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.logging.Level;
import java.util.logging.Logger;
import org.springframework.web.client.RestTemplate;

/**
 *
 * @author mezca
 */
public class Main {

    public static void main(String args[]) {

        //o id da captura tem que existir na BD
        final String uri = "http://192.168.160.174:8080/api/photo/capture/10";

        PhotoWithBytes photoWithBytes = new PhotoWithBytes();
        
        try {
            photoWithBytes.setPhoto( Files.readAllBytes(Paths.get("banner1.jpg") ));
        } catch (IOException ex) {}
        
        photoWithBytes.setFileName("bannerteste23332.jpg");
        RestTemplate restTemplate = new RestTemplate();
        restTemplate.postForObject(uri, photoWithBytes, PhotoWithBytes.class);

        //System.out.println(result);
    }
}
