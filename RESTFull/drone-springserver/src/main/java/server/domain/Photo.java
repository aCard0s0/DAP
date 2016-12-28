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
@Table(name = "photo")
@XmlRootElement
@NamedQueries({
    @NamedQuery(name = "Photo.findAll", query = "SELECT p FROM Photo p"),
    @NamedQuery(name = "Photo.findByIdPhoto", query = "SELECT p FROM Photo p WHERE p.idPhoto = :idPhoto"),
    @NamedQuery(name = "Photo.findBySrc", query = "SELECT p FROM Photo p WHERE p.src = :src"),
    @NamedQuery(name = "Photo.findByLatitude", query = "SELECT p FROM Photo p WHERE p.latitude = :latitude"),
    @NamedQuery(name = "Photo.findByLongitude", query = "SELECT p FROM Photo p WHERE p.longitude = :longitude"),
    @NamedQuery(name = "Photo.findByHeigth", query = "SELECT p FROM Photo p WHERE p.heigth = :heigth")})
public class Photo implements Serializable {

    private static final long serialVersionUID = 1L;
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    @Basic(optional = false)
    @Column(name = "idPhoto")
    private Integer idPhoto;
    @Size(max = 100)
    @Column(name = "src")
    private String src;
    @Size(max = 45)
    @Column(name = "latitude")
    private String latitude;
    @Size(max = 45)
    @Column(name = "longitude")
    private String longitude;
    // @Max(value=?)  @Min(value=?)//if you know range of your decimal fields consider using these annotations to enforce field validation
    @Column(name = "heigth")
    private Float heigth;
    
    @JoinColumn(name = "Capture_idCapture", referencedColumnName = "idCapture")
    @JsonIgnore
    @ManyToOne(optional = false)
    private Capture captureidCapture;

    public Photo() {
    }

    public Photo(Integer idPhoto) {
        this.idPhoto = idPhoto;
    }

    public Integer getIdPhoto() {
        return idPhoto;
    }

    public void setIdPhoto(Integer idPhoto) {
        this.idPhoto = idPhoto;
    }

    public String getSrc() {
        return src;
    }

    public void setSrc(String src) {
        this.src = src;
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

    public Float getHeigth() {
        return heigth;
    }

    public void setHeigth(Float heigth) {
        this.heigth = heigth;
    }

    public Capture getCaptureidCapture() {
        return captureidCapture;
    }

    public void setCaptureidCapture(Capture captureidCapture) {
        this.captureidCapture = captureidCapture;
    }

    @Override
    public int hashCode() {
        int hash = 0;
        hash += (idPhoto != null ? idPhoto.hashCode() : 0);
        return hash;
    }

    @Override
    public boolean equals(Object object) {
        // TODO: Warning - this method won't work in the case the id fields are not set
        if (!(object instanceof Photo)) {
            return false;
        }
        Photo other = (Photo) object;
        if ((this.idPhoto == null && other.idPhoto != null) || (this.idPhoto != null && !this.idPhoto.equals(other.idPhoto))) {
            return false;
        }
        return true;
    }

    @Override
    public String toString() {
        return "server.domain.Photo[ idPhoto=" + idPhoto + " ]";
    }
    
}
