/********************
*AGV自动导航软件
* Author:Bo.Z
* Date:20160304
*********************/


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QLabel>
#include <QProgressBar>
#include <qnamespace.h>

#include <string>
#include "myimagewidget.h"
#include "coordinatetransform.h"
#include "vgacarfunctionthread.h"
#include "Joystick.h"

using std::string;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

private: //窗口设置函数
    void setNormalActions(bool YorN);
    void setMapStudyActions(bool YorN);
    void setNavigationActions(bool YorN);
    void setWindow();
    void setConnect();
    void setVgaConnect();

private: //功能处理函数
    int  showImageFunc(QString strImageFile);
    int  readImageMatedata(QString strMatadataFile);
    void showMassageFunc(QString strMsg);
    int  saveImageFunc(QString strImageFile);
    int  getImageX(QPoint point);
    int  getImageY(QPoint point);

protected:  //事件重载
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent  *event);
    void keyReleaseEvent(QKeyEvent *event);
    void timerEvent(QTimerEvent *e);

private:
    MyImageWidget *pMyImageWidget;
    CoordinateTransform *pMyCoordTrans;
    VgaCarFunctionThread *pVGA;

    int nImageWidth;
    int nImageHeight;
    bool bIsImageMoveFlag;
    bool bIsSetInitPosFlag;
    bool bIsSetTargetPosFlag;

    int nLeftButtonPressPosX;
    int nLeftButtonPressPosY;

    QLabel *msgLabel;
    QLabel *xyLabel;
    QProgressBar *progressBar;

    int key1;
    int key2;

    CJoystick joystick;

private slots:
    void slot_open();
    void slot_close();
    void slot_save();
    void slot_mapstudymode();
    void slot_navigationmode();

    void slot_move();
    void slot_zoomin();
    void slot_zoomout();
    void slot_fit();
    void slot_normal();
    void slot_showcar();
    void slot_showline();

    void slot_startstudy();
    void slot_stopstudy();    
    void slot_turnleft();
    void slot_turnright();
    void slot_turnfront();
    void slot_turnback();
    void slot_turnleftfront();
    void slot_turnrightfront();
    void slot_turnleftback();
    void slot_turnrightback();

    void slot_startnavi();
    void slot_stopnavi();
    void slot_settarget();
    void slot_setcarinitposition();
    void slot_about();

private slots: //外部接口槽函数
    void agv_LoadMapFile(QString MapFile, QString MatedataFile);
    void agv_LoadMapData(const unsigned char* MapData, int lenth, double DataCoordX, double DataCoordY, double DataResolution);
    void agv_SetCarCurrentPosition(double currentPosX,double currentPosY,double currentCarDirect);
    void agv_ShowMessage(QString Message);
    void agv_SetNaviRoute(double* pRouteX,double* pRouteY,int nRouteNumber);

signals: //外部接口信号函数
    void agv_StartMapStudy();
    void agv_StopMapStudy();
    void agv_SetCarDirection(DIRECT CarDirect);
    void agv_SetCarInitPoistion(double initPosX,double initPosY,double initCarDirect = 0);
    void agv_SetNaviTargetPoistion(double targetPosX,double targetPosY);
    void agv_StartNavigation();
    void agv_StopNavigation();
};

#endif // MAINWINDOW_H
