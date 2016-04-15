#include "coordinatetransform.h"

CoordinateTransform::CoordinateTransform()
{
    m_OriginPixelLongitude = -10;
    m_OriginPixelLatitude = -10;
    m_Resolution = 0.5;
}

void CoordinateTransform::setOriginGeoCoord(double dLon, double dLat, double dResolution)
{
    m_OriginPixelLongitude = dLon;
    m_OriginPixelLatitude = dLat;
    m_Resolution = dResolution;
}

void CoordinateTransform::pixelPosToGeoCoord(double &dLon, double &dLat, int nPixelX, int nPixelY)
{
    dLon = m_OriginPixelLongitude+nPixelX*m_Resolution;
    dLat = m_OriginPixelLatitude+nPixelY*m_Resolution;
}

void CoordinateTransform::geoCoordToPixelPos(int &nPixelX, int &nPixelY, double dLon, double dLat)
{
    double dCol = 0.0, dRow = 0.0;
    dCol = (dLon - m_OriginPixelLongitude) / m_Resolution + 0.5;
    dRow = (dLat - m_OriginPixelLatitude) / m_Resolution + 0.5;
    nPixelX = int(dCol);
    nPixelY = int(dRow);
}

