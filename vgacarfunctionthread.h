/********************
*AGV自动导航软件
* Author:Bo.Z
* Date:20160304
*********************/


#ifndef VGACARFUNCTIONTHREAD_H
#define VGACARFUNCTIONTHREAD_H

#include <QThread>
#include <string>

using std::string;

enum DIRECT
{
    Front = 1,
    Back = 2,
    Left = 3,
    Right = 4,
    LeftFront = 5,
    RightFront = 6,
    LeftBack = 7,
    RightBack = 8
};

class VgaCarFunctionThread : public QThread
{
    Q_OBJECT

public:
    VgaCarFunctionThread();

protected:
    void run();

protected: //接口
    void loadMapFile(string MapFile, string MatedataFile);
    void loadMapData(const unsigned char* MapData, int lenth, double DataCoordX, double DataCoordY, double DataResolution);
    void startMapStudy();
    void stopMapStudy();
    void setCarDirection(DIRECT CarDirect);
    void setCarInitPoistion(double initPosX,double initPosY,double initCarDirect = 0);
    void setCarCurrentPosition(double currentPosX,double currentPosY,double currentCarDirect);
    void setNaviTargetPoistion(double targetPosX,double targetPosY);
    void setNaviRoute(double* pRouteX,double* pRouteY,int nRouteNumber);
    void startNavigation();
    void stopNavigation();
    void showMessage(string Message);

signals:
    void agv_LoadMapFile(QString MapFile, QString MatedataFile);
    void agv_LoadMapData(const unsigned char* MapData, int lenth, double DataCoordX, double DataCoordY, double DataResolution);
    void agv_SetCarCurrentPosition(double currentPosX,double currentPosY,double currentCarDirect);
    void agv_ShowMessage(QString Message);
    void agv_SetNaviRoute(double* pRouteX,double* pRouteY,int nRouteNumber);

protected slots:
    void agv_StartMapStudy();
    void agv_StopMapStudy();
    void agv_SetCarDirection(DIRECT CarDirect);
    void agv_SetCarInitPoistion(double initPosX,double initPosY,double initCarDirect = 0);
    void agv_SetNaviTargetPoistion(double targetPosX,double targetPosY);
    void agv_StartNavigation();
    void agv_StopNavigation();

private:
    bool m_StartMapStudyFlag;
    bool m_StopMapStudyFlag;
    bool m_StartNavigationFlag;
    bool m_StopNavigationFlag;
    bool m_SetCarDirectFlag;
    bool m_SetCarInitPoistion;
    bool m_SetNaviTargetPoistion;

    DIRECT m_CarDirect;
    double m_InitPosX;
    double m_InitPosY;
    double m_InitCarDirect;
    double m_TargetPosX;
    double m_TargetPosY;

private:
    bool routeFlag;

};

#endif // VGACARFUNCTIONTHREAD_H
