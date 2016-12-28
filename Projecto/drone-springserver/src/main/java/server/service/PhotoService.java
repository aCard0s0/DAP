package server.service;

import com.google.common.io.Files;
import drone.communication.PhotoWithBytes;
import java.io.File;
import java.io.IOException;
import java.util.Collection;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import server.domain.Photo;
import server.repository.PhotoRepo;

/**
 *      This BEAN will do the operations on Capture.class (database entity)
 *  using Spring Repository (JpaRepository<Object, Serializable>).
 * 
 * @author acardoso
 */
@Service
public class PhotoService implements ServiceBean<Photo, Integer>{

    @Autowired
    private PhotoRepo repo;
    
    @Autowired
    private CaptureService captureBean;
    
    @Override
    public Photo create(Photo tblObj) {
        
        // save method update if exist and create if not.
        // this prevent that behavior.
        if (tblObj.getIdPhoto()!= null)
            return null;
        
        return repo.save(tblObj);
    }

    public boolean getPhoto(PhotoWithBytes photoWithBytes, Integer idCapture){
        
        Photo photo = new Photo();
        // localização das imagens
        // maybe does not work in linux?
        System.out.println("recebi novo ficheiro");
        
        //String filepath = "src/main/resources/public/imgparks/" + idCapture + "_" + photoWithBytes.getFileName();
        String filepath = "public/imgparks/" + idCapture + "_" + photoWithBytes.getFileName();
   
        // fazer decodeBase64 de photoWithBytes.getPhoto()
        
        try {
            Files.write(photoWithBytes.getPhoto(), new File(filepath));
        } catch (IOException ex) {
            System.out.println(ex.toString());
            return false;
        }

        photo.setSrc(filepath);
        photo.setLatitude(photoWithBytes.getLatitude());
        photo.setLongitude(photoWithBytes.getLongitude());
        photo.setHeigth(photoWithBytes.getHeigth());
        photo.setCaptureidCapture(captureBean.findOne(idCapture));
        create(photo);
        
        return true;
    }
    
    public void decodeBase64Img(byte[] photoToDecode){
        
        
        
    }
    
    @Override
    public Collection<Photo> findAll() {
        return repo.findAll();
    }

    @Override
    public Photo findOne(Integer id) {
        return repo.findOne(id);
    }

    @Override
    public Photo update(Photo tblObj) {
        
        // save method update if exist and create if not.
        // this prevent that behavior.
        Photo newPhoto = findOne( tblObj.getIdPhoto());
        if (newPhoto == null)
            return null;
        
        // updating...
        newPhoto.setCaptureidCapture( tblObj.getCaptureidCapture() );
        newPhoto.setSrc( tblObj.getSrc() );
        newPhoto.setLatitude( tblObj.getLatitude() );
        newPhoto.setLongitude( tblObj.getLongitude() );
        newPhoto.setHeigth( tblObj.getHeigth() );
    
        return repo.save( newPhoto );
    }

    @Override
    public Photo delete(Integer id) {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }
    
    
}
