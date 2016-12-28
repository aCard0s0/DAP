import pika, os, ftplib, sys, datetime, requests,time

remoteDir = "/internal_000/Bebop_2/media"
localDownloadDir = "/home/andrecajus/Desktop/ProjectImgAlgorithm/temp/"
localFinalDownloadDir = "/home/andrecajus/Desktop/ProjectImgAlgorithm/"
ftpHost ="192.168.42.1"
ftpLogin = ""
ftpPassword = ""
park = "9"
drone = "4"

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
		
ftp = ftplib.FTP(ftpHost)
ftp.login(ftpLogin, ftpPassword)

ftp.cwd(remoteDir)

data = []

#ftp.dir(data.append)
ftp.retrlines('LIST', callback=data.append)
files = (line.rsplit(None, 1)[1] for line in data)
files_list = list(files)
for line in files_list:
    print(line)
    if line.find(".mp4") > -1:
        getbinary(ftp,line)
	
ftp.quit()


    #print ("-", line)
