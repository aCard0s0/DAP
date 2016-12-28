package server.repository;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;
import server.domain.Park;

/**
 *
 * @author acardoso
 */
@Repository
public interface ParkRepo extends JpaRepository<Park, Integer>{
    
}
