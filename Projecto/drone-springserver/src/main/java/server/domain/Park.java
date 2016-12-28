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
@Table(name = "park")
@XmlRootElement
@NamedQueries({
    @NamedQuery(name = "Park.findAll", query = "SELECT p FROM Park p"),
    @NamedQuery(name = "Park.findByIdPark", query = "SELECT p FROM Park p WHERE p.idPark = :idPark"),
    @NamedQuery(name = "Park.findByName", query = "SELECT p FROM Park p WHERE p.name = :name"),
    @NamedQuery(name = "Park.findByNLots", query = "SELECT p FROM Park p WHERE p.nLots = :nLots"),
    @NamedQuery(name = "Park.findByCentraLatitude", query = "SELECT p FROM Park p WHERE p.centraLatitude = :centraLatitude"),
    @NamedQuery(name = "Park.findByCentraLongitude", query = "SELECT p FROM Park p WHERE p.centraLongitude = :centraLongitude"),
    @NamedQuery(name = "Park.findByActive", query = "SELECT p FROM Park p WHERE p.active = :active")})
public class Park implements Serializable {

    @OneToMany(mappedBy = "parkidPark")
    @JsonIgnore
    private Collection<Logger> loggerCollection;

    private static final long serialVersionUID = 1L;
    
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    @Basic(optional = false)
    @Column(name = "idPark")
    private Integer idPark;
    
    @Size(max = 45)
    @Column(name = "name")
    private String name;
    
    @Column(name = "nLots")
    private Integer nLots;
    
    @Size(max = 45)
    @Column(name = "centraLatitude")
    private String centraLatitude;
    
    @Size(max = 45)
    @Column(name = "centraLongitude")
    private String centraLongitude;
    
    @Column(name = "active")
    private Boolean active;
    
    @Column(name = "type")
    private String type;

    public String getType() {
        return type;
    }

    public void setType(String type) {
        this.type = type;
    }

    public String getParkImg() {
        return parkImg;
    }

    public void setParkImg(String parkImg) {
        this.parkImg = parkImg;
    }
    
    @Column(name = "parkImg")
    private String parkImg;
                    
    @OneToMany(cascade = CascadeType.REMOVE , mappedBy = "parkidPark")
    private Collection<GPSCoor> gPSCoorCollection;
    
    @OneToMany(cascade = CascadeType.REMOVE , mappedBy = "parkidPark")
    @JsonIgnore
    private Collection<Capture> captureCollection;

    public Park() {
    }

  
    
    public Park(Integer idPark) {
        this.idPark = idPark;
    }

    public Integer getIdPark() {
        return idPark;
    }

    public void setIdPark(Integer idPark) {
        this.idPark = idPark;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public Integer getNLots() {
        return nLots;
    }

    public void setNLots(Integer nLots) {
        this.nLots = nLots;
    }

    public String getCentraLatitude() {
        return centraLatitude;
    }

    public void setCentraLatitude(String centraLatitude) {
        this.centraLatitude = centraLatitude;
    }

    public String getCentraLongitude() {
        return centraLongitude;
    }

    public void setCentraLongitude(String centraLongitude) {
        this.centraLongitude = centraLongitude;
    }

    public Boolean getActive() {
        return active;
    }

    public void setActive(Boolean active) {
        this.active = active;
    }

    @XmlTransient
    public Collection<GPSCoor> getGPSCoorCollection() {
        return gPSCoorCollection;
    }

    public void setGPSCoorCollection(Collection<GPSCoor> gPSCoorCollection) {
        this.gPSCoorCollection = gPSCoorCollection;
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
        hash += (idPark != null ? idPark.hashCode() : 0);
        return hash;
    }

    @Override
    public boolean equals(Object object) {
        // TODO: Warning - this method won't work in the case the id fields are not set
        if (!(object instanceof Park)) {
            return false;
        }
        Park other = (Park) object;
        if ((this.idPark == null && other.idPark != null) || (this.idPark != null && !this.idPark.equals(other.idPark))) {
            return false;
        }
        return true;
    }

    @Override
    public String toString() {
        return "server.domain.Park[ idPark=" + idPark + " ]";
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
