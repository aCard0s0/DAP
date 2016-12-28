package server.repository;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;
import server.domain.Capture;

/**
 *
 * @author acardoso
 */
@Repository
public interface CaptureRepo extends JpaRepository<Capture, Integer>{
    
}
