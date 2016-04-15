/********************
*AGV自动导航软件
* Author:Bo.Z
* Date:20160304
*********************/


#ifndef MYIMAGEWIDGET_H
#define MYIMAGEWIDGET_H

#include <QWidget>
#include <QPoint>

namespace Ui {
class MyImageWidget;
}

class MyImageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MyImageWidget(QWidget *parent = 0);
    ~MyImageWidget();

private:
    Ui::MyImageWidget *ui;

public:
    void zoomIn(double factor);
    void zoomOut(double factor);
    void fitToWindow();
    void normalSize();
    void moveImage(int x,int y);

    void showCarFlag(bool flag);
    void drawCar(int x,int y,double sita);
    void showTargetFlag(bool flag);
    void drawTarget(int x,int y);
    void showNaviRouteFlag(bool flag);
    void drawNaviRoute(QPoint *pPoints,int nNumber);

    void showLineFlag(bool flag);
    void drawMoveLine(int x,int y);
    void clearMoveLine();

    void openImage(QPixmap pixmap);

    int     getBeginX()const {return nX;}
    int     getBeginY()const {return nY;}
    int     getCurrentWidth(){return nCurrentWidth;}
    int     getCurrentHeigh(){return nCurrentHeight;}
    QPixmap getPixMap(){return pPixMap;}
    bool    getShowCarFlag(){return bShowCarFlag;}
    bool    getShowLineFlag(){return bShowLineFlag;}
    int     getCarX(){return nCarX;}
    int     getCarY(){return nCarY;}
    double  getCarSita(){return dCarSita;}
    int     getTargetX(){return nTargetX;}
    int     getTargetY(){return nTargetY;}

protected:
    void setBeginX(int x);
    void setBeginY(int y);
    void setCurrentWidth(int w);
    void setCurrentHeight(int h);
    void setPixMap(QPixmap pixmap);

    void setCarX(int x);
    void setCarY(int y);
    void setCarSita(double d);
    void setTargetX(int x);
    void setTargetY(int y);

protected:
    void paintEvent(QPaintEvent *);


protected:
    QPixmap pPixMap;
    int nX;
    int nY;
    int nCurrentWidth;
    int nCurrentHeight;
    int nImageWidth;
    int nImageHeight;

    int nCarX;
    int nCarY;
    double dCarSita;
    int nTargetX;
    int nTargetY;

    bool bShowCarFlag;
    bool bShowTargetFlag;
    bool bShowNaviRouteFlag;
    bool bShowLineFlag;

    QPoint *moveLinePoints;
    int nMoveLinePointsNumber;

    QPoint *naviRoutePoints;
    int nNaviRoutePointsNumber;

    double scaleFactor;

private:
    void scaleImage(double factor);
    void painterDrawCar(QPainter &paint);
    void painterDrawTarget(QPainter &paint);
    void painterDrawNaveRoute(QPainter &paint);


};

#endif // MYIMAGEWIDGET_H
