/***************************************************************************
* (C) 1997-2004 Aktiv Co. All rights reserved.                            *
* Definitions for Guardant Stealth & Net libraries.                       *
***************************************************************************/

#ifndef __NSKEY32__H_
#define __NSKEY32__H_

#if defined(__WIN32__) || defined(_WIN32) || defined(WIN32) || defined(_WIN64)
#define NVSK_API            __stdcall
#elif defined(__GNUC__)
#define NVSK_API
#else
#error "This Guardant API supports 32/x64-bit compilers only"
#endif

#include "grdapi.h"

/*- Demo codes ----------------------------------------------------------*/
#define ns_DEMONVK 0x519175b7Lu  /* Demo public code                     */
#define ns_DEMORDO 0x51917645Lu  /* Demo private read code               */
#define ns_DEMOPRF 0x51917603Lu  /* Demo private write code              */
#define ns_DEMOMST 0x5191758cLu  /* Demo private master code             */

/*- Dongle Models -------------------------------------------------------*/
/* Old compatibility constants                                           */
#define nskm_GS          0       /* Guardant Stealth       LPT           */
#define nskm_GU          1       /* Guardant Stealth       USB           */
#define nskm_GF          2       /* Guardant Fidus         LPT           */
#define nskm_G2          3       /* Guardant StealthII     LPT           */

/* New constants                                                         */
#define nskm_GS1L  nskm_GS       /* Guardant Stealth       LPT           */
#define nskm_GS1U  nskm_GU       /* Guardant Stealth       USB           */
#define nskm_GF1L  nskm_GF       /* Guardant Fidus         LPT           */
#define nskm_GS2L  nskm_G2       /* Guardant StealthII     LPT           */
#define nskm_GS2U        4       /* Guardant StealthII     USB           */

#define nskm_Total       5       /* Total quantity of different models   */

/*- Dongle Interfaces ---------------------------------------------------*/
#define nski_LPT         0       /* LPT port                             */
#define nski_USB         1       /* USB bus                              */

/*- Dongle Types --------------------------------------------------------*/
#define nskt_DOS         0x0000  /* DOS Stealth Dongle                   */
#define nskt_Win         0x0000  /* Windows Stealth Dongle               */
#define nskt_LAN         0x0001  /* LAN Stealth Dongle                   */
#define nskt_Time        0x0002  /* Time Stealth Dongle                  */
#define nskt_GSII64      0x0008  /* Support of Guardant Stealth II 64 bit algorithm */

/*- Flags for nnkSetMode() ----------------------------------------------*/
/* Search for the dongle with specified                                  */
#define nsf_NProg        0x0001  /* wKeyNProg == wNProg                  */
#define nsf_ID           0x0002  /* dwKeyID == dwKeyID                   */
#define nsf_SN           0x0004  /* Serial Number                        */
#define nsf_Ver          0x0008  /* bKeyVersion >= bVer                  */
#define nsf_Mask         0x0010  /* wKeyMask & wMask == wMask            */
#define nsf_Type         0x0020  /* wKeyType & wType == wType            */
/* Input: Operation Mode Flags                                           */
#define nsf_SysAddrMode  0x0080  /* Enables SAM (System Address Mode) in */
/* read/write operations (UAM is a      */
/* default mode)                        */
#define nsf_CodeIsString 0x0100  /* Reserved (used by NNKCOMMAND()       */
/* function only)                       */
#define nsf_NoRetry      0x0200  /* Disables auto configuration of       */
/* communication protocol               */
#define nsf_NoFullAccess 0x0400  /* Disables full capture of the         */
/*  parallel port resource              */
#define nsf_OnlyStdLPT1  0x0800  /* Enables search for the key in LPT1   */
/* only (address 0x378)                 */
#define nsf_OnlyStdLPT2  0x1000  /* Enables search for the key in LPT2   */
/* only (address 0x278)                 */
#define nsf_OnlyStdLPT3  0x2000  /* Enables search for the key in LPT3   */
/* only (address 0x3BC)                 */
/* If none of these 3 flags is set, the */
/* key will be searched for in all LPT  */
/* ports available.                     */
#define nsf_NoAutoMem32  0x4000  /* Indicates that data segment is       */
/* different from the standard one used */
/* by NNKCOMMAND()function only in      */
/* 32-bit API                           */
#define nsf_UseOldCRC    0x8000  /* Reserved                             */

