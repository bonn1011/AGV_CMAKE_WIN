#include "vgacarfunctionthread.h"

VgaCarFunctionThread::VgaCarFunctionThread()
{
    routeFlag = true;
}

void VgaCarFunctionThread::run()
{
    m_StartMapStudyFlag = false;
    m_StopMapStudyFlag = false;
    m_StartNavigationFlag = false;
    m_StopNavigationFlag = false;
    m_SetCarDirectFlag = false;
    m_SetCarInitPoistion = false;
    m_SetNaviTargetPoistion = false;
    m_CarDirect = Front;
    m_InitPosX = 0.0;
    m_InitPosY = 0.0;
    m_InitCarDirect = 0.0;
    m_TargetPosX = 100.0;
    m_TargetPosY = 100.0;

    while(true)
    {
        if(m_StartMapStudyFlag)
        {
            m_StartMapStudyFlag = false;
            startMapStudy();
        }

        if(m_StartNavigationFlag)
        {
            m_StartNavigationFlag = false;
            startNavigation();
        }        

        if(m_SetCarInitPoistion)
        {
            m_SetCarInitPoistion = false;
            setCarInitPoistion(m_InitPosX,m_InitPosY,m_InitCarDirect);
        }

        if(m_SetNaviTargetPoistion)
        {
            m_SetNaviTargetPoistion = false;
            setNaviTargetPoistion(m_TargetPosX,m_TargetPosY);
        }
    }
}

/***********
 * 接口函数*/
void VgaCarFunctionThread::loadMapFile(std::string MapFile, std::string MatedataFile)
{
    emit agv_LoadMapFile(QString::fromStdString(MapFile),QString::fromStdString(MatedataFile));
}

void VgaCarFunctionThread::loadMapData(const unsigned char *MapData, int lenth, double DataCoordX, double DataCoordY, double DataResolution)
{
    emit agv_LoadMapData(MapData,lenth,DataCoordX,DataCoordY,DataResolution);
}

void VgaCarFunctionThread::startMapStudy()
{
    //showMessage("startMapStudy");
    double cX = -10;
    double cY = -10;
    double cSita = 0;
    while(true)
    {
        if(m_StopMapStudyFlag)
        {
            m_StopMapStudyFlag = false;
            stopMapStudy();
            return;
        }
        if(m_SetCarDirectFlag)
        {
            m_SetCarDirectFlag = false;
            setCarDirection(m_CarDirect);
        }
        for(int j=0;j<100000000;j++)
        {}
        switch (m_CarDirect)
        {
        case Front:
            cY=cY+0.5;
            cSita = 0;
            break;
        case Back:
            cY=cY-0.5;
            cSita = 180;
            break;
        case Left:
            cX=cX-0.5;
            cSita = 270;
            break;
        case Right:
            cX=cX+0.5;
            cSita = 90;
            break;
        case LeftFront:
            cY=cY+0.5;
            cX=cX-0.5;
            cSita = 315;
            break;
        case RightFront:
            cY=cY+0.5;
            cX=cX+0.5;
            cSita = 45;
            break;
        case LeftBack:
            cY=cY-0.5;
            cX=cX-0.5;
            cSita = 225;
            break;
        case RightBack:
            cY=cY-0.5;
            cX=cX+0.5;
            cSita = 135;
            break;
        default:
            break;
        }

        setCarCurrentPosition(cX,cY,cSita);
    }
}

void VgaCarFunctionThread::stopMapStudy()
{
    loadMapFile("C:\\Users\\BoZ\\Pictures\\Saved Pictures\\1.jpg","C:\\Users\\BoZ\\Pictures\\Saved Pictures\\1.xml");
}

void VgaCarFunctionThread::setCarDirection(DIRECT CarDirect)
{
    QString strTemp = "setCarDirection: ";
    switch (CarDirect)
    {
    case Front:
        strTemp = strTemp+"Front.";
        break;
    case Back:
        strTemp = strTemp+"Back.";
        break;
    case Left:
        strTemp = strTemp+"Left.";
        break;
    case Right:
        strTemp = strTemp+"Right.";
        break;
    case LeftFront:
        strTemp = strTemp+"LeftFront.";
        break;
    case RightFront:
        strTemp = strTemp+"RightFront.";
        break;
    case LeftBack:
        strTemp = strTemp+"LeftBack.";
        break;
    case RightBack:
        strTemp = strTemp+"RightBack.";
        break;
    default:
        strTemp = "Error!";
        break;
    }
    //showMessage(strTemp.toStdString());
}

void VgaCarFunctionThread::setCarInitPoistion(double initPosX, double initPosY, double initCarDirect)
{
    QString strTemp = "setCarInitPoistion( "+QString::number(initPosX)+","+QString::number(initPosY)+","+QString::number(initCarDirect)+").";
    showMessage(strTemp.toStdString());
}

