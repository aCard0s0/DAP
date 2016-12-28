/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package server.domain;

import com.fasterxml.jackson.annotation.JsonIgnore;
import java.io.Serializable;
import java.util.Date;
import javax.persistence.Basic;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToOne;
import javax.persistence.NamedQueries;
import javax.persistence.NamedQuery;
import javax.persistence.Table;
import javax.persistence.Temporal;
import javax.persistence.TemporalType;
import javax.validation.constraints.NotNull;
import javax.validation.constraints.Size;
import javax.xml.bind.annotation.XmlRootElement;

/**
 *
 * @author tiago
 */
@Entity
@Table(name = "logger")
@XmlRootElement
@NamedQueries({
    @NamedQuery(name = "Logger.findAll", query = "SELECT l FROM Logger l"),
    @NamedQuery(name = "Logger.findByIdLogger", query = "SELECT l FROM Logger l WHERE l.idLogger = :idLogger"),
    @NamedQuery(name = "Logger.findByState", query = "SELECT l FROM Logger l WHERE l.state = :state"),
    @NamedQuery(name = "Logger.findByTime", query = "SELECT l FROM Logger l WHERE l.time = :time"),
    @NamedQuery(name = "Logger.findByIdFlow", query = "SELECT l FROM Logger l WHERE l.idFlow = :idFlow")})
public class Logger implements Serializable {

    private static final long serialVersionUID = 1L;
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    @Basic(optional = false)
    @Column(name = "idLogger")
    private Integer idLogger;
    @Size(max = 45)
    @Column(name = "state")
    private String state;
    @Column(name = "time")
    @Temporal(TemporalType.TIMESTAMP)
    private Date time;
    @Column(name = "idFlow")
    private Integer idFlow;
    @JoinColumn(name = "Drone_idDrone", referencedColumnName = "idDrone")
    @ManyToOne
    private Drone droneidDrone;
    @JoinColumn(name = "Park_idPark", referencedColumnName = "idPark")
    @ManyToOne
    private Park parkidPark;

    public Logger() {
    }

    public Logger(Integer idLogger) {
        this.idLogger = idLogger;
    }

    public Integer getIdLogger() {
        return idLogger;
    }

    public void setIdLogger(Integer idLogger) {
        this.idLogger = idLogger;
    }

    public String getState() {
        return state;
    }

    public void setState(String state) {
        this.state = state;
    }

    public Date getTime() {
        return time;
    }

    public void setTime(Date time) {
        this.time = time;
    }

    public Integer getIdFlow() {
        return idFlow;
    }

    public void setIdFlow(Integer idFlow) {
        this.idFlow = idFlow;
    }

    public Drone getDroneidDrone() {
        return droneidDrone;
    }

    public void setDroneidDrone(Drone droneidDrone) {
        this.droneidDrone = droneidDrone;
    }

    public Park getParkidPark() {
        return parkidPark;
    }

    public void setParkidPark(Park parkidPark) {
        this.parkidPark = parkidPark;
    }

    @Override
    public int hashCode() {
        int hash = 0;
        hash += (idLogger != null ? idLogger.hashCode() : 0);
        return hash;
    }

    @Override
    public boolean equals(Object object) {
        // TODO: Warning - this method won't work in the case the id fields are not set
        if (!(object instanceof Logger)) {
            return false;
        }
        Logger other = (Logger) object;
        if ((this.idLogger == null && other.idLogger != null) || (this.idLogger != null && !this.idLogger.equals(other.idLogger))) {
            return false;
        }
        return true;
    }

    @Override
    public String toString() {
        return "server.domain.Logger[ idLogger=" + idLogger + " ]";
    }
    
}
