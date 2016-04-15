/********************
*AGV自动导航软件
* Author:Bo.Z
* Date:20160304
*********************/


#include "myimagewidget.h"
#include "ui_myimagewidget.h"
#include <QPainter>
#include <math.h>

MyImageWidget::MyImageWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyImageWidget)
{
    ui->setupUi(this);

    pPixMap = QPixmap();

    nX = 0;
    nY = 0;
    nCurrentWidth = 0;
    nCurrentHeight = 0;
    nImageWidth = 0;
    nImageHeight = 0;
    nCarX = 0;
    nCarY = 0;
    dCarSita = 0.0;
    nTargetX = 100;
    nTargetY = 100;
    bShowCarFlag = false;
    bShowTargetFlag = false;
    bShowNaviRouteFlag = false;
    bShowLineFlag = false;

    moveLinePoints = new QPoint[this->width()*this->height()];
    nMoveLinePointsNumber = 0;    

    naviRoutePoints = new QPoint[this->width()*this->height()];
    nNaviRoutePointsNumber = 0;

    scaleFactor = 1.0;
}

MyImageWidget::~MyImageWidget()
{
    delete ui;
}

void MyImageWidget::zoomIn(double factor)
{
    scaleImage(factor);
}

void MyImageWidget::zoomOut(double factor)
{
    scaleImage(factor);
}

void MyImageWidget::fitToWindow()
{
    setBeginX(0);
    setBeginY(0);
    setCurrentWidth(this->width());
    setCurrentHeight(this->height());
}

void MyImageWidget::normalSize()
{
    setBeginX(0);
    setBeginY(0);
    setCurrentWidth(nImageWidth);
    setCurrentHeight(nImageHeight);
    scaleFactor = 1.0;
}

void MyImageWidget::moveImage(int x, int y)
{
    setBeginX(nX+x);
    setBeginY(nY+y);
    update();
}

void MyImageWidget::showCarFlag(bool flag)
{
    bShowCarFlag = flag;    
    update();
}

void MyImageWidget::drawCar(int x, int y, double sita)
{
    setCarX(x);
    setCarY(y);
    setCarSita(sita);
    update();
}

void MyImageWidget::showTargetFlag(bool flag)
{
    bShowTargetFlag = flag;
    update();
}

void MyImageWidget::drawTarget(int x, int y)
{
    setTargetX(x);
    setTargetY(y);
    update();
}

void MyImageWidget::showNaviRouteFlag(bool flag)
{
    bShowNaviRouteFlag = flag;
    update();
}

void MyImageWidget::drawNaviRoute(QPoint *pPoints,int nNumber)
{
    nNaviRoutePointsNumber = nNumber;
    for(int i=0;i<nNumber;i++)
    {
        naviRoutePoints[i] = pPoints[i];
    }
    update();
}

void MyImageWidget::showLineFlag(bool flag)
{
    bShowLineFlag = flag;
    update();
}

void MyImageWidget::drawMoveLine(int x, int y)
{
    moveLinePoints[nMoveLinePointsNumber] = QPoint(x,y);
    nMoveLinePointsNumber++;
    update();
}

void MyImageWidget::clearMoveLine()
{
    if(moveLinePoints!=NULL)
    {
        delete moveLinePoints;
        moveLinePoints = NULL;
    }
    if(nImageWidth<=0)
    {
        moveLinePoints = new QPoint[this->width()*this->height()];

    }
    else
    {
        moveLinePoints = new QPoint[nImageWidth*nImageWidth];
    }
    nMoveLinePointsNumber = 0;
    update();
}

void MyImageWidget::openImage(QPixmap pixmap)
{
    nImageWidth = pixmap.width();
    nImageHeight = pixmap.height();
    setCurrentWidth(nImageWidth);
    setCurrentHeight(nImageHeight);
    setBeginX(0);
    setBeginY(0);
    setPixMap(pixmap);

    clearMoveLine();

    update();
}

void MyImageWidget::setBeginX(int x)
{
    nX = x;
    update();
}

void MyImageWidget::setBeginY(int y)
{
    nY = y;
    update();
}

void MyImageWidget::setCurrentWidth(int w)
{
    nCurrentWidth = w;
    update();
}

void MyImageWidget::setCurrentHeight(int h)
{
    nCurrentHeight = h;
    update();
}

void MyImageWidget::setPixMap(QPixmap pixmap)
{
    pPixMap =  pixmap ;
    update();
}

void MyImageWidget::setCarX(int x)
{
    nCarX = x;
    update();
}

void MyImageWidget::setCarY(int y)
{
    nCarY = y;
    update();
}

void MyImageWidget::setCarSita(double sita)
{
    dCarSita = sita;
    update();
}

void MyImageWidget::setTargetX(int x)
{
    nTargetX = x;
    update();
}

void MyImageWidget::setTargetY(int y)
{
    nTargetY = y;
    update();
}

