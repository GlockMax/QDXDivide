

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0628 */
/* at Tue Jan 19 08:14:07 2038
 */
/* Compiler settings for server.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.01.0628 
    protocol : all , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */



/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __server_h__
#define __server_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef DECLSPEC_XFGVIRT
#if defined(_CONTROL_FLOW_GUARD_XFG)
#define DECLSPEC_XFGVIRT(base, func) __declspec(xfg_virtual(base, func))
#else
#define DECLSPEC_XFGVIRT(base, func)
#endif
#endif

/* Forward Declarations */ 

#ifndef __IServerData_FWD_DEFINED__
#define __IServerData_FWD_DEFINED__
typedef interface IServerData IServerData;

#endif 	/* __IServerData_FWD_DEFINED__ */


#ifndef __ServerData_FWD_DEFINED__
#define __ServerData_FWD_DEFINED__

#ifdef __cplusplus
typedef class ServerData ServerData;
#else
typedef struct ServerData ServerData;
#endif /* __cplusplus */

#endif 	/* __ServerData_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IServerData_INTERFACE_DEFINED__
#define __IServerData_INTERFACE_DEFINED__

/* interface IServerData */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IServerData;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8820E927-C1F1-11D5-B0AB-0050BF4A8273")
    IServerData : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetProgramsNum( 
            /* [retval][out] */ long *pNum) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetUnitName( 
            /* [retval][out] */ BSTR *ppbstrMessage) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE LoadFile( 
            /* [retval][out] */ BSTR *ppbstrMessage) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetProgWLength( 
            long ProgNum,
            double *Length) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Clear( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetProgName( 
            long ProgNum,
            BSTR *Name) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetProgComment( 
            long ProgNum,
            BSTR *Text) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetProgFLength( 
            long ProgNum,
            double *Length) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetProgTime( 
            long ProgNum,
            double *Time) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetProgLines( 
            long ProgNum,
            long *Count) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetProgSteps( 
            long ProgNum,
            long *Count) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetToolsNum( 
            long *pNum) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetUnitFile( 
            BSTR *pText) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetUnitType( 
            BSTR *pText) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetUnitMan( 
            BSTR *pText) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetUnitOper( 
            BSTR *pText) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetUnitAxes( 
            BSTR *pText) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetStockStlName( 
            BSTR *pText) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetStockType( 
            BSTR *pText) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetStockWidth( 
            double *pSize) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetStockHeight( 
            double *pSize) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetStockLength( 
            double *pSize) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetStockX( 
            double *pCoord) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetStockY( 
            double *pCoord) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetStockZ( 
            double *pCoord) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetStockRotX( 
            double *pCoord) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetStockRotY( 
            double *pCoord) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetStockRotZ( 
            double *pCoord) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetStockBase( 
            long *pNum) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetToolWLength( 
            long ToolNum,
            double *pVal) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetToolFLength( 
            long ToolNum,
            double *pVal) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetToolTime( 
            long ToolNum,
            double *pVal) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetToolName( 
            long ToolNum,
            BSTR *pText) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetToolType( 
            long ToolNum,
            BSTR *pText) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetToolVolume( 
            long ToolName,
            double *pVal) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetToolPos( 
            long ToolNum,
            long *pNum) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IServerDataVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IServerData * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IServerData * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IServerData * This);
        
        DECLSPEC_XFGVIRT(IServerData, GetProgramsNum)
        HRESULT ( STDMETHODCALLTYPE *GetProgramsNum )( 
            IServerData * This,
            /* [retval][out] */ long *pNum);
        
        DECLSPEC_XFGVIRT(IServerData, GetUnitName)
        HRESULT ( STDMETHODCALLTYPE *GetUnitName )( 
            IServerData * This,
            /* [retval][out] */ BSTR *ppbstrMessage);
        
        DECLSPEC_XFGVIRT(IServerData, LoadFile)
        HRESULT ( STDMETHODCALLTYPE *LoadFile )( 
            IServerData * This,
            /* [retval][out] */ BSTR *ppbstrMessage);
        
        DECLSPEC_XFGVIRT(IServerData, GetProgWLength)
        HRESULT ( STDMETHODCALLTYPE *GetProgWLength )( 
            IServerData * This,
            long ProgNum,
            double *Length);
        
        DECLSPEC_XFGVIRT(IServerData, Clear)
        HRESULT ( STDMETHODCALLTYPE *Clear )( 
            IServerData * This);
        
        DECLSPEC_XFGVIRT(IServerData, GetProgName)
        HRESULT ( STDMETHODCALLTYPE *GetProgName )( 
            IServerData * This,
            long ProgNum,
            BSTR *Name);
        
        DECLSPEC_XFGVIRT(IServerData, GetProgComment)
        HRESULT ( STDMETHODCALLTYPE *GetProgComment )( 
            IServerData * This,
            long ProgNum,
            BSTR *Text);
        
        DECLSPEC_XFGVIRT(IServerData, GetProgFLength)
        HRESULT ( STDMETHODCALLTYPE *GetProgFLength )( 
            IServerData * This,
            long ProgNum,
            double *Length);
        
        DECLSPEC_XFGVIRT(IServerData, GetProgTime)
        HRESULT ( STDMETHODCALLTYPE *GetProgTime )( 
            IServerData * This,
            long ProgNum,
            double *Time);
        
        DECLSPEC_XFGVIRT(IServerData, GetProgLines)
        HRESULT ( STDMETHODCALLTYPE *GetProgLines )( 
            IServerData * This,
            long ProgNum,
            long *Count);
        
        DECLSPEC_XFGVIRT(IServerData, GetProgSteps)
        HRESULT ( STDMETHODCALLTYPE *GetProgSteps )( 
            IServerData * This,
            long ProgNum,
            long *Count);
        
        DECLSPEC_XFGVIRT(IServerData, GetToolsNum)
        HRESULT ( STDMETHODCALLTYPE *GetToolsNum )( 
            IServerData * This,
            long *pNum);
        
        DECLSPEC_XFGVIRT(IServerData, GetUnitFile)
        HRESULT ( STDMETHODCALLTYPE *GetUnitFile )( 
            IServerData * This,
            BSTR *pText);
        
        DECLSPEC_XFGVIRT(IServerData, GetUnitType)
        HRESULT ( STDMETHODCALLTYPE *GetUnitType )( 
            IServerData * This,
            BSTR *pText);
        
        DECLSPEC_XFGVIRT(IServerData, GetUnitMan)
        HRESULT ( STDMETHODCALLTYPE *GetUnitMan )( 
            IServerData * This,
            BSTR *pText);
        
        DECLSPEC_XFGVIRT(IServerData, GetUnitOper)
        HRESULT ( STDMETHODCALLTYPE *GetUnitOper )( 
            IServerData * This,
            BSTR *pText);
        
        DECLSPEC_XFGVIRT(IServerData, GetUnitAxes)
        HRESULT ( STDMETHODCALLTYPE *GetUnitAxes )( 
            IServerData * This,
            BSTR *pText);
        
        DECLSPEC_XFGVIRT(IServerData, GetStockStlName)
        HRESULT ( STDMETHODCALLTYPE *GetStockStlName )( 
            IServerData * This,
            BSTR *pText);
        
        DECLSPEC_XFGVIRT(IServerData, GetStockType)
        HRESULT ( STDMETHODCALLTYPE *GetStockType )( 
            IServerData * This,
            BSTR *pText);
        
        DECLSPEC_XFGVIRT(IServerData, GetStockWidth)
        HRESULT ( STDMETHODCALLTYPE *GetStockWidth )( 
            IServerData * This,
            double *pSize);
        
        DECLSPEC_XFGVIRT(IServerData, GetStockHeight)
        HRESULT ( STDMETHODCALLTYPE *GetStockHeight )( 
            IServerData * This,
            double *pSize);
        
        DECLSPEC_XFGVIRT(IServerData, GetStockLength)
        HRESULT ( STDMETHODCALLTYPE *GetStockLength )( 
            IServerData * This,
            double *pSize);
        
        DECLSPEC_XFGVIRT(IServerData, GetStockX)
        HRESULT ( STDMETHODCALLTYPE *GetStockX )( 
            IServerData * This,
            double *pCoord);
        
        DECLSPEC_XFGVIRT(IServerData, GetStockY)
        HRESULT ( STDMETHODCALLTYPE *GetStockY )( 
            IServerData * This,
            double *pCoord);
        
        DECLSPEC_XFGVIRT(IServerData, GetStockZ)
        HRESULT ( STDMETHODCALLTYPE *GetStockZ )( 
            IServerData * This,
            double *pCoord);
        
        DECLSPEC_XFGVIRT(IServerData, GetStockRotX)
        HRESULT ( STDMETHODCALLTYPE *GetStockRotX )( 
            IServerData * This,
            double *pCoord);
        
        DECLSPEC_XFGVIRT(IServerData, GetStockRotY)
        HRESULT ( STDMETHODCALLTYPE *GetStockRotY )( 
            IServerData * This,
            double *pCoord);
        
        DECLSPEC_XFGVIRT(IServerData, GetStockRotZ)
        HRESULT ( STDMETHODCALLTYPE *GetStockRotZ )( 
            IServerData * This,
            double *pCoord);
        
        DECLSPEC_XFGVIRT(IServerData, GetStockBase)
        HRESULT ( STDMETHODCALLTYPE *GetStockBase )( 
            IServerData * This,
            long *pNum);
        
        DECLSPEC_XFGVIRT(IServerData, GetToolWLength)
        HRESULT ( STDMETHODCALLTYPE *GetToolWLength )( 
            IServerData * This,
            long ToolNum,
            double *pVal);
        
        DECLSPEC_XFGVIRT(IServerData, GetToolFLength)
        HRESULT ( STDMETHODCALLTYPE *GetToolFLength )( 
            IServerData * This,
            long ToolNum,
            double *pVal);
        
        DECLSPEC_XFGVIRT(IServerData, GetToolTime)
        HRESULT ( STDMETHODCALLTYPE *GetToolTime )( 
            IServerData * This,
            long ToolNum,
            double *pVal);
        
        DECLSPEC_XFGVIRT(IServerData, GetToolName)
        HRESULT ( STDMETHODCALLTYPE *GetToolName )( 
            IServerData * This,
            long ToolNum,
            BSTR *pText);
        
        DECLSPEC_XFGVIRT(IServerData, GetToolType)
        HRESULT ( STDMETHODCALLTYPE *GetToolType )( 
            IServerData * This,
            long ToolNum,
            BSTR *pText);
        
        DECLSPEC_XFGVIRT(IServerData, GetToolVolume)
        HRESULT ( STDMETHODCALLTYPE *GetToolVolume )( 
            IServerData * This,
            long ToolName,
            double *pVal);
        
        DECLSPEC_XFGVIRT(IServerData, GetToolPos)
        HRESULT ( STDMETHODCALLTYPE *GetToolPos )( 
            IServerData * This,
            long ToolNum,
            long *pNum);
        
        END_INTERFACE
    } IServerDataVtbl;

    interface IServerData
    {
        CONST_VTBL struct IServerDataVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IServerData_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IServerData_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IServerData_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IServerData_GetProgramsNum(This,pNum)	\
    ( (This)->lpVtbl -> GetProgramsNum(This,pNum) ) 

#define IServerData_GetUnitName(This,ppbstrMessage)	\
    ( (This)->lpVtbl -> GetUnitName(This,ppbstrMessage) ) 

#define IServerData_LoadFile(This,ppbstrMessage)	\
    ( (This)->lpVtbl -> LoadFile(This,ppbstrMessage) ) 

#define IServerData_GetProgWLength(This,ProgNum,Length)	\
    ( (This)->lpVtbl -> GetProgWLength(This,ProgNum,Length) ) 

#define IServerData_Clear(This)	\
    ( (This)->lpVtbl -> Clear(This) ) 

#define IServerData_GetProgName(This,ProgNum,Name)	\
    ( (This)->lpVtbl -> GetProgName(This,ProgNum,Name) ) 

#define IServerData_GetProgComment(This,ProgNum,Text)	\
    ( (This)->lpVtbl -> GetProgComment(This,ProgNum,Text) ) 

#define IServerData_GetProgFLength(This,ProgNum,Length)	\
    ( (This)->lpVtbl -> GetProgFLength(This,ProgNum,Length) ) 

#define IServerData_GetProgTime(This,ProgNum,Time)	\
    ( (This)->lpVtbl -> GetProgTime(This,ProgNum,Time) ) 

#define IServerData_GetProgLines(This,ProgNum,Count)	\
    ( (This)->lpVtbl -> GetProgLines(This,ProgNum,Count) ) 

#define IServerData_GetProgSteps(This,ProgNum,Count)	\
    ( (This)->lpVtbl -> GetProgSteps(This,ProgNum,Count) ) 

#define IServerData_GetToolsNum(This,pNum)	\
    ( (This)->lpVtbl -> GetToolsNum(This,pNum) ) 

#define IServerData_GetUnitFile(This,pText)	\
    ( (This)->lpVtbl -> GetUnitFile(This,pText) ) 

#define IServerData_GetUnitType(This,pText)	\
    ( (This)->lpVtbl -> GetUnitType(This,pText) ) 

#define IServerData_GetUnitMan(This,pText)	\
    ( (This)->lpVtbl -> GetUnitMan(This,pText) ) 

#define IServerData_GetUnitOper(This,pText)	\
    ( (This)->lpVtbl -> GetUnitOper(This,pText) ) 

#define IServerData_GetUnitAxes(This,pText)	\
    ( (This)->lpVtbl -> GetUnitAxes(This,pText) ) 

#define IServerData_GetStockStlName(This,pText)	\
    ( (This)->lpVtbl -> GetStockStlName(This,pText) ) 

#define IServerData_GetStockType(This,pText)	\
    ( (This)->lpVtbl -> GetStockType(This,pText) ) 

#define IServerData_GetStockWidth(This,pSize)	\
    ( (This)->lpVtbl -> GetStockWidth(This,pSize) ) 

#define IServerData_GetStockHeight(This,pSize)	\
    ( (This)->lpVtbl -> GetStockHeight(This,pSize) ) 

#define IServerData_GetStockLength(This,pSize)	\
    ( (This)->lpVtbl -> GetStockLength(This,pSize) ) 

#define IServerData_GetStockX(This,pCoord)	\
    ( (This)->lpVtbl -> GetStockX(This,pCoord) ) 

#define IServerData_GetStockY(This,pCoord)	\
    ( (This)->lpVtbl -> GetStockY(This,pCoord) ) 

#define IServerData_GetStockZ(This,pCoord)	\
    ( (This)->lpVtbl -> GetStockZ(This,pCoord) ) 

#define IServerData_GetStockRotX(This,pCoord)	\
    ( (This)->lpVtbl -> GetStockRotX(This,pCoord) ) 

#define IServerData_GetStockRotY(This,pCoord)	\
    ( (This)->lpVtbl -> GetStockRotY(This,pCoord) ) 

#define IServerData_GetStockRotZ(This,pCoord)	\
    ( (This)->lpVtbl -> GetStockRotZ(This,pCoord) ) 

#define IServerData_GetStockBase(This,pNum)	\
    ( (This)->lpVtbl -> GetStockBase(This,pNum) ) 

#define IServerData_GetToolWLength(This,ToolNum,pVal)	\
    ( (This)->lpVtbl -> GetToolWLength(This,ToolNum,pVal) ) 

#define IServerData_GetToolFLength(This,ToolNum,pVal)	\
    ( (This)->lpVtbl -> GetToolFLength(This,ToolNum,pVal) ) 

#define IServerData_GetToolTime(This,ToolNum,pVal)	\
    ( (This)->lpVtbl -> GetToolTime(This,ToolNum,pVal) ) 

#define IServerData_GetToolName(This,ToolNum,pText)	\
    ( (This)->lpVtbl -> GetToolName(This,ToolNum,pText) ) 

#define IServerData_GetToolType(This,ToolNum,pText)	\
    ( (This)->lpVtbl -> GetToolType(This,ToolNum,pText) ) 

#define IServerData_GetToolVolume(This,ToolName,pVal)	\
    ( (This)->lpVtbl -> GetToolVolume(This,ToolName,pVal) ) 

#define IServerData_GetToolPos(This,ToolNum,pNum)	\
    ( (This)->lpVtbl -> GetToolPos(This,ToolNum,pNum) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IServerData_INTERFACE_DEFINED__ */



#ifndef __SERVERLib_LIBRARY_DEFINED__
#define __SERVERLib_LIBRARY_DEFINED__

/* library SERVERLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_SERVERLib;

EXTERN_C const CLSID CLSID_ServerData;

#ifdef __cplusplus

class DECLSPEC_UUID("8820E928-C1F1-11D5-B0AB-0050BF4A8273")
ServerData;
#endif
#endif /* __SERVERLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

unsigned long             __RPC_USER  BSTR_UserSize64(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal64(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal64(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree64(     unsigned long *, BSTR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


