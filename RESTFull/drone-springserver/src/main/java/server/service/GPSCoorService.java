package server.service;

import java.util.Collection;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import server.domain.GPSCoor;
import server.repository.GPSCoorRepo;

/**
 *      This BEAN will do the operations on Capture.class (database entity)
 *  using Spring Repository (JpaRepository<Object, Serializable>).
 * 
 * @author acardoso
 */
@Service
public class GPSCoorService implements ServiceBean<GPSCoor, Integer>{

    @Autowired
    private GPSCoorRepo repo;
    
    @Override
    public GPSCoor create(GPSCoor tblObj) {
        
        // save method update if exist and create if not.
        // this prevent that behavior.
        if (tblObj.getIdGPSCoor()!= null)
            return null;
        
        return repo.save(tblObj);
    }

    @Override
    public Collection<GPSCoor> findAll() {
        return repo.findAll();
    }

    @Override
    public GPSCoor findOne(Integer id) {
        return repo.findOne(id);
    }

    @Override
    public GPSCoor update(GPSCoor tblObj) {
       
        // save method update if exist and create if not.
        // this prevent that behavior.
        GPSCoor newGPScoor = findOne( tblObj.getIdGPSCoor() );
        if (newGPScoor == null)
            return null;
        
        // updating...
        newGPScoor.setParkidPark( tblObj.getParkidPark() );
        newGPScoor.setIndex( tblObj.getIndex());
        newGPScoor.setLatitude( tblObj.getLatitude() );
        newGPScoor.setLongitude( tblObj.getLongitude() );
    
        return repo.save( newGPScoor );
    }

    @Override
    public GPSCoor delete(Integer id) {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }
    
    public void deletePerm(Integer id) {
        
        
        GPSCoor newGPScoor = findOne( id );
         if (newGPScoor != null) {
             System.out.println("vou eliminar " + newGPScoor.getIdGPSCoor());
             repo.delete(newGPScoor);
         }
         
       
    }
}
