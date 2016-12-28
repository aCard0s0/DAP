package server.service;

import java.util.Collection;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import server.domain.Capture;
import server.repository.CaptureRepo;

/**
 *      This BEAN will do the operations on Capture.class (database entity)
 *  using Spring Repository (JpaRepository<Object, Serializable>).
 * 
 * @author acardoso
 */
@Service
public class CaptureService implements ServiceBean<Capture, Integer>{

    @Autowired
    private CaptureRepo repo;

    @Override
    public Capture create(Capture tblObj) {
        
        // save method update if exist and create if not.
        // this prevent that behavior.
        if (tblObj.getIdCapture() != null)
            return null;
        
       // new Park( new ParkService().findOne( 1 ) );
        //tblObj.setParkidPark( );
        
        return repo.save( tblObj );
    }

    @Override
    public Collection<Capture> findAll() {
        return repo.findAll();
    }

    @Override
    public Capture findOne(Integer id) {
        return repo.findOne(id);
    }

    @Override
    public Capture update(Capture tblObj) {
        
        // save method update if exist and create if not.
        // this prevent that behavior.
        Capture newCapture = findOne( tblObj.getIdCapture());
        if (newCapture == null)
            return null;
        
        // updating...
        newCapture.setParkidPark( tblObj.getParkidPark() );
        newCapture.setStartedTime( tblObj.getStartedTime() );
        newCapture.setFinishTime( tblObj.getFinishTime() );
        newCapture.setNOccupiedLots( tblObj.getNOccupiedLots() );
        //newCapture.setValid( tblObj.getValid() );
    
        return repo.save( newCapture );
    }

    @Override
    public Capture delete(Integer id) {
        
        Capture delCapture = findOne( id );
        if (delCapture == null)
            return null;
        
        delCapture.setValid( Boolean.FALSE );   // deleted indicator
        return repo.save( delCapture );
    }
    
}
