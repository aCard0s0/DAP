import pika, os, ftplib, sys, datetime, requests

localDownloadDir = "/home/andrecajus/Desktop/ProjectImgAlgorithm/temp/"
localFinalDownloadDir = "/home/andrecajus/Desktop/ProjectImgAlgorithm/"
remoteDir = ""
ftpHost ="94.23.76.188"
ftpLogin = "testetiago"
ftpPassword = "Abm9%c80"


def getbinary(ftp, filename):
    destinationFileName = "park1_drone5_" +filename.replace(".mp4",".part")
    fhandle = open(localDownloadDir + destinationFileName , 'wb')
    ftp.retrbinary("RETR " + filename, fhandle.write)
    fhandle.flush()
    fhandle.close()

    os.rename("temp/" + destinationFileName, localFinalDownloadDir + destinationFileName.replace(".part",".mp4"))
    print ("filename",filename, "successful downloaded..") 
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
	getbinary(ftp,line)
	
ftp.quit()


    #print ("-", line)
