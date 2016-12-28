/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package server.repository;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;
import server.domain.Drone;

/**
 *
 * @author acardoso
 */
@Repository
public interface DroneRepo extends JpaRepository<Drone, Integer>{
    
}