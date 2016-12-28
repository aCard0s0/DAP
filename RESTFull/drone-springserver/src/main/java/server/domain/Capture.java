/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package server.domain;

import com.fasterxml.jackson.annotation.JsonIgnore;
import java.io.Serializable;
import java.util.Collection;
import java.util.Date;
import javax.persistence.Basic;
import javax.persistence.CascadeType;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToOne;
import javax.persistence.NamedQueries;
import javax.persistence.NamedQuery;
import javax.persistence.OneToMany;
import javax.persistence.Table;
import javax.persistence.Temporal;
import javax.persistence.TemporalType;
import javax.validation.constraints.NotNull;
import javax.xml.bind.annotation.XmlRootElement;
import javax.xml.bind.annotation.XmlTransient;

/**
 *
 * @author acardoso
 */
@Entity
@Table(name = "capture")
@XmlRootElement
@NamedQueries({
    @NamedQuery(name = "Capture.findAll", query = "SELECT c FROM Capture c"),
    @NamedQuery(name = "Capture.findByIdCapture", query = "SELECT c FROM Capture c WHERE c.idCapture = :idCapture"),
    @NamedQuery(name = "Capture.findByStartedTime", query = "SELECT c FROM Capture c WHERE c.startedTime = :startedTime"),
    @NamedQuery(name = "Capture.findByFinishTime", query = "SELECT c FROM Capture c WHERE c.finishTime = :finishTime"),
    @NamedQuery(name = "Capture.findByNOccupiedLots", query = "SELECT c FROM Capture c WHERE c.nOccupiedLots = :nOccupiedLots"),
    @NamedQuery(name = "Capture.findByValid", query = "SELECT c FROM Capture c WHERE c.valid = :valid")})
public class Capture implements Serializable {

    private static final long serialVersionUID = 1L;
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    @Basic(optional = false)
    @Column(name = "idCapture")
    private Integer idCapture;
    
    @Basic(optional = false)
    @NotNull
    @Column(name = "startedTime")
    @Temporal(TemporalType.TIMESTAMP)
    private Date startedTime;
    
    @Column(name = "finishTime")
    @Temporal(TemporalType.TIMESTAMP)
    private Date finishTime;
    
    @Column(name = "nOccupiedLots")
    private Integer nOccupiedLots;
    
    @Column(name = "valid")
    private Boolean valid;
    
    @OneToMany(cascade = CascadeType.ALL, mappedBy = "captureidCapture")
    private Collection<Photo> photoCollection;
    
    @JoinColumn(name = "Park_idPark", referencedColumnName = "idPark")
    @JsonIgnore
    @ManyToOne(optional = false)
    private Park parkidPark;
    
    @JoinColumn(name = "Drone_idDrone", referencedColumnName = "idDrone")
    @JsonIgnore
    @ManyToOne(optional = false)
    private Drone droneidDrone;

    public Capture() {
    }

    public Capture(Integer idCapture) {
        this.idCapture = idCapture;
    }

    public Capture(Integer idCapture, Date startedTime) {
        this.idCapture = idCapture;
        this.startedTime = startedTime;
    }

    public Integer getIdCapture() {
        return idCapture;
    }

    public void setIdCapture(Integer idCapture) {
        this.idCapture = idCapture;
    }

    public Date getStartedTime() {
        return startedTime;
    }

    public void setStartedTime(Date startedTime) {
        this.startedTime = startedTime;
    }

    public Date getFinishTime() {
        return finishTime;
    }

    public void setFinishTime(Date finishTime) {
        this.finishTime = finishTime;
    }

    public Integer getNOccupiedLots() {
        return nOccupiedLots;
    }

    public void setNOccupiedLots(Integer nOccupiedLots) {
        this.nOccupiedLots = nOccupiedLots;
    }

    public Boolean getValid() {
        return valid;
    }

    public void setValid(Boolean valid) {
        this.valid = valid;
    }

    @XmlTransient
    public Collection<Photo> getPhotoCollection() {
        return photoCollection;
    }

    public void setPhotoCollection(Collection<Photo> photoCollection) {
        this.photoCollection = photoCollection;
    }

    public Park getParkidPark() {
        return parkidPark;
    }

    public void setParkidPark(Park parkidPark) {
        this.parkidPark = parkidPark;
    }

    public Drone getDroneidDrone() {
        return droneidDrone;
    }

    public void setDroneidDrone(Drone droneidDrone) {
        this.droneidDrone = droneidDrone;
    }

    @Override
    public int hashCode() {
        int hash = 0;
        hash += (idCapture != null ? idCapture.hashCode() : 0);
        return hash;
    }

    @Override
    public boolean equals(Object object) {
        // TODO: Warning - this method won't work in the case the id fields are not set
        if (!(object instanceof Capture)) {
            return false;
        }
        Capture other = (Capture) object;
        if ((this.idCapture == null && other.idCapture != null) || (this.idCapture != null && !this.idCapture.equals(other.idCapture))) {
            return false;
        }
        return true;
    }

    @Override
    public String toString() {
        return "server.domain.Capture[ idCapture=" + idCapture + " ]";
    }
    
}