#define nsf_NotStdLPTAddr 0x02000000L

/*- UAM Addresses of Fields ---                                          */
#define uam_bNProg           (30 - 30) /* 00h Programm number              */
#define uam_bVer             (31 - 30) /* 01h Version                      */
#define uam_wSN              (32 - 30) /* 02h Serial number                */
#define uam_wMask            (34 - 30) /* 04h Bit mask                     */
#define uam_wGP              (36 - 30) /* 06h Counter #1 (GP)              */
#define uam_wRealLANRes      (38 - 30) /* 08h Current network license      */
/* limit                            */
#define uam_dwIndex          (40 - 30) /* 0Ah Index                        */
#define uam_abAlgoAddr       (44 - 30) /* 0Eh User data, algorithm         */
/* descriptors                      */

/*- SAM Addresses of Fields ---                                          */
/* Used by nsc_ChkNSK command                                            */
/* Fields protection against nsc_Init, nsc_Protect, nsc_Write commands   */
/*    * - Partial protection: nsc_Protect can be executed only after nsc_Init */
/*    X - Full protection                                                */
/* Protection against command:        Init Protect Write                 */
#define sam_bKeyModelAddr    0 /*  0h   X     X     X    0=GS,1=GU,2=GF  */
#define sam_bKeyMemSizeAddr  1 /*  1h   X     X     X    0=0, 8=256      */
#define sam_bKeyProgVerAddr  2 /*  2h   X     X     X                    */
#define sam_bKeyProtocolAddr 3 /*  3h   X     X     X                    */
#define sam_wClientVerAddr   4 /*  4h   X     X     X    0x104=1.4       */
#define sam_bKeyUserAddrAddr 6 /*  6h   X     X     X                    */
#define sam_bKeyAlgoAddrAddr 7 /*  7h   X     X     X                    */
#define sam_wPrnportAddr     8 /*  8h   X     X     X                    */

#define sam_dwPublicCode    14 /*  Eh   X     X     X                    */
#define sam_bVersion        18 /* 12h   X     X     X                    */
#define sam_bLANRes         19 /* 13h   X     X     X                    */
#define sam_wType           20 /* 14h   X     X     X                    */
#define sam_dwID            22 /* 16h   X     X     X                    */
#define sam_bWriteProtect   26 /* 1Ah         *     X                    */
#define sam_bReadProtect    27 /* 1Bh         *     X                    */
#define sam_bNumFunc        28 /* 1Ch         *     X                    */
#define sam_bTableLMS       29 /* 1Dh         *     X                    */
#define sam_bNProg          30 /* 1Eh   X     X                          */
#define sam_bVer            31 /* 1Fh   X     X                          */
#define sam_wSN             32 /* 20h   X     X                          */
#define sam_wMask           34 /* 22h   X     X                          */
#define sam_wGP             36 /* 24h   X     X                          */
#define sam_wRealLANRes     38 /* 26h   X     X                          */
#define sam_dwIndex         40 /* 28h   X     X                          */
#define sam_abAlgoAddr      44 /* 2Ch                                    */


/*- Guardant Stealth Algorithm Numbers ----------------------------------*/
#define nsan_AutoProtect   0   /* For automatic protection               */
#define nsan_Fast          1   /* For CodeInit (EnCode/DeCode) operation */
#define nsan_Random        2   /* Random number generator                */
#define nsan_DEMO          3   /* For Transform operation                */
#define nsan_GSII64        4   /* For GSII64 TransformEx operation       */

