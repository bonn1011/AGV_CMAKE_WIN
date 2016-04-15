#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
class CJoystick
{
public:
	CJoystick(void);
	virtual ~CJoystick(void);
	//һ��ĳ�Ա����
     HINSTANCE m_hInstance;               // ʵ�����
     HWND m_hWnd;                         // ���ھ��
     LPDIRECTINPUT8  m_lpDI;              // DI8�ӿ�ָ��
     LPDIRECTINPUTDEVICE8  m_lpDIDevice;  // DIDevice8�ӿ�ָ��
     DIJOYSTATE   m_diJs;                  //�洢Joystick״̬��Ϣ
	 DIJOYSTATE2  m_JS;  
     GUID JoystickGUID;                   //GUID
	 //��Ա����
     bool Initialise(void);               //��ʼ������
     //ö���豸
     static BOOL CALLBACK DIEnumDevicesCallback(const DIDEVICEINSTANCE* lpddi, VOID* pvRef);      //ö�ٶ���
     static BOOL CALLBACK EnumObjectsCallback( const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext );
     HRESULT PollDevice(void);        // ��ѭ�豸����������ѭ�����ж�ȡ�豸״̬
};
