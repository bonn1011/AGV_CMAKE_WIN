/********************
*AGV自动导航软件
* Author:Bo.Z
* Date:20160304
*********************/


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"

#include <QtXml>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextCodec>
#include <QPainter>
#include <math.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    pMyCoordTrans = new CoordinateTransform();    
    pVGA = new VgaCarFunctionThread();
    pVGA->start();

    nImageWidth= 0;
    nImageHeight= 0;
    bIsImageMoveFlag = false;
    bIsSetInitPosFlag = false;
    bIsSetTargetPosFlag = false;

    nLeftButtonPressPosX = -1;
    nLeftButtonPressPosY = -1;

    key1 = 0;
    key2 = 0;

    joystick.m_hWnd = (HWND)this->winId(); //首先获得窗口句柄
    if(!joystick.Initialise())//初始化
    {
        qDebug("游戏手柄初始化失败！");
    }
    startTimer(100);

    setWindow();
    setNormalActions(false);
    setMapStudyActions(false);
    setNavigationActions(false);
    setConnect();
    setVgaConnect();
}

MainWindow::~MainWindow()
{
    delete ui;
    if(pMyCoordTrans!=NULL)
    {
        delete pMyCoordTrans;
        pMyCoordTrans = NULL;
    }
    if(pMyImageWidget!=NULL)
    {
        delete pMyImageWidget;
        pMyImageWidget = NULL;
    }
    pVGA->terminate();
}

void MainWindow::setNormalActions(bool YorN)
{
    ui->actionClose->setEnabled(YorN);
    ui->actionSave->setEnabled(YorN);
    ui->actionMove->setEnabled(YorN);
    ui->actionZoomIn->setEnabled(YorN);
    ui->actionZoomOut->setEnabled(YorN);
    ui->actionFit->setEnabled(YorN);
    ui->actionNormal->setEnabled(YorN);
//    ui->actionShowCar->setEnabled(YorN);
//    ui->actionShowLine->setEnabled(YorN);
}

void MainWindow::setMapStudyActions(bool YorN)
{
    ui->actionStartStudy->setEnabled(YorN);
    ui->actionStopStudy->setEnabled(YorN);
    ui->actionLeft->setEnabled(YorN);
    ui->actionRight->setEnabled(YorN);
    ui->actionFront->setEnabled(YorN);
    ui->actionBack->setEnabled(YorN);
    ui->actionLeftFront->setEnabled(YorN);
    ui->actionRightFront->setEnabled(YorN);
    ui->actionLeftBack->setEnabled(YorN);
    ui->actionRightBack->setEnabled(YorN);
}

void MainWindow::setNavigationActions(bool YorN)
{
    ui->actionStartNavi->setEnabled(YorN);
    ui->actionStopNavi->setEnabled(YorN);
    ui->actionSetTarget->setEnabled(YorN);
    ui->actionInitCarPos->setEnabled(YorN);
}

void MainWindow::setWindow()
{    
    pMyImageWidget = new MyImageWidget();
    setCentralWidget(pMyImageWidget);

    ui->actionMove->setCheckable(true);
    ui->actionMove->setChecked(false);
    ui->actionNavigationMode->setChecked(true);

    QTextCodec::setCodecForLocale( QTextCodec::codecForName("GBK") );
    msgLabel=new QLabel();
    ui->statusBar->addWidget(msgLabel);
    xyLabel=new QLabel();
    ui->statusBar->addPermanentWidget(xyLabel);
    progressBar = new QProgressBar();
    progressBar->setHidden(true);
    progressBar->setTextVisible( false );
    progressBar->setRange(0,100);
    ui->statusBar->addWidget(progressBar,1);

    msgLabel->setText("Ready...");

    pMyImageWidget->setMouseTracking(true);
    centralWidget()->setMouseTracking(true);
    setMouseTracking(true);
}

