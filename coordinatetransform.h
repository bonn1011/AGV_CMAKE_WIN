/********************
*AGV自动导航软件
* Author:Bo.Z
* Date:20160304
*********************/


#ifndef COORDINATETRANSFORM_H
#define COORDINATETRANSFORM_H


class CoordinateTransform
{
public:
    CoordinateTransform();

public:
    void setOriginGeoCoord(double dLon, double dLat, double dResolution);
    void pixelPosToGeoCoord(double &dLon,double &dLat,int nPixelX,int nPixelY);
    void geoCoordToPixelPos(int &nPixelX,int &nPixelY,double dLon,double dLat);

private:
    double m_OriginPixelLongitude;
    double m_OriginPixelLatitude;
    double m_Resolution;
};

#endif // COORDINATETRANSFORM_H
