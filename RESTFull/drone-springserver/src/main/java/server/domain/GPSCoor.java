/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package server.domain;

import com.fasterxml.jackson.annotation.JsonIgnore;
import java.io.Serializable;
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
import javax.validation.constraints.Size;
import javax.xml.bind.annotation.XmlRootElement;

/**
 *
 * @author acardoso
 */
@Entity
@Table(name = "gpscoor")
@XmlRootElement
@NamedQueries({
    @NamedQuery(name = "GPSCoor.findAll", query = "SELECT g FROM GPSCoor g"),
    @NamedQuery(name = "GPSCoor.findByIdGPSCoor", query = "SELECT g FROM GPSCoor g WHERE g.idGPSCoor = :idGPSCoor"),
    @NamedQuery(name = "GPSCoor.findByIndexItem", query = "SELECT g FROM GPSCoor g WHERE g.indexItem = :indexItem"),
    @NamedQuery(name = "GPSCoor.findByLatitude", query = "SELECT g FROM GPSCoor g WHERE g.latitude = :latitude"),
    @NamedQuery(name = "GPSCoor.findByLongitude", query = "SELECT g FROM GPSCoor g WHERE g.longitude = :longitude")})
public class GPSCoor implements Serializable {

    private static final long serialVersionUID = 1L;
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    @Basic(optional = false)
    @Column(name = "idGPSCoor")
    private Integer idGPSCoor;
    @Column(name = "indexItem")
    private Integer indexItem;
    @Size(max = 45)
    @Column(name = "latitude")
    private String latitude;
    @Size(max = 45)
    @Column(name = "longitude")
    private String longitude;
    
    @JoinColumn(name = "Park_idPark", referencedColumnName = "idPark")
    @ManyToOne(optional = false)
    @JsonIgnore
    private Park parkidPark;

    public GPSCoor() {
    }

    public GPSCoor(Integer idGPSCoor) {
        this.idGPSCoor = idGPSCoor;
    }

    public Integer getIdGPSCoor() {
        return idGPSCoor;
    }

    public void setIdGPSCoor(Integer idGPSCoor) {
        this.idGPSCoor = idGPSCoor;
    }

    public Integer getIndex() {
        return indexItem;
    }

    public void setIndex(Integer index) {
        this.indexItem = index;
    }

    public String getLatitude() {
        return latitude;
    }

    public void setLatitude(String latitude) {
        this.latitude = latitude;
    }

    public String getLongitude() {
        return longitude;
    }

    public void setLongitude(String longitude) {
        this.longitude = longitude;
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
        hash += (idGPSCoor != null ? idGPSCoor.hashCode() : 0);
        return hash;
    }

    @Override
    public boolean equals(Object object) {
        // TODO: Warning - this method won't work in the case the id fields are not set
        if (!(object instanceof GPSCoor)) {
            return false;
        }
        GPSCoor other = (GPSCoor) object;
        if ((this.idGPSCoor == null && other.idGPSCoor != null) || (this.idGPSCoor != null && !this.idGPSCoor.equals(other.idGPSCoor))) {
            return false;
        }
        return true;
    }

    @Override
    public String toString() {
        return "server.domain.GPSCoor[ idGPSCoor=" + idGPSCoor + " ]";
    }
    
}