void VgaCarFunctionThread::setCarCurrentPosition(double currentPosX, double currentPosY, double currentCarDirect)
{
    emit agv_SetCarCurrentPosition(currentPosX,currentPosY,currentCarDirect);
}

void VgaCarFunctionThread::setNaviTargetPoistion(double targetPosX, double targetPosY)
{
    QString strTemp = "setNaviTargetPoistion( "+QString::number(targetPosX)+","+QString::number(targetPosY)+").";
    showMessage(strTemp.toStdString());
    for(int j=0;j<200000000;j++)
    {}

    double *pPointX = new double[1000];
    double *pPointY = new double[1000];
    int nPointsNumber = 0;

    for(int i=10;i<300;i+=10)
    {
        for(int j=10;j<300;j+=10)
        {
            if(routeFlag)
            {
                pPointX[nPointsNumber] = i;
                pPointY[nPointsNumber] = j;
            }
            else
            {
                pPointX[nPointsNumber] = j;
                pPointY[nPointsNumber] = i;
            }
            nPointsNumber++;
        }
    }

    routeFlag = !routeFlag;

    setNaviRoute(pPointX,pPointY,nPointsNumber);
}

void VgaCarFunctionThread::setNaviRoute(double* pRouteX,double* pRouteY,int nRouteNumber)
{
    emit agv_SetNaviRoute(pRouteX,pRouteY,nRouteNumber);
}

void VgaCarFunctionThread::startNavigation()
{
    double cX = m_InitPosX;
    double cY = m_InitPosY;
    double cSita = m_InitCarDirect;
    while(true)
    {
        m_InitPosX = cX;
        m_InitPosY = cY;
        m_InitCarDirect = cSita;
        if(m_StopNavigationFlag)
        {
            m_StopNavigationFlag = false;
            stopNavigation();
            return;
        }
        if(m_SetCarDirectFlag)
        {
            m_SetCarDirectFlag = false;
            setCarDirection(m_CarDirect);
        }
        for(int j=0;j<100000000;j++)
        {}
        switch (m_CarDirect)
        {
        case Front:
            cY=cY+0.5;
            cSita = 0;
            break;
        case Back:
            cY=cY-0.5;
            cSita = 180;
            break;
        case Left:
            cX=cX-0.5;
            cSita = 270;
            break;
        case Right:
            cX=cX+0.5;
            cSita = 90;
            break;
        case LeftFront:
            cY=cY+0.5;
            cX=cX-0.5;
            cSita = 315;
            break;
        case RightFront:
            cY=cY+0.5;
            cX=cX+0.5;
            cSita = 45;
            break;
        case LeftBack:
            cY=cY-0.5;
            cX=cX-0.5;
            cSita = 225;
            break;
        case RightBack:
            cY=cY-0.5;
            cX=cX+0.5;
            cSita = 135;
            break;
        default:
            break;
        }
        setCarCurrentPosition(cX,cY,cSita);
    }
}

void VgaCarFunctionThread::stopNavigation()
{
    showMessage("stopNavigation");
}

void VgaCarFunctionThread::showMessage(std::string Message)
{
    emit agv_ShowMessage(QString::fromStdString(Message));
}


/*************
 * 信号-槽函数*/
void VgaCarFunctionThread::agv_StartMapStudy()
{
    //startMapStudy();
    m_StartMapStudyFlag = true;
}

void VgaCarFunctionThread::agv_StopMapStudy()
{
    //stopMapStudy();
    m_StopMapStudyFlag = true;
}

void VgaCarFunctionThread::agv_SetCarDirection(DIRECT CarDirect)
{
    //setCarDirection(CarDirect);
    m_CarDirect = CarDirect;
    m_SetCarDirectFlag = true;
}

void VgaCarFunctionThread::agv_SetCarInitPoistion(double initPosX, double initPosY, double initCarDirect)
{
    //setCarInitPoistion(initPosX,initPosY,initCarDirect);
    m_InitPosX = initPosX;
    m_InitPosY = initPosY;
    m_InitCarDirect = initCarDirect;

    m_SetCarInitPoistion = true;
}

void VgaCarFunctionThread::agv_SetNaviTargetPoistion(double targetPosX, double targetPosY)
{
    //setNaviTargetPoistion(targetPosX,targetPosY);
    m_TargetPosX = targetPosX;
    m_TargetPosY = targetPosY;
    m_SetNaviTargetPoistion = true;
}

void VgaCarFunctionThread::agv_StartNavigation()
{
    //startNavigation();
    m_StartNavigationFlag = true;
}

void VgaCarFunctionThread::agv_StopNavigation()
{
    //stopNavigation();
    m_StopNavigationFlag = true;
}

