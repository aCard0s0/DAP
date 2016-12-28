package server.repository;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;
import server.domain.GPSCoor;

/**
 *
 * @author acardoso
 */
@Repository
public interface GPSCoorRepo extends JpaRepository<GPSCoor, Integer>{
    
}
