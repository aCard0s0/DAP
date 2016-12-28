package server.repository;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;
import server.domain.Photo;

/**
 *
 * @author acardoso
 */
@Repository
public interface PhotoRepo extends JpaRepository<Photo, Integer>{
    
}
