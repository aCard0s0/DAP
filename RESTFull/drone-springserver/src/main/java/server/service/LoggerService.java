/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package server.service;

import java.util.Collection;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import server.domain.Logger;
import server.repository.LoggerRepo;

/**
 *
 * @author tiago
 */
@Service
public class LoggerService implements ServiceBean<Logger, Integer>{

    @Autowired
    private LoggerRepo repo;
    
    @Override
    public Logger create(Logger tblObj) {
        
        return repo.save( tblObj );
    }

    @Override
    public Collection<Logger> findAll() {
    
        return repo.findAll();
    }

    @Override
    public Logger findOne(Integer id) {
        return repo.findOne(id);
    }

    @Override
    public Logger update(Logger tblObj) {
        
        // save method update if exist and create if not.
        // this prevent that behavior.
        Logger logger = findOne( tblObj.getIdLogger());
        if (logger == null)
            return null;
        
        
    
        return repo.save( logger );
    }

    @Override
    public Logger delete(Integer id) {
                
        Logger logger = findOne( id );
        if (logger == null)
            return null;
        
       
        return repo.save( logger );    
    }

  
    
}
