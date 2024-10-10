

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 8.01.0628 */
/* at Tue Jan 19 08:14:07 2038
 */
/* Compiler settings for NCMHandlers.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.01.0628 
    protocol : all , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */



#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        EXTERN_C __declspec(selectany) const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif // !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, IID_IPreview,0xa5f0293f,0x474b,0x4e0a,0xac,0x0f,0x4b,0xdc,0xcd,0xf0,0xe5,0x19);


MIDL_DEFINE_GUID(IID, IID_IThumbnail,0xbafe7e5d,0xe31a,0x47fc,0xab,0x42,0xbd,0x5e,0x8a,0xaa,0x82,0x98);


MIDL_DEFINE_GUID(IID, IID_ISearch,0xa0fabdd9,0xd41c,0x4590,0xb4,0x89,0x91,0x8e,0x02,0x69,0xc0,0x16);


MIDL_DEFINE_GUID(IID, IID_IIconHandler,0xa028dfe6,0x1a43,0x4c59,0x93,0x57,0x43,0x30,0xcc,0xa1,0x16,0x03);


MIDL_DEFINE_GUID(IID, LIBID_NCMHandlersLib,0xbfe9f6ac,0x4c41,0x4533,0x96,0xb2,0x8c,0x25,0xc9,0x94,0xbe,0xbe);


MIDL_DEFINE_GUID(CLSID, CLSID_Preview,0xf5615057,0x69da,0x44d3,0xa8,0x26,0x23,0x4a,0x3c,0x46,0x45,0x67);


MIDL_DEFINE_GUID(CLSID, CLSID_Thumbnail,0xd3a5d1c5,0xc0cd,0x4aa6,0xbe,0x3d,0x80,0xf4,0xf1,0x7f,0xe9,0x8b);


MIDL_DEFINE_GUID(CLSID, CLSID_Search,0xef06979e,0x136f,0x4259,0xb2,0x02,0x8f,0xfb,0xa5,0x5e,0x5a,0xa8);


MIDL_DEFINE_GUID(CLSID, CLSID_IconHandler,0x40f3183a,0x7102,0x4c59,0xa7,0x13,0x95,0x09,0x39,0x66,0x2d,0x93);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