void MainWindow::setConnect()
{
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(slot_open()));
    connect(ui->actionClose, SIGNAL(triggered()), this, SLOT(slot_close()));
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(slot_save()));
    connect(ui->actionMapStudyMode, SIGNAL(triggered()), this, SLOT(slot_mapstudymode()));
    connect(ui->actionNavigationMode, SIGNAL(triggered()), this, SLOT(slot_navigationmode()));
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));

    connect(ui->actionMove, SIGNAL(triggered()), this, SLOT(slot_move()));
    connect(ui->actionZoomIn, SIGNAL(triggered()), this, SLOT(slot_zoomin()));
    connect(ui->actionZoomOut, SIGNAL(triggered()), this, SLOT(slot_zoomout()));
    connect(ui->actionFit, SIGNAL(triggered()), this, SLOT(slot_fit()));
    connect(ui->actionNormal, SIGNAL(triggered()), this, SLOT(slot_normal()));
    connect(ui->actionShowCar, SIGNAL(triggered()), this, SLOT(slot_showcar()));
    connect(ui->actionShowLine, SIGNAL(triggered()), this, SLOT(slot_showline()));

    connect(ui->actionStartStudy, SIGNAL(triggered()), this, SLOT(slot_startstudy()));
    connect(ui->actionStopStudy, SIGNAL(triggered()), this, SLOT(slot_stopstudy()));
    connect(ui->actionLeft, SIGNAL(triggered()), this, SLOT(slot_turnleft()));
    connect(ui->actionRight, SIGNAL(triggered()), this, SLOT(slot_turnright()));
    connect(ui->actionFront, SIGNAL(triggered()), this, SLOT(slot_turnfront()));
    connect(ui->actionBack, SIGNAL(triggered()), this, SLOT(slot_turnback()));
    connect(ui->actionLeftFront, SIGNAL(triggered()), this, SLOT(slot_turnleftfront()));
    connect(ui->actionRightFront, SIGNAL(triggered()), this, SLOT(slot_turnrightfront()));
    connect(ui->actionLeftBack, SIGNAL(triggered()), this, SLOT(slot_turnleftback()));
    connect(ui->actionRightBack, SIGNAL(triggered()), this, SLOT(slot_turnrightback()));

    connect(ui->actionStartNavi, SIGNAL(triggered()), this, SLOT(slot_startnavi()));
    connect(ui->actionStopNavi, SIGNAL(triggered()), this, SLOT(slot_stopnavi()));
    connect(ui->actionSetTarget, SIGNAL(triggered()), this, SLOT(slot_settarget()));
    connect(ui->actionInitCarPos, SIGNAL(triggered()), this, SLOT(slot_setcarinitposition()));

    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(slot_about()));

}

void MainWindow::setVgaConnect()
{
    connect(pVGA,SIGNAL(agv_LoadMapFile(QString, QString)),this,SLOT(agv_LoadMapFile(QString, QString)));
    connect(pVGA,SIGNAL(agv_LoadMapData(const unsigned char*,int,double,double,double)),this,SLOT(agv_LoadMapData(const unsigned char*,int,double,double,double)));
    connect(pVGA,SIGNAL(agv_SetCarCurrentPosition(double,double,double)),this,SLOT(agv_SetCarCurrentPosition(double,double,double)));
    connect(pVGA,SIGNAL(agv_ShowMessage(QString)),this,SLOT(agv_ShowMessage(QString)));
    connect(pVGA,SIGNAL(agv_SetNaviRoute(double*,double*,int)),this,SLOT(agv_SetNaviRoute(double*,double*,int)));

    connect(this, SIGNAL(agv_StartMapStudy()), pVGA, SLOT(agv_StartMapStudy()));
    connect(this, SIGNAL(agv_StopMapStudy()), pVGA, SLOT(agv_StopMapStudy()));
    connect(this, SIGNAL(agv_SetCarDirection(DIRECT)), pVGA, SLOT(agv_SetCarDirection(DIRECT)));
    connect(this, SIGNAL(agv_SetCarInitPoistion(double,double,double)), pVGA, SLOT(agv_SetCarInitPoistion(double,double,double)));
    connect(this, SIGNAL(agv_SetNaviTargetPoistion(double,double)), pVGA, SLOT(agv_SetNaviTargetPoistion(double,double)));
    connect(this, SIGNAL(agv_StartNavigation()), pVGA, SLOT(agv_StartNavigation()));
    connect(this, SIGNAL(agv_StopNavigation()), pVGA, SLOT(agv_StopNavigation()));
}

