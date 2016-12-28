/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package server.repository;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;
import server.domain.Logger;

/**
 *
 * @author tiago
 */
@Repository
public interface LoggerRepo extends JpaRepository<Logger, Integer>{
//    @Query("from Logger l where l.idFlow=:idFlow")
//    Page<Logger> findByExample(@Param("idFlow") String idFlow,  Pageable page);

}