/*- Guardant Stealth Algorithm Request Size -----------------------------*/
#define nsars_AutoProtect  4   /* For automatic protection               */
#define nsars_Fast        32   /* For CodeInit (EnCode/DeCode) operation */
#define nsars_Random       4   /* Random number generator                */
#define nsars_DEMO         4   /* For Transform operation                */
#define nsars_GSII64       8   /* For GSII64 TransformEx operation       */

/*- Guardant Stealth Fast EnCode/DeCode Algorythm Types -----------------*/
#define nsat_Algo0         0   /* Basic method                           */
#define nsat_AlgoASCII     1   /* Character method                       */
#define nsat_AlgoFile      2   /* File method                            */

/*- Guardant Stealth TransformEx GSII64 algorithm use Method ------------*/
/*- bit 0-5 block chaining mode                                          */
#define nsam_ECB           0   /* Electronic Code Book                   */
#define nsam_CBC           1   /* Cipher Block Chaining                  */
#define nsam_CFB           2   /* Cipher Feed Back                       */
#define nsam_OFB           3   /* Output Feed Back                       */
/*- bit 7 - Encode/Decode                                                */
#define nsam_Encode        0   /* Encode mode                            */
#define nsam_Decode      128   /* Decode mode                            */

/*- CRC Starting value --------------------------------------------------*/
#define ns_StartCRC            -1l /* Starting value for 'continued' CRC */

/*- Error codes ---------------------------------------------------------*/
#define nse_Ok                  0 /* No errors                           */
#define nse_KeyNotFound         1 /* Dongle with specified search        */
/* conditions not found                */
/* 2 not used                          */
#define nse_AddressTooBig       3 /* The specified address is too big    */
/* 4 not used                          */
#define nse_GPis0               5 /* GP executions counter exhausted     */
/* (has 0 value)                       */
#define nse_BadCommand          6 /* Bad key call command                */
/* 7 not used                          */
#define nse_VerifyError         8 /* Write verification error            */
#define nse_LANProtNotFound     9 /* Network protocol not found          */
#define nse_LANResourceExhaust 10 /* License counter of Guardant Net     */
/* exhausted                           */
#define nse_ConnectionLoosed   11 /* Connection with Guardant Net server */
/* was lost                            */
#define nse_ConnectionLost     11 /* Connection with Guardant Net server */
/* was lost                            */
#define nse_LANplugsNotFound   12 /* Guardant Net server not found       */
#define nse_LANserverMemory    13 /* Guardant Net server memory          */
/* allocation error                    */
#define nse_LANDPMIError       14 /* Guardant Net server found DPMI      */
/* error                               */
#define nse_Internal           15 /* Guardant Net server internal error  */
#define nse_ServerReloaded     16 /* Guardant Net server has been        */
/* reloaded                            */
#define nse_VersionTooOld      17 /* This command is not supported by    */
/* this key version (the version is    */
/* too old)                            */
#define nse_BadDriver          18 /* Windows NT driver is required       */
#define nse_LANNetBIOS         19 /* Network protocol error              */
#define nse_LANpacket          20 /* Network packet format is not        */
/* supported                           */
#define nse_LANneedLogin       21 /* Logging in Guardant Net server is   */
/* required                            */
#define nse_LANneedLogout      22 /* Logging out from Guardant Net       */
/* server is required                  */
#define nse_LANKeyBusy         23 /* Guardant Net is busy locked by      */
/* another copy of protected           */
/* application                         */
#define nse_DriverBusy         24 /* Guardant driver cannot capture the  */
/* parallel port                       */
/* 25 - 29 not used                    */
#define nse_CRCError           30 /* CRC error occurred while attempting */
/* to call the key                     */
#define nse_CRCErrorRead       31 /* CRC error occurred while attempting */
/* to read data from the key           */
#define nse_CRCErrorWrite      32 /* CRC error occurred while attempting */
/* to write data to the key            */
#define nse_Overbound          33 /* The boundary of the key's memory    */
/* has been override                   */
#define nse_AlgoNotFound       34 /* The hardware algorithm with this    */
/* number has not been found in the    */
/* key                                 */
#define nse_CRCErrorFunc       35 /* CRC error of the hardware algorithm */
#define nse_CRCChkNSK          36 /* CRC error occurred while attempting */
/* to execute                          */
#define nse_ProtocolNotSup     37 /* ChkNSK operation, or all keys found */
/* Guardant API release is too old     */
#define nse_CnvTypeError       38 /* Non-existent reversible conversion  */
/* method has been specified           */
/* 39 - 45 reserved                    */
#define nse_InvalidArg         46 /* One or more arguments are invalid   */
#define nse_LastError          47

