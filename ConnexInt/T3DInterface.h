#pragma once
#include "T3DData.h"

[ event_receiver(com)]
class T3DInterface
{
public:
	T3DInterface(void);
	~T3DInterface(void);
protected:
   CComPtr<ISensor> m_pISensor;
   CComPtr<IKeyboard> m_pIKeyboard;
   T3DData Data;
// COM Event handlers
public:
   HRESULT InitializeCOM();
   HRESULT OnDeviceChange (long reserved);
   HRESULT OnKeyDown (int keyCode);
   HRESULT OnKeyUp (int keyCode);
   HRESULT OnSensorInput(void);
   HRESULT UninitializeCOM();
};
