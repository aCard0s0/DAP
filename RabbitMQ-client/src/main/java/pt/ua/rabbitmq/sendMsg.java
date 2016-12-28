/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package pt.ua.rabbitmq;

import com.rabbitmq.client.Channel;
import com.rabbitmq.client.Connection;
import com.rabbitmq.client.ConnectionFactory;
import java.io.IOException;
import java.net.URISyntaxException;
import java.security.KeyManagementException;
import java.security.NoSuchAlgorithmException;
import java.util.concurrent.TimeoutException;

/**
 *
 * @author mezca
 */
public class sendMsg {

    public static void main(String args[]) throws IOException, TimeoutException {
        ConnectionFactory factory = new ConnectionFactory();
//        try {
//            //conta cloudamqp que já contem a password criada por tiago.coelho
//            factory.setUri("amqp://tetbnpzv:0xL4VEtuRN_YwSvu-4wrpND-OqlEjYWo@chicken.rmq.cloudamqp.com/tetbnpzv");
//        } catch (URISyntaxException | NoSuchAlgorithmException | KeyManagementException ex) {
//
//        }
        factory.setHost("192.168.160.174");
        Connection connection;
        Channel channel;

        connection = factory.newConnection();
        
        channel = connection.createChannel();
        channel.queueDeclare("droneActivities", false, false, false, null);

        String message = "DRONE;1;PARK;2";
        channel.basicPublish("", "droneActivities", null, message.getBytes("UTF-8"));
        System.out.println(" [x] Has sent '" + message + "'");

        channel.close();
        connection.close();

    }
}