void MyImageWidget::paintEvent(QPaintEvent *)
{
    QPainter myPainter(this);
    myPainter.drawPixmap(nX,nY,nCurrentWidth,nCurrentHeight,pPixMap);
    if(bShowLineFlag==true)
    {
        myPainter.setPen(QPen(Qt::blue, 3));
        myPainter.drawPolyline(moveLinePoints,nMoveLinePointsNumber);
    }
    if(bShowCarFlag==true)
    {
        painterDrawCar(myPainter);
    }
    if(bShowTargetFlag==true)
    {
        painterDrawTarget(myPainter);
    }    
    if(bShowNaviRouteFlag==true)
    {
        painterDrawNaveRoute(myPainter);
    }
}

void MyImageWidget::scaleImage(double factor)
{
    Q_ASSERT(pPixMap);
    scaleFactor *= factor;

    setCurrentWidth(scaleFactor * nImageWidth);
    setCurrentHeight(scaleFactor * nImageHeight);

    update();
}

void MyImageWidget::painterDrawCar(QPainter &paint)
{
    paint.setPen(QPen(Qt::black, 1));
    paint.setBrush(Qt::black);
    paint.drawEllipse(QPoint(nCarX,nCarY),7,7);


    QPoint points[3];
    double rdx,rdy,dx0,dy0,dx1,dy1,dx2,dy2;
    double pi = 3.141592653;

    dx0 = 21*sin((dCarSita/180)*pi);
    dy0 = 21*cos((dCarSita/180)*pi);
    dx2 = 14*sin((dCarSita/180)*pi);
    dy2 = 14*cos((dCarSita/180)*pi);
    rdx = 11*sin((dCarSita/180)*pi);
    rdy = 11*cos((dCarSita/180)*pi);
    dx1 = 7*cos((dCarSita/180)*pi);
    dy1 = 7*sin((dCarSita/180)*pi);

    points[0]=QPoint(nCarX+rdx-dx1,nCarY-rdy-dy1);
    points[1]=QPoint(nCarX+dx0,nCarY-dy0);
    points[2]=QPoint(nCarX+rdx+dx1,nCarY-rdy+dy1);
    points[3]=QPoint(nCarX+dx2,nCarY-dy2);

    paint.drawPolygon(points,4);

//    //绘制小车图标
//    QImage *myImage = new QImage;
//    QString strPath = QApplication::applicationDirPath()+"/car.png";
//    myImage->load(strPath);
//    QPixmap myPixmap = QPixmap::fromImage(*myImage).scaled(myImage->width()/5,myImage->height()/5);
//    QMatrix myMatrix;
//    myMatrix.rotate(dCarSita);
//    myPixmap=myPixmap.transformed(myMatrix);
//    switch((int)dCarSita)
//    {
//    case 0:
//        paint.drawPixmap(nCarX-myImage->width()/10,nCarY-myImage->height()/10,myPixmap);
//        break;
//    case 45:
//        paint.drawPixmap(nCarX-myImage->width()/7,nCarY-myImage->height()/7,myPixmap);
//        break;
//    case 90:
//        paint.drawPixmap(nCarX-myImage->height()/10,nCarY-myImage->width()/10,myPixmap);
//        break;
//    case 135:
//        paint.drawPixmap(nCarX-myImage->width()/7,nCarY-myImage->width()/7,myPixmap);
//        break;
//    case 180:
//        paint.drawPixmap(nCarX-myImage->width()/10,nCarY-myImage->height()/10,myPixmap);
//        break;
//    case 225:
//        paint.drawPixmap(nCarX-myImage->width()/7,nCarY-myImage->height()/7,myPixmap);
//        break;
//    case 270:
//        paint.drawPixmap(nCarX-myImage->height()/10,nCarY-myImage->width()/10,myPixmap);
//        break;
//    case 315:
//        paint.drawPixmap(nCarX-myImage->height()/7,nCarY-myImage->height()/7,myPixmap);
//        break;
//    default:
//        paint.drawPixmap(nCarX,nCarY,myPixmap);
//        break;
//    }
}

void MyImageWidget::painterDrawTarget(QPainter &paint)
{
    paint.setPen(QPen(Qt::gray, 1));
    paint.setBrush(Qt::gray);
    paint.drawEllipse(QPoint(nTargetX,nTargetY),5,2);
    paint.setPen(QPen(Qt::red, 1));
    paint.setBrush(Qt::red);
    paint.drawLine(QPoint(nTargetX,nTargetY),QPoint(nTargetX,nTargetY-8));
    QPoint points[3];
    points[0]=QPoint(nTargetX,nTargetY-8);
    points[1]=QPoint(nTargetX,nTargetY-20);
    points[2]=QPoint(nTargetX+11,nTargetY-14);
    paint.drawPolygon(points,3);
}

void MyImageWidget::painterDrawNaveRoute(QPainter &paint)
{
    paint.setPen(QPen(Qt::green, 3));
    paint.setBrush(Qt::green);
    paint.drawPolyline(naviRoutePoints,nNaviRoutePointsNumber);
}
