/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package server.domain;

import com.fasterxml.jackson.annotation.JsonIgnore;
import java.io.Serializable;
import java.util.Collection;
import javax.persistence.Basic;
import javax.persistence.CascadeType;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.NamedQueries;
import javax.persistence.NamedQuery;
import javax.persistence.OneToMany;
import javax.persistence.Table;
import javax.validation.constraints.Size;
import javax.xml.bind.annotation.XmlRootElement;
import javax.xml.bind.annotation.XmlTransient;

/**
 *
 * @author acardoso
 */
@Entity
@Table(name = "drone")
@XmlRootElement
@NamedQueries({
    @NamedQuery(name = "Drone.findAll", query = "SELECT d FROM Drone d"),
    @NamedQuery(name = "Drone.findByIdDrone", query = "SELECT d FROM Drone d WHERE d.idDrone = :idDrone"),
    @NamedQuery(name = "Drone.findByName", query = "SELECT d FROM Drone d WHERE d.name = :name"),
    @NamedQuery(name = "Drone.findByActive", query = "SELECT d FROM Drone d WHERE d.active = :active")})
public class Drone implements Serializable {

    @OneToMany(mappedBy = "droneidDrone")
    @JsonIgnore
    private Collection<Logger> loggerCollection;

    private static final long serialVersionUID = 1L;
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    @Basic(optional = false)
    @Column(name = "idDrone")
    private Integer idDrone;
    @Size(max = 45)
    @Column(name = "name")
    private String name;
    @Column(name = "active")
    private Boolean active;
    @OneToMany(cascade = CascadeType.ALL, mappedBy = "droneidDrone")
    private Collection<Capture> captureCollection;

    public Drone() {
    }

    public Drone(Integer idDrone) {
        this.idDrone = idDrone;
    }

    public Integer getIdDrone() {
        return idDrone;
    }

    public void setIdDrone(Integer idDrone) {
        this.idDrone = idDrone;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public Boolean getActive() {
        return active;
    }

    public void setActive(Boolean active) {
        this.active = active;
    }

    @XmlTransient
    public Collection<Capture> getCaptureCollection() {
        return captureCollection;
    }

    public void setCaptureCollection(Collection<Capture> captureCollection) {
        this.captureCollection = captureCollection;
    }

    @Override
    public int hashCode() {
        int hash = 0;
        hash += (idDrone != null ? idDrone.hashCode() : 0);
        return hash;
    }

    @Override
    public boolean equals(Object object) {
        // TODO: Warning - this method won't work in the case the id fields are not set
        if (!(object instanceof Drone)) {
            return false;
        }
        Drone other = (Drone) object;
        if ((this.idDrone == null && other.idDrone != null) || (this.idDrone != null && !this.idDrone.equals(other.idDrone))) {
            return false;
        }
        return true;
    }

    @Override
    public String toString() {
        return "server.domain.Drone[ idDrone=" + idDrone + " ]";
    }

    @XmlTransient
    @JsonIgnore
    public Collection<Logger> getLoggerCollection() {
        return loggerCollection;
    }

    public void setLoggerCollection(Collection<Logger> loggerCollection) {
        this.loggerCollection = loggerCollection;
    }
    
}
