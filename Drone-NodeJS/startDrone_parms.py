#import pika, os, urlparse, logging
import pika, os, ftplib, sys, datetime, requests



from Naked.toolshed.shell import execute_js

success = execute_js('flightplan.js yes')
print(success)

print ("waiting for orders")
# send a message
#channel.basic_publish(exchange='', routing_key='hello', body='Hello CloudAMQP!')
#print " [x] Sent 'Hello World!'"