/* --- Guardant Stealth API functions Prototypes for C/C++               */
#ifdef __cplusplus
extern "C" {
#endif

/*--- Specify dongle search conditions and operation modes ---           */
void NVSK_API nskSetMode(DWORD dwFlags, BYTE bProg, DWORD dwID, \
                         WORD wSN, BYTE bVer, WORD wMask, WORD wType);

/*--- Obtain the ID of the first dongle found                            */
int NVSK_API nskFindFirst(DWORD dwPublic, DWORD* pdwID);

/*--- Obtain the ID of the next dongle found                             */
int NVSK_API nskFindNext(DWORD* pdwID);

/*--- Login to Guardant local dongle                                     */
int NVSK_API nskLogin(DWORD dwPrivateRD);

/*--- Logout from local dongle                                           */
int NVSK_API nskLogout(DWORD dwPrivateRD);

/*--- Check for availability of the dongle                               */
int NVSK_API nskCheck(DWORD dwPrivateRD);

/*--- Check for availability of the dongle and decrement GP executions counter*/
int NVSK_API nskDecGP(DWORD dwPrivateWR);

/*--- Transform a block of bytes using the dongle's hardware algorithm   */
int NVSK_API nskTransform(DWORD dwPrivateRD, BYTE bAlgoNum, \
                          BYTE bLng, void* pData);

/*--- Transform a block of bytes using the GSII64 dongle's hardware algorithm */
int NVSK_API nskTransformEx(DWORD dwPrivateRD, DWORD dwAlgoNum, \
                            DWORD dwLng, void* pData,
                            DWORD dwMethod, void* pIV);

/*--- Read a block of bytes from the dongle's memory                      */
int NVSK_API nskRead(DWORD dwPrivateRD, BYTE bAddr, \
                     BYTE bLng, void* pData);

/*--- Write a block of bytes into the dongle's memory                     */
int NVSK_API nskWrite(DWORD dwPrivateWR, BYTE bAddr, \
                      BYTE bLng, void* pData);

/*--- Initialize the dongle's memory                                      */
int NVSK_API nskInit(DWORD dwPrivateMST);

/*--- Implement locks / Specify the number of hardware algorithms         */
int NVSK_API nskProtect(DWORD dwPrivateMST, BYTE bWrProt, \
                        BYTE bRdProt, BYTE bNumFunc);

/*--- Implement locks / Specify the number of hardware algorithms         */
/*--- and LMS table address                                               */
int NVSK_API nskProtectLMS(DWORD dwPrivateMST, BYTE bWrProt, \
                           BYTE bRdProt, BYTE bNumFunc, BYTE bTableLMS);

/*--- Calculate 32-bit CRC of a memory block                              */
DWORD NVSK_API nskCRC(void* pData, DWORD dwLng, DWORD dwPrevCRC);

/*--- Initialize a password for fast reversible conversion                */
int NVSK_API nskCodeInit(DWORD dwPrivateRD, WORD wCnvType, \
                         BYTE bAddr, void* pKey);

/*---  Encode data using fast reversible conversion                       */
int NVSK_API nskEnCode(WORD wCnvType, void* pKeyBuf, \
                       void* pData, DWORD dwLng);

/*--- Decode data using fast reversible conversion                        */
int NVSK_API nskDeCode(WORD wCnvType, void* pKeyBuf, \
                       void* pData, DWORD dwLng);

/*------------------------------------------------------------------------*/
/*--- Guardant Net API functions prototypes for C/C++                     */
/*------------------------------------------------------------------------*/

/*--- Specify dongle search conditions and operation modes                */
void NVSK_API nnkSetMode(DWORD dwFlags, BYTE bProg, DWORD dwID, \
                         WORD wSN, BYTE bVer, WORD wMask, WORD wType);

/*--- Check for availability of the dongle                                */
int NVSK_API nnkCheck(DWORD dwPrivateRD);

/*--- Check for availability of the dongle and decrementing GP executions counter */
int NVSK_API nnkDecGP(DWORD dwPrivateWR);

/*--- Transform a block of bytes using the dongle's hardware algorithm    */
int NVSK_API nnkTransform(DWORD dwPrivateRD, BYTE bAlgoNum, \
                          BYTE bLng, void* pData);

/*--- Transform a block of bytes using GSII64 dongle's hardware algorithm */
int NVSK_API nnkTransformEx(DWORD dwPrivateRD, DWORD dwAlgoNum, \
                            DWORD dwLng, void* pData,
                            DWORD dwMethod, void* pIV);

/*--- Read a block of bytes from the dongle's memory                      */
int NVSK_API nnkRead(DWORD dwPrivateRD, BYTE bAddr, \
                     BYTE bLng, void* pData);

/*--- Write a block of bytes into the dongle's memory                     */
int NVSK_API nnkWrite(DWORD dwPrivateWR, BYTE bAddr, \
                      BYTE bLng, void* pData);

/*--- Initialize the dongle's memory                                      */
int NVSK_API nnkInit(DWORD dwPrivateMST);

/*--- Implement locks / Specify the number of hardware algorithms         */
int NVSK_API nnkProtect(DWORD dwPrivateMST, BYTE bWrProt, \
                        BYTE bRdProt, BYTE bNumFunc);

/*--- Implement locks / Specify the number of hardware algorithms         */
/*--- and LMS table address                                               */
int NVSK_API nnkProtectLMS(DWORD dwPrivateMST, BYTE bWrProt, \
                           BYTE bRdProt, BYTE bNumFunc, BYTE bTableLMS);

/*--- Calculate 32-bit CRC of a memory block                              */
DWORD NVSK_API nnkCRC(void* pData, DWORD dwLng, DWORD dwPrevCRC);

/*--- Initialize a password for fast reversible conversion                */
int NVSK_API nnkCodeInit(DWORD dwPrivateRD, WORD wCnvType, \
                         BYTE bAddr, void* pKey);

/*--- Encode data using fast reversible conversion                        */
int NVSK_API nnkEnCode(WORD wCnvType, void* pKeyBuf, \
                       void* pData, DWORD dwLng);

/*--- Decode data using fast reversible conversion                        */
int NVSK_API nnkDeCode(WORD wCnvType, void* pKeyBuf, \
                       void* pData, DWORD dwLng);

/*--- Login to Guardant Net server                                        */
int NVSK_API nnkLogin(DWORD dwPrivateRD);

/*--- Login to Guardant Net server to a specified resource                */
int NVSK_API nnkLoginLMS(DWORD dwPrivateRD, BYTE bModuleLMS);

/*--- Logout from Guardant Net server                                     */
int NVSK_API nnkLogout(DWORD dwPrivateRD);

/*--- Lock Guardant Net dongle                                            */
int NVSK_API nnkLockBeg(DWORD dwPrivateRD);

/*--- Unlock Guardant Net dongle                                          */
int NVSK_API nnkLockEnd(DWORD dwPrivateRD);

#if defined(_WINDOWS_) && (defined(__WIN32__) || defined(_WIN32) || defined(WIN32))
/*--- Must be called at the beginning of any Win32 DllMain function       */
int NVSK_API nnkDllMain(HINSTANCE hinstDLL, DWORD fdwReason, void* lpvReserved);
#endif

/*------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* __NSKEY32__H_ */