int MainWindow::showImageFunc(QString strImageFile)
{
    if(strImageFile.isEmpty())
    {
        showMassageFunc(tr("路径为空！"));
    }

    QImage *qImg=new QImage;

    if(! ( qImg->load(strImageFile) ) ) //加载图像
    {
        showMassageFunc(tr("打开图像失败！"));
        delete qImg;
        pMyImageWidget->openImage(QPixmap());
        return -1;
    }

    nImageWidth= qImg->width();
    nImageHeight= qImg->height();
    pMyImageWidget->openImage(QPixmap::fromImage(*qImg));

    return 0;
}

int MainWindow::readImageMatedata(QString strMatadataFile)
{
    QString dLon,dLat,dReso;

    QDomDocument doc("mydocument");
    QFile file(strMatadataFile);

    if (!file.open(QIODevice::ReadOnly))
    {
        return -1;
    }
    if (!doc.setContent(&file))
    {
        file.close();
        return -1;
    }
    file.close();

    QDomElement docElem = doc.documentElement();

    QDomNodeList nodeList = docElem.childNodes();
    for(int i=0;i<nodeList.length();i++)
    {
        QDomElement element = nodeList.at(i).toElement();
        if(!element.isNull())
        {
            if(element.tagName()=="RealCoordX")
            {
                dLon = element.text();
            }
            if(element.tagName()=="RealCoordY")
            {
                dLat = element.text();
            }
            if(element.tagName()=="PixelResolution")
            {
                dReso = element.text();
            }
        }
    }

    pMyCoordTrans->setOriginGeoCoord(dLon.toInt(),dLat.toInt(),dReso.toDouble());

    return 0;
}

void MainWindow::showMassageFunc(QString strMsg)
{
    if(strMsg.isEmpty())
    {
        QMessageBox::warning(this,tr("Warning..."),tr("窗口有空白消息弹出！"));
        return;
    }
    QMessageBox::warning(this,tr("Warning..."),strMsg);
}

int MainWindow::saveImageFunc(QString strImageFile)
{
    //Q_ASSERT(pMyImageWidget->getPixMap());
    //return pMyImageWidget->getPixMap().save(strImageFile,0,50);

    int nGrabX,nGrabY,nGrabW,nGrabH;

    if(pMyImageWidget->getBeginX()>=0)
    {
        nGrabX = pMyImageWidget->getBeginX();
    }
    else
    {
        nGrabX = 0;
    }
    if(pMyImageWidget->getBeginY()>=0)
    {
        nGrabY = pMyImageWidget->getBeginY();
    }
    else
    {
        nGrabY = 0;
    }
    if(pMyImageWidget->width()<=pMyImageWidget->getCurrentWidth())
    {
        nGrabW = pMyImageWidget->width()-nGrabX;
    }
    else
    {
        nGrabW = pMyImageWidget->getCurrentWidth()-nGrabX;
    }
    if(pMyImageWidget->height()<=pMyImageWidget->getCurrentHeigh())
    {
        nGrabH = pMyImageWidget->height()-nGrabY;
    }
    else
    {
        nGrabH = pMyImageWidget->getCurrentHeigh()-nGrabY;
    }

    QPixmap pmap = pMyImageWidget->grab(QRect(nGrabX,nGrabY,nGrabW,nGrabH));
    return pmap.save(strImageFile,0,50);
}

int MainWindow::getImageX(QPoint point)
{
    int nWidgetPosX = pMyImageWidget->mapFromParent(point).x();
    double nScaleRatioX = double(pMyImageWidget->getCurrentWidth())/nImageWidth;
    return (nWidgetPosX-pMyImageWidget->getBeginX())/nScaleRatioX;
}

int MainWindow::getImageY(QPoint point)
{
    int nWidgetPosY = pMyImageWidget->mapFromParent(point).y();
    double nScaleRatioY = double(pMyImageWidget->getCurrentHeigh())/nImageHeight;
    return nImageHeight-(nWidgetPosY-pMyImageWidget->getBeginY())/nScaleRatioY;;
}


