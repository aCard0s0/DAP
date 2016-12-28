package server.service;

import java.util.Collection;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import server.domain.Park;
import server.repository.ParkRepo;

/**
 *      This BEAN will do the operations on Capture.class (database entity)
 *  using Spring Repository (JpaRepository<Object, Serializable>).
 * 
 * @author acardoso
 */
@Service
public class ParkService implements ServiceBean<Park, Integer>{

    @Autowired
    private ParkRepo repo;
    
    @Override
    public Park create(Park tblObj) {
        
        // save method update if exist and create if not.
        // this prevent that behavior.
        if (tblObj.getIdPark() != null)
            return null;
        
        return repo.save(tblObj);
    }

    @Override
    public Collection<Park> findAll() {
        return repo.findAll();
    }

    @Override
    public Park findOne(Integer id) {
        return repo.findOne(id);
    }

    @Override
    public Park update(Park tblObj) {
        
        // save method update if exist and create if not.
        // this prevent that behavior.
        Park newPark = findOne( tblObj.getIdPark() );
        if (newPark == null)
            return null;
        
        // updating...
        newPark.setName( tblObj.getName() );
        newPark.setNLots( tblObj.getNLots());
        newPark.setCentraLatitude( tblObj.getCentraLatitude() );
        newPark.setCentraLongitude( tblObj.getCentraLongitude() );
        newPark.setType(tblObj.getType());
        newPark.setParkImg(tblObj.getParkImg());
        //newPark.setActive( tblObj.getActive() );
    
        return repo.save( newPark );
    }

    @Override
    public Park delete(Integer id) {
        
        Park delPark = findOne( id );
        if (delPark == null)
            return null;
        
 
        repo.delete(delPark );
        return delPark;
    }
    
}
