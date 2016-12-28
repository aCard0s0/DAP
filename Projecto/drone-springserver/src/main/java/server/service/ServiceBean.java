package server.service;

import java.util.Collection;

/**
 *  Plant for all Services.
 *      POST      - create()
 *      GET       - findAll()
 *      GET/id    - findOne()
 *      PUT/id    - update()
 *      DELETE/id - delete()
 * 
 * @author acardoso
 * @param <T>   T is the table class.
 * @param <P>   P is the PK type
 */
public interface ServiceBean<T, P> {

    T create(T tblObj);    
    
    Collection< T > findAll();

    T findOne(P id);

    T update(T tblObj);

    T delete(P id);
    
   
}
