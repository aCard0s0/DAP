/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package server.service;

import java.util.Collection;
import java.util.Iterator;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import server.domain.Admins;
import server.repository.AdminsRepo;

/**
 *
 * @author acardoso
 */
@Service
public class AdminsService implements ServiceBean<Admins, Integer>{

    @Autowired
    private AdminsRepo repo;
    
    @Override
    public Admins create(Admins tblObj) {
        
        // save method update if exist and create if not.
        // this prevent that behavior.
        if (tblObj.getIdUser()!= null)
            return null;
        
        return repo.save( tblObj );
    }
    
    public boolean login(String email, String pass){
        
        Collection<Admins> allUsers = findAll();
        Iterator<Admins> itrUser = allUsers.iterator();
        
        while(itrUser.hasNext()){
            Admins tmpUser = itrUser.next();
            if (tmpUser.getIsAdmin() && 
                    tmpUser.getEmail().equals( email ) && 
                    tmpUser.getPassword().equals( pass )){
                return true;
            }
        }
        return false;
    }

    @Override
    public Collection<Admins> findAll() {
        return repo.findAll();
    }

    @Override
    public Admins findOne(Integer id) {
        return repo.findOne(id);
    }

    @Override
    public Admins update(Admins tblObj) {
        
        // save method update if exist and create if not.
        // this prevent that behavior.
        Admins newUser = findOne( tblObj.getIdUser());
        if (newUser == null)
            return null;
        
        // updating...
        newUser.setEmail(tblObj.getEmail() );
        newUser.setPassword( tblObj.getPassword() );
        newUser.setIsAdmin( tblObj.getIsAdmin() );
    
        return repo.save( newUser );
    }

    @Override
    public Admins delete(Integer id) {
                
        Admins delDrone = findOne( id );
        if (delDrone == null)
            return null;
        
        delDrone.setIsAdmin( Boolean.FALSE );   // deleted indicator
        return repo.save( delDrone );    
    }
    
}
