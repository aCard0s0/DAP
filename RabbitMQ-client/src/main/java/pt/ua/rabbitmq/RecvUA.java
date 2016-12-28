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
public class RecvUA {

   private static final String EXCHANGE_NAME = "droneActivities";
   
   public static void main(String[] argv) throws Exception {
    ConnectionFactory factory = new ConnectionFactory();
//    factory.setPort(5673);
    System.out.println(factory.getPort());
    factory.setHost("192.168.160.174");
    Connection connection = factory.newConnection();
    Channel channel = connection.createChannel();

    channel.exchangeDeclare(EXCHANGE_NAME, "fanout");
    String queueName = channel.queueDeclare().getQueue();
    channel.queueBind(queueName, EXCHANGE_NAME, "");

    System.out.println(" [*] Waiting for messages. To exit press CTRL+C");

    Consumer consumer = new DefaultConsumer(channel) {
      @Override
      public void handleDelivery(String consumerTag, Envelope envelope,
                                 AMQP.BasicProperties properties, byte[] body) throws IOException {
        String message = new String(body, "UTF-8");
        System.out.println(" [x] Received '" + message + "'");
      }
    };
    channel.basicConsume(queueName, true, consumer);
  }
}