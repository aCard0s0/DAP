#import pika, os, urlparse, logging
import pika, os, ftplib, sys, datetime, requests,json, time


remoteDir = "/internal_000/Bebop_2/media"
ftpHost ="192.168.42.1"
ftpLogin = ""
ftpPassword = ""
rabitMQserver = "192.168.160.174"
restHost = "http://192.168.160.174:8080"
park = ""
drone = ""
localDownloadDir = "/home/andrecajus/Desktop/ProjectImgAlgorithm/temp/"
localFinalDownloadDir = "/home/andrecajus/Desktop/ProjectImgAlgorithm/"
nodejsScript = "flightplan.js"
#remoteDir = ""
#ftpHost ="94.23.76.188"
#ftpLogin = "testetiago"
#ftpPassword = "Abm9%c80"


def getbinary(ftp, filename):
    print ("filename",filename, "starting download..") 
    destinationFileName = "park" + str(park) + "_drone" + str(drone) + "_" +filename.replace(".mp4",".part")
    fhandle = open(localDownloadDir + destinationFileName , 'wb')
    ftp.retrbinary("RETR " + filename, fhandle.write)
    fhandle.flush()
    fhandle.close()
    time.sleep(5)
    os.rename("temp/" + destinationFileName, localFinalDownloadDir + destinationFileName.replace(".part",".mp4"))

    #print ("filename",filename, "successful downloaded..") 
    #successful = ftp.delete(filename)
    #if successful:
    #    print ("filename",filename, "successful deleted") 
    #else:
    #    print ("filename",filename, "cannot be deleted") 

def prepareFTP():		
	ftp = ftplib.FTP(ftpHost)
	ftp.login(ftpLogin, ftpPassword)

	ftp.cwd(remoteDir)

	try:
		file = open('flightPlan_' + str(park) +'.mavlink','rb')
		ftp.storbinary('STOR /internal_000/flightplans/flightPlan.mavlink',file)
	except FileNotFoundError as err:
		logg = {"state": "nao foi encontrado o mavlink do parque " + str(park), "idFlow": 1, "time": str(time.strftime('%Y-%m-%dT%H:%M:%S')) }
		url = restHost + "/api/logger/park/" + str(park) + "/drone/" + str(drone)
		headers = {'content-type': 'application/json'}

		requests.post(url, data=json.dumps(logg), headers=headers)
		return False

	data = []

	ftp.retrlines('LIST', callback=data.append)
	files = (line.rsplit(None, 1)[1] for line in data)
	files_list = list(files)

	for line in files_list:
		if line.find(".mp4") > -1:
			print ("removing file ->" + str(line))
			ftp.delete(line)
		
	ftp.quit()
	return True

def startFtpJob():		
	ftp = ftplib.FTP(ftpHost)
	ftp.login(ftpLogin, ftpPassword)

	ftp.cwd(remoteDir)

	data = []

	ftp.retrlines('LIST', callback=data.append)
	files = (line.rsplit(None, 1)[1] for line in data)
	files_list = list(files)

	for line in files_list:
		if line.find(".mp4") > -1:
			getbinary(ftp,line)
		
	ftp.quit()

def callNodeJsFromDrone():
    from Naked.toolshed.shell import execute_js

    logg = {"state": "executando captura no park " + str(park), "idFlow": 1, "time": str(time.strftime('%Y-%m-%dT%H:%M:%S')) }
    url = restHost + "/api/logger/park/" + str(park) + "/drone/" + str(drone)
    #para versoes diferentes
    #resp = requests.post(url, json=logg)
    headers = {'content-type': 'application/json'}

    requests.post(url, data=json.dumps(logg), headers=headers)


    ftpresult = prepareFTP()

    if ftpresult:
    	
	    success = execute_js(nodejsScript)
	    if success is False:
	        logg = {"state": "nao foi possivel executar o mavlink", "idFlow": 1, "time": str(time.strftime('%Y-%m-%dT%H:%M:%S')) }
	        #resp = requests.post(url, json=logg)
	        requests.post(url, data=json.dumps(logg), headers=headers)
	    else:
	        time.sleep(10)
	        
	        startFtpJob()
	        logg = {"state": "a enviar video para analise de imagem", "idFlow": 1, "time": str(time.strftime('%Y-%m-%dT%H:%M:%S')) }
	        #resp = requests.post(url, json=logg)
	        requests.post(url, data=json.dumps(logg), headers=headers)

	    print ("sent to watchfolder for image Analysis")



exchange_name = "droneActivities"
connection = pika.BlockingConnection(pika.ConnectionParameters(
        host=rabitMQserver))
channel = connection.channel()

channel.exchange_declare(exchange=exchange_name,
                         type='fanout')

result = channel.queue_declare(exclusive=True)
queue_name = result.method.queue

channel.queue_bind(exchange=exchange_name,
                   queue=queue_name)

print(' [*] Waiting for logs. To exit press CTRL+C')

def callback(ch, method, properties, body):
    print(" [x] %r" % body)
    bodystr = str(body).replace("'","")
    global drone
    drone =  bodystr.split(';')[1]
    global park
    park =  bodystr.split(';')[3]
   
    callNodeJsFromDrone()

channel.basic_consume(callback,
                      queue=queue_name,
                      no_ack=True)

channel.start_consuming()
print ("waiting for orders")
# send a message
#channel.basic_publish(exchange='', routing_key='hello', body='Hello CloudAMQP!')
#print " [x] Sent 'Hello World!'"






