/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package pt.ua.rabbitmq;

import com.rabbitmq.client.*;
import java.io.IOException;
/**
 *
 * @author Tiago
 */
public class Recv {

  private final static String QUEUE_NAME = "droneActivities";

  public static void main(String[] argv) throws Exception {
    ConnectionFactory factory = new ConnectionFactory();
    //factory.setPort(445);
    System.out.println(factory.getPort());
    //conta cloudamqp que já contem a password criada por tiago.coelho
    //factory.setUri("amqp://tetbnpzv:0xL4VEtuRN_YwSvu-4wrpND-OqlEjYWo@chicken.rmq.cloudamqp.com/tetbnpzv");
     factory.setHost("192.168.160.174"); 
    Connection connection = factory.newConnection();
    
    Channel channel = connection.createChannel();
  
    channel.queueDeclare(QUEUE_NAME, false, false, false, null);
    System.out.println(" [*] Waiting for messages teste. To exit press CTRL+C");

    Consumer consumer = new DefaultConsumer(channel) {
      @Override
      public void handleDelivery(String consumerTag, Envelope envelope, AMQP.BasicProperties properties, byte[] body)
          throws IOException {
        String message = new String(body, "UTF-8");
        System.out.println(" [x] Has received '" + message + "'");
      }
    };
    channel.basicConsume(QUEUE_NAME, true, consumer);
  }
}