void MainWindow::mousePressEvent(QMouseEvent *event)
{
    int nCurrentPosX = pMyImageWidget->mapFromParent(event->pos()).x();
    int nCurrentPosY = pMyImageWidget->mapFromParent(event->pos()).y();

    if(pMyImageWidget->getCurrentWidth()>0)
    {
        if(event->button()==Qt::LeftButton)
        {
            nLeftButtonPressPosX = nCurrentPosX;
            nLeftButtonPressPosY = nCurrentPosY;
        }
    }    
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    int nCurrentPosX = pMyImageWidget->mapFromParent(event->pos()).x();
    int nCurrentPosY = pMyImageWidget->mapFromParent(event->pos()).y();

    if(pMyImageWidget->getCurrentWidth()>0)
    {        
        QPoint pointTemp = pMyImageWidget->mapToParent(QPoint(nLeftButtonPressPosX,nLeftButtonPressPosY));
        int nImagePosX = getImageX(pointTemp);
        int nImagePosY = getImageY(pointTemp);
        double dLon,dLat;
        pMyCoordTrans->pixelPosToGeoCoord(dLon,dLat,nImagePosX,nImagePosY);

        if(event->button()==Qt::LeftButton)
        {
            if(bIsSetInitPosFlag==true)
            {
                bIsSetInitPosFlag = false;
                this->setCursor(Qt::ArrowCursor);
                double dSita = 0;
                if(nCurrentPosY!=nLeftButtonPressPosY||nCurrentPosX!=nLeftButtonPressPosX)
                {
                    double dT = double(nCurrentPosY-nLeftButtonPressPosY)/(nCurrentPosX-nLeftButtonPressPosX);
                    if(nCurrentPosX-nLeftButtonPressPosX>=0)
                    {
                        dSita = 90+(atan(dT)*180)/3.1416;
                    }
                    else
                    {
                        dSita = 270+(atan(dT)*180)/3.1416;
                    }
                }
                pMyImageWidget->drawCar(nLeftButtonPressPosX,nLeftButtonPressPosY,dSita);
                emit agv_SetCarInitPoistion(dLon,dLat,dSita);

            }

            if(bIsSetTargetPosFlag ==true)
            {
                bIsSetTargetPosFlag = false;
                this->setCursor(Qt::ArrowCursor);
                pMyImageWidget->drawTarget(nLeftButtonPressPosX,nLeftButtonPressPosY);
                pMyImageWidget->showTargetFlag(true);
                emit agv_SetNaviTargetPoistion(dLon,dLat);
            }
        }
    }
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(pMyImageWidget->getCurrentWidth()>0)
    {
        if(event->button()==Qt::LeftButton)
        {

        }
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    int nCurrentPosX = pMyImageWidget->mapFromParent(event->pos()).x();
    int nCurrentPosY = pMyImageWidget->mapFromParent(event->pos()).y();

    if(pMyImageWidget->getCurrentHeigh()>0)
    {
        int nImagePosX = getImageX(event->pos());
        int nImagePosY = getImageY(event->pos());
        double dLon,dLat;
        pMyCoordTrans->pixelPosToGeoCoord(dLon,dLat,nImagePosX,nImagePosY);
        QString str="("+QString::number(dLon)+","+QString::number(dLat)+")";
        xyLabel->setText(str);

        if(event->buttons() & Qt::LeftButton)
        {
            if(bIsImageMoveFlag==true)
            {
                pMyImageWidget->moveImage(nCurrentPosX-nLeftButtonPressPosX,nCurrentPosY-nLeftButtonPressPosY);
                nLeftButtonPressPosX = nCurrentPosX;
                nLeftButtonPressPosY = nCurrentPosY;
            }
        }
    }
}

void MainWindow::wheelEvent(QWheelEvent *event)
{
    if(pMyImageWidget->getCurrentHeigh()>0)
    {
        int numDegrees = event->delta();
        if (event->orientation() == Qt::Vertical)
        {
            if(numDegrees>0)
            {
                pMyImageWidget->zoomIn(1.05);
            }
            else
            {
                pMyImageWidget->zoomOut(0.95);
            }
        }
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(key1==0||key1==event->key())
    {
        if(event->key()==Qt::Key_Left)
        {
            key1 = Qt::Key_Left;
        }
        if(event->key()==Qt::Key_Right)
        {
            key1 = Qt::Key_Right;
        }
        if(event->key()==Qt::Key_Up)
        {
            key1 = Qt::Key_Up;
        }
        if(event->key()==Qt::Key_Down)
        {
            key1 = Qt::Key_Down;
        }
    }
    else if(key2==0||key2==event->key())
    {
        if(event->key()==Qt::Key_Left)
        {
            key2 = Qt::Key_Left;
        }
        if(event->key()==Qt::Key_Right)
        {
            key2 = Qt::Key_Right;
        }
        if(event->key()==Qt::Key_Up)
        {
            key2 = Qt::Key_Up;
        }
        if(event->key()==Qt::Key_Down)
        {
            key2 = Qt::Key_Down;
        }
    }

}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if(key1!=0)
    {
        switch (key1)
        {
        case Qt::Key_Left:
            if(key2!=0)
            {
                switch (key2)
                {
                case Qt::Key_Up:
                    slot_turnleftfront();
                    break;
                case Qt::Key_Down:
                    slot_turnleftback();
                    break;
                default:
                    break;
                }
                key2 = 0;
            }
            else
            {
                slot_turnleft();
            }
            break;
        case Qt::Key_Right:
            if(key2!=0)
            {
                switch (key2)
                {
                case Qt::Key_Up:
                    slot_turnrightfront();
                    break;
                case Qt::Key_Down:
                    slot_turnrightback();
                    break;
                default:
                    break;
                }
                key2 = 0;
            }
            else
            {
                slot_turnright();
            }
            break;
        case Qt::Key_Up:
            if(key2!=0)
            {
                switch (key2)
                {
                case Qt::Key_Left:
                    slot_turnleftfront();
                    break;
                case Qt::Key_Right:
                    slot_turnrightfront();
                    break;
                default:
                    break;
                }
                key2 = 0;
            }
            else
            {
                slot_turnfront();
            }
            break;
        case Qt::Key_Down:
            if(key2!=0)
            {
                switch (key2)
                {
                case Qt::Key_Left:
                    slot_turnleftback();
                    break;
                case Qt::Key_Right:
                    slot_turnrightback();
                    break;
                default:
                    break;
                }
                key2 = 0;
            }
            else
            {
                slot_turnback();
            }
            break;
        default:
            break;
        }
        key1 = 0;
    }
}

void MainWindow::timerEvent(QTimerEvent *e)
{
    if(FAILED( joystick.PollDevice() ) ) //轮循
    {
        qDebug("读取设备状态错误");
    }

    // Display joystick state to dialog

    // Axis
    // Device state text
    switch(joystick.m_diJs.rgdwPOV[0])
    {
    case 0:
        slot_turnfront();
        break;
    case 4500:
        slot_turnrightfront();
        break;
    case 9000:
        slot_turnright();
        break;
    case 13500:
        slot_turnrightback();
        break;
    case 18000:
        slot_turnback();
        break;
    case 22500:
        slot_turnleftback();
        break;
    case 27000:
        slot_turnleft();
        break;
    case 31500:
        slot_turnleftfront();
        break;
    default: break;
    }

    // Fill up text with which buttons are pressed
    bool m_Button = false;

    for( int i = 0; i < 32; i++ )
    {
      if( joystick.m_diJs.rgbButtons[i] & 0x80 )
      {
          m_Button = true;
          switch(i)
          {
          case 0:
              //strText = "Y";
              break;
          case 1:
              //strText = "B";
              break;
          case 2:
              //strText = "A";
              break;
          case 3:
              //strText = "X";
              break;
          case 4:
              //strText = "LB";
              break;
          case 5:
              //strText = "RB";
              break;
          case 6:
              //strText = "LT";
              break;
          case 7:
              //strText = "RT";
              break;
          case 8:
              //strText = "BACK";
              break;
          case 9:
              //strText = "START";
              break;

          default:break;
          }
      }

    }
    if (m_Button)
    {

    }
    else
    {

    }
}

void MainWindow::slot_open()
{
    QString strImage = QFileDialog::getOpenFileName(this,"Open Map...","","Map(*.pgm *.jpg)");
    if(strImage.isEmpty())
    {
        showMassageFunc(tr("文件路径错误！"));
        return;
    }

    int nRes = showImageFunc(strImage);
    if(-1 == nRes)
    {
        showMassageFunc(tr("打开地图文件时出现错误！"));
        return;
    }    
    slot_navigationmode();
    setNormalActions(true);
    setMapStudyActions(ui->actionMapStudyMode->isChecked());
    setNavigationActions(ui->actionNavigationMode->isChecked());
    //ui->actionMapStudyMode->setEnabled(false);

    QFileInfo fileInfo(strImage);
    QString strMataFile = fileInfo.absolutePath()+"\\"+fileInfo.baseName()+".xml";
    nRes = readImageMatedata(strMataFile);
    if(-1 == nRes)
    {
        pMyCoordTrans->setOriginGeoCoord(-10,-10,0.5);
        showMassageFunc(tr("打开元数据文件时出现错误,初始化默认坐标系！"));
        return;
    }
    msgLabel->setText("Map Opened!");
}

void MainWindow::slot_close()
{    
    pMyImageWidget->openImage(QPixmap());
    setNormalActions(false);
    setMapStudyActions(ui->actionMapStudyMode->isChecked());
    setNavigationActions(false);
    ui->actionMapStudyMode->setEnabled(true);
    nImageWidth = 0;
    nImageHeight = 0;
    msgLabel->setText("Map Closed!");
    xyLabel->setText("");
}

void MainWindow::slot_save()
{
    QString strSaveFile = QFileDialog::getSaveFileName(this,"Save Map...","","Map(*.jpg)");
    if(strSaveFile.isEmpty())
    {
        showMassageFunc(tr("文件路径错误！"));
        return;
    }
    bool nRes = saveImageFunc(strSaveFile);
    if(false == nRes)
    {
        showMassageFunc(tr("保存地图文件时出现错误！"));
        return;
    }
    msgLabel->setText("Map Saved!");
}

void MainWindow::slot_mapstudymode()
{
    ui->actionMapStudyMode->setChecked(true);
    ui->actionNavigationMode->setChecked(false);
    setMapStudyActions(ui->actionMapStudyMode->isChecked());
    setNavigationActions(ui->actionNavigationMode->isChecked());

}

void MainWindow::slot_navigationmode()
{    
    ui->actionMapStudyMode->setChecked(false);
    ui->actionNavigationMode->setChecked(true);
    setMapStudyActions(ui->actionMapStudyMode->isChecked());
    if(nImageWidth>0)
    {
        setNavigationActions(ui->actionNavigationMode->isChecked());
    }
}

void MainWindow::slot_move()
{
    bIsImageMoveFlag = ui->actionMove->isChecked();
    if(bIsImageMoveFlag)
    {
        this->setCursor(Qt::OpenHandCursor);
    }
    else
    {
        this->setCursor(Qt::ArrowCursor);
    }
}

void MainWindow::slot_zoomin()
{
    pMyImageWidget->zoomIn(1.05);
}

void MainWindow::slot_zoomout()
{
    pMyImageWidget->zoomOut(0.95);
}

void MainWindow::slot_fit()
{
    pMyImageWidget->fitToWindow();
}

void MainWindow::slot_normal()
{
    pMyImageWidget->normalSize();
}

void MainWindow::slot_showcar()
{
    if(ui->actionMapStudyMode->isChecked())
    {
        pMyImageWidget->drawCar(0,pMyImageWidget->height(),0);
    }
    pMyImageWidget->showCarFlag(ui->actionShowCar->isChecked());
}

void MainWindow::slot_showline()
{
    pMyImageWidget->showLineFlag(ui->actionShowLine->isChecked());
    if(!ui->actionShowLine->isChecked())
    {
        pMyImageWidget->clearMoveLine();
    }
}

void MainWindow::slot_startstudy()
{
    emit agv_StartMapStudy();
}

void MainWindow::slot_stopstudy()
{
    emit agv_StopMapStudy();
}

void MainWindow::slot_turnleft()
{
    emit agv_SetCarDirection(Left);
}

void MainWindow::slot_turnright()
{
    emit agv_SetCarDirection(Right);
}

void MainWindow::slot_turnfront()
{
    emit agv_SetCarDirection(Front);
}

void MainWindow::slot_turnback()
{
    emit agv_SetCarDirection(Back);
}

void MainWindow::slot_turnleftfront()
{
    emit agv_SetCarDirection(LeftFront);
}

void MainWindow::slot_turnrightfront()
{
    emit agv_SetCarDirection(RightFront);
}

void MainWindow::slot_turnleftback()
{
    emit agv_SetCarDirection(LeftBack);
}

void MainWindow::slot_turnrightback()
{
    emit agv_SetCarDirection(RightBack);
}

void MainWindow::slot_startnavi()
{
    emit agv_StartNavigation();
}

void MainWindow::slot_stopnavi()
{
    emit agv_StopNavigation();
    pMyImageWidget->showTargetFlag(false);
    pMyImageWidget->showNaviRouteFlag(false);
}

void MainWindow::slot_settarget()
{    
    this->setCursor(Qt::CrossCursor);
    bIsSetTargetPosFlag = true;
}

void MainWindow::slot_setcarinitposition()
{
    this->setCursor(Qt::CrossCursor);
    bIsSetInitPosFlag = true;
}

void MainWindow::slot_about()
{
    AboutDialog aboutDlg(this);
    aboutDlg.exec();
}

void MainWindow::agv_LoadMapFile(QString MapFile, QString MatedataFile)
{
    if(MapFile.length()<1)
    {
        showMassageFunc(tr("文件路径错误！"));
        return;
    }
    int nRes = showImageFunc(MapFile);
    if(-1 == nRes)
    {
        showMassageFunc(tr("打开地图文件时出现错误！"));
        return;
    }
    slot_navigationmode();
    setNormalActions(true);
    setMapStudyActions(ui->actionMapStudyMode->isChecked());
    setNavigationActions(ui->actionNavigationMode->isChecked());
    ui->actionMapStudyMode->setEnabled(false);

    nRes = readImageMatedata(MatedataFile);
    if(-1 == nRes)
    {
        showMassageFunc(tr("打开元数据文件时出现错误！"));
        return;
    }
    msgLabel->setText("Map Opened!");
}

void MainWindow::agv_LoadMapData(const unsigned char* MapData, int lenth, double DataCoordX, double DataCoordY, double DataResolution)
{
    QImage *qImg=new QImage;

    if(!(qImg->loadFromData(MapData,lenth))) //加载图像
    {
        showMassageFunc(tr("打开地图数据失败！"));
        delete qImg;
        pMyImageWidget->openImage(QPixmap());
        return ;
    }

    nImageWidth= qImg->width();
    nImageHeight= qImg->height();
    pMyImageWidget->openImage(QPixmap::fromImage(*qImg));
    pMyCoordTrans->setOriginGeoCoord(DataCoordX, DataCoordY, DataResolution);
}

void MainWindow::agv_SetCarCurrentPosition(double currentPosX, double currentPosY, double currentCarDirect)
{
    int nImageX,nImageY;
    pMyCoordTrans->geoCoordToPixelPos(nImageX,nImageY,currentPosX,currentPosY);
    double nScaleRatioX = double(pMyImageWidget->getCurrentWidth())/nImageWidth;
    double nScaleRatioY = double(pMyImageWidget->getCurrentHeigh())/nImageHeight;

    int nWidgetPosX = pMyImageWidget->getBeginX()+nImageX*nScaleRatioX;
    int nWidgetPosY = (nImageHeight-nImageY)*nScaleRatioY+pMyImageWidget->getBeginY();

    pMyImageWidget->drawCar(nWidgetPosX,nWidgetPosY,currentCarDirect);
    pMyImageWidget->drawMoveLine(nWidgetPosX,nWidgetPosY);
}

void MainWindow::agv_ShowMessage(QString Message)
{
    showMassageFunc(Message);
}

void MainWindow::agv_SetNaviRoute(double* pRouteX,double* pRouteY,int nRouteNumber)
{
    QPoint *pPoints = new QPoint[nRouteNumber+1];
    for(int i=0;i<nRouteNumber;i++)
    {
        int nImageX,nImageY;
        pMyCoordTrans->geoCoordToPixelPos(nImageX,nImageY,pRouteX[i],pRouteY[i]);
        double nScaleRatioX = double(pMyImageWidget->getCurrentWidth())/nImageWidth;
        double nScaleRatioY = double(pMyImageWidget->getCurrentHeigh())/nImageHeight;

        int nWidgetPosX = pMyImageWidget->getBeginX()+nImageX*nScaleRatioX;
        int nWidgetPosY = (nImageHeight-nImageY)*nScaleRatioY+pMyImageWidget->getBeginY();

        pPoints[i] = QPoint(nWidgetPosX,nWidgetPosY);
    }
    pMyImageWidget->drawNaviRoute(pPoints,nRouteNumber);
    pMyImageWidget->showNaviRouteFlag(true);
}

