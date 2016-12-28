/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package server.service;

import com.rabbitmq.client.Channel;
import com.rabbitmq.client.Connection;
import com.rabbitmq.client.ConnectionFactory;
import java.io.IOException;
import java.util.Collection;
import java.util.concurrent.TimeoutException;
import java.util.logging.Level;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import server.domain.Drone;
import server.repository.DroneRepo;

/**
 *
 * @author acardoso
 */
@Service
public class DroneService implements ServiceBean<Drone, Integer>{

    private static String EXCHANGE_NAME ="droneActivities";
    private final String MY_IP = "192.168.160.174";
    
    @Autowired
    private DroneRepo repo;
    
    ConnectionFactory factory;
    Connection connection;
    Channel channel;
    
    @Override
    public Drone create(Drone tblObj) {
        
        // save method update if exist and create if not.
        // this prevent that behavior.
        if (tblObj.getIdDrone() != null)
            return null;
        
       // new Park( new ParkService().findOne( 1 ) );
        //tblObj.setParkidPark( );
        
        return repo.save( tblObj );
    }

    public void sendDrone(Integer id, Integer parkId) 
            throws IOException, TimeoutException{
        
    
        factory = new ConnectionFactory();
        factory.setHost( MY_IP );
        
        connection = factory.newConnection();
        channel = connection.createChannel();
        channel.exchangeDeclare(EXCHANGE_NAME, "fanout");
        
        String message = "DRONE;" + id +";PARK;" + parkId;
        channel.basicPublish(EXCHANGE_NAME, "", null, message.getBytes());
        System.out.println(" [x] Has sent '" + message + "'");

        channel.close();
        connection.close();
    }
    
    @Override
    public Collection<Drone> findAll() {
        return repo.findAll();
    }

    @Override
    public Drone findOne(Integer id) {
        return repo.findOne(id);
    }

    @Override
    public Drone update(Drone tblObj) {
        
        // save method update if exist and create if not.
        // this prevent that behavior.
        Drone newDrone = findOne( tblObj.getIdDrone());
        if (newDrone == null)
            return null;
        
        // updating...
        newDrone.setName(tblObj.getName());
        //newDrone.setActive( newDrone.getActive() );
    
        return repo.save( newDrone );
    }

    @Override
    public Drone delete(Integer id) {
                
        Drone delDrone = findOne( id );
        if (delDrone == null)
            return null;
        
        delDrone.setActive( Boolean.FALSE );   // deleted indicator
        return repo.save( delDrone );    
    }
    
}
