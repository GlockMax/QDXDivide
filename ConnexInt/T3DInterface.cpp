#include "StdAfx.h"
#include "t3dinterface.h"

T3DInterface::T3DInterface(void)
{
	InitializeCOM();
}

T3DInterface::~T3DInterface(void)
{
	UninitializeCOM();
}

HRESULT T3DInterface::InitializeCOM()
{
	HRESULT hr;
	CComPtr<IUnknown> _3DxDevice;

	// Create the device object
	hr = _3DxDevice.CoCreateInstance(__uuidof(Device));
	if(!SUCCEEDED(hr))
		return hr;

	//CComPtr<ITDxInfo> _3DxTDxInfo;

	//hr = _3DxTDxInfo.CoCreateInstance(__uuidof(TDxInfo));
	//if(!SUCCEEDED(hr))
	//	return hr;

	//BSTR revisionNumber;
	//_3DxTDxInfo->RevisionNumber (&revisionNumber);

	CComPtr<ISimpleDevice> _3DxSimpleDevice;

	hr = _3DxDevice.QueryInterface(&_3DxSimpleDevice);
	if(!SUCCEEDED(hr))
		return hr;

	// Get the interfaces to the sensor and the keyboard;
	hr = _3DxSimpleDevice->get_Sensor(&m_pISensor);
	hr = __hook(&_ISimpleDeviceEvents::DeviceChange, _3DxSimpleDevice, 
				&T3DInterface::OnDeviceChange);

	hr = __hook(&_ISensorEvents::SensorInput, m_pISensor, 
				&T3DInterface::OnSensorInput);

	hr = _3DxSimpleDevice->get_Keyboard(&m_pIKeyboard);

	hr = __hook(&_IKeyboardEvents::KeyDown, m_pIKeyboard, 
				&T3DInterface::OnKeyDown);

	hr = __hook(&_IKeyboardEvents::KeyUp, m_pIKeyboard, 
				&T3DInterface::OnKeyUp);

	// Set the preferences we want to use
	//hr = _3DxSimpleDevice->LoadPreferences(BSTR("LINGVO"));

	// Connect to the driver
	hr = _3DxSimpleDevice->Connect();

	return hr;
}

HRESULT T3DInterface::OnDeviceChange(long reserved )
{
   HRESULT result = S_OK;
   //AtlTrace (_T("CCubeWindow::OnDeviceChange(reserved =0x%x)\n"), reserved);

   HRESULT hr;
   ISimpleDevicePtr _p3DxDevice;
   hr = m_pIKeyboard->get_Device((IDispatch**)&_p3DxDevice);
   if (SUCCEEDED(hr))
   {
      long device;
      _p3DxDevice->get_Type(&device);
      //AtlTrace (_T("Attached device=%d\n"), device);
      _p3DxDevice.Release();

      long keys, programmableKeys;
      m_pIKeyboard->get_Keys(&keys);
      m_pIKeyboard->get_ProgrammableKeys(&programmableKeys);
      //AtlTrace (_T("Number of Keys=%d\tProgrammable keys=%d\n"), keys, programmableKeys);
      for (long i=1; i<=keys; i++)
      {
         BSTR bstrLabel;
         BSTR bstrName;
         m_pIKeyboard->GetKeyLabel(i, &bstrLabel);
         m_pIKeyboard->GetKeyName(i, &bstrName);
         CString strLabel(bstrLabel);
         CString strName(bstrName);

         //AtlTrace (_T("Key Label=%s\tKey Name=%s\n"), (const TCHAR *)strLabel, (const TCHAR *)strName);
      }
   }
   return result;
}

HRESULT T3DInterface::OnKeyDown(int keyCode )
{
   HRESULT result = S_OK;
   HWND w = GetActiveWindow();
   if(w)
	   PostMessage(w, WM_APP_3DXKEYDOWN, WPARAM(keyCode), 0);
   switch (keyCode)
   {
   case 31: // Fit
      break;
   }
   return result;
}

HRESULT T3DInterface::OnKeyUp(int keyCode )
{
   HRESULT result = S_OK;
   //AtlTrace (_T("CCubeWindow::OnKeyUp(keyCode =%d)\n"), keyCode);
   return result;
}

HRESULT T3DInterface::OnSensorInput(void)
{
   HRESULT result = S_OK;
   static DWORD s_dwLastDraw = 0;

   //AtlTrace (_T("CCubeWindow::OnSensorInput()\n"));
   try {
      CComPtr<IAngleAxis> pRotation;
      HRESULT hr = m_pISensor->get_Rotation(&pRotation);

      pRotation->get_Angle(&Data.Ar);
	  if(Data.Ar != 0.)
	  {
         pRotation->get_X(&Data.Xr);
         pRotation->get_Y(&Data.Yr);
         pRotation->get_Z(&Data.Zr);
	  }


      CComPtr<IVector3D> pTranslation;
      hr = m_pISensor->get_Translation(&pTranslation);    
 
      pTranslation->get_Length(&Data.Lt);
	  if(Data.Lt != 0.)
	  {
         pTranslation->get_X(&Data.Xt);
         pTranslation->get_Y(&Data.Yt);
         pTranslation->get_Z(&Data.Zt);
	  }

	  if(Data.Lt != 0. || Data.Ar != 0.)
	  {
			m_pISensor->get_Period(&Data.Period);
	  }
      pRotation.Release();
      pTranslation.Release();
   }
   catch (...)
   {
      // Some sort of exception handling
   }
	HWND w = GetActiveWindow();
	if(w)
		SendMessage(w, WM_APP_3DXINPUT,0,(LPARAM)(&Data));

   return result;
}

HRESULT T3DInterface::UninitializeCOM()
{
   HRESULT hr = S_FALSE;

   CComPtr<IDispatch> _3DxDevice;
   if (m_pISensor)
      hr = m_pISensor->get_Device(&_3DxDevice);
   else if (m_pIKeyboard)
      hr = m_pIKeyboard->get_Device(&_3DxDevice);
   if (hr == S_OK)
   {
      CComPtr<ISimpleDevice> _3DxSimpleDevice;
      hr = _3DxDevice.QueryInterface(&_3DxSimpleDevice);
      if (SUCCEEDED(hr))
         _3DxSimpleDevice->Disconnect();
   }

   if (m_pISensor)
   {
      // unhook (unadvise) the sensor event sink
      __unhook(&_ISensorEvents::SensorInput, m_pISensor, 
                     &T3DInterface::OnSensorInput);

      m_pISensor.Release();
   }

   if (m_pIKeyboard)
   {
      __unhook(&_IKeyboardEvents::KeyDown, m_pIKeyboard, 
                     &T3DInterface::OnKeyDown);

      m_pIKeyboard.Release();
   }

   return hr;
}
