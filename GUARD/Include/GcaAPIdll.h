#ifndef __GCAAPIDLL_H__
#define __GCAAPIDLL_H__

#if defined(__WIN32__) || defined(_WIN32) || defined(WIN32)
    #define NO_INIT
    #define INLINE
    #define ALIGNED
#endif

#ifndef GrdSA_SoftAlgo
    #define GrdSA_SoftAlgo      0x80000000
#endif
#define GrdSC_AES128            (GrdSA_SoftAlgo + 1)

#define GrdAES128_CONTEXT_SIZE      (620)

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

void GcaExit
(
    HANDLE hGrd,
    DWORD dwRet
);

void GcaLedOn
(
    HANDLE hGrd
);

void GcaLedOff
(
    HANDLE hGrd
);

int GcaRead
(
    HANDLE hGrd,
    DWORD dwAddr,
    DWORD dwLng,
    void *pData,
    void *pReserved
);

int GcaWrite
(
    HANDLE hGrd,
    DWORD dwAddr,
    DWORD dwLng,
    void *pData,
    void *pReserved
);

int GcaPI_Read
(
    HANDLE hGrd,
    DWORD dwItemNum,
    DWORD dwAddr,
    DWORD dwLng,
    void *pData,
    DWORD dwReadPsw,
    void *pReserved
);

int GcaPI_Update
(
    HANDLE hGrd,
    DWORD dwItemNum,
    DWORD dwAddr,
    DWORD dwLng,
    void *pData,
    DWORD dwUpdatePsw,
    DWORD dwMethod,
    void *pReserved
);

int GcaGetTime
(
    HANDLE hGrd,
    TGrdSystemTime *pGrdSystemTime,
    void *pReserved
);

int GcaPI_GetTimeLimit
(
    HANDLE hGrd,
    DWORD dwItemNum,
    TGrdSystemTime *pGrdSystemTime,
    void *pReserved
);

int GcaPI_GetCounter
(
    HANDLE hGrd,
    DWORD dwItemNum,
    DWORD *pdwCounter,
    void *pReserved
);

int GcaGetLastError
(
    HANDLE hGrd,
    void **ppLastErrFunc
);

int GcaGetRTCQuality
(
    HANDLE hGrd
);

int GccaCryptEx
(
    HANDLE hGrd,
    DWORD dwAlgo,
    DWORD dwDataLng,
    void *pData,
    DWORD dwMethod,
    DWORD dwIVLng,
    void *pIV,
    void *pKeyBuf,
    void *pContext,
    void *pReserved
);

int GccaSign
(
    HANDLE hGrd,
    DWORD dwAlgoNum,
    DWORD dwDataLng,
    void *pData,
    DWORD dwSignResultLng,
    void *pSignResult,
    void *pPrivateKey,
    void *pReserved
);

int GccaVerifySign
(
    HANDLE hGrd,
    DWORD dwAlgoType,
    DWORD dwPublicKeyLng,
    void *pPublicKey,
    DWORD dwDataLng,
    void *pData,
    DWORD dwSignLng,
    void *pSign,
    void *pReserved
);

int GccaGenerateKeyPair
(
    HANDLE hGrd,
    DWORD dwAlgoType,
    DWORD dwPrivateKeyLng,
    void *pPrivateKey,
    DWORD dwPublicKeyLng,
    void *pPublicKey
);

int GccaHash
(
    HANDLE hGrd,
    DWORD dwHash,
    DWORD dwDataLng,
    void *pData,
    DWORD dwMethod,
    void *pDigest,
    void *pKeyBuf,
    void *pContext
);

int GccaGetRandom
(
    HANDLE hGrd,
    BYTE *pbRand
);

int GcaSetTimeout
(
    HANDLE hGrd,
    DWORD dwTimeout
);

int GcaCodeRun
(
    HANDLE hGrd,
    DWORD dwAlgoName,
    DWORD dwP1,
    DWORD *pdwRet,
    DWORD dwDataFromDongleLng,
    void *pDataFromDongle,
    DWORD dwDataToDongleLng,
    void *pDataToDongle,
    void *pReserved
);

int SetFuncAddresses
(
    HANDLE hHandle,
    DWORD dwAlgoNum,
    void *func1, 
    void *func2, 
    void *func3, 
    void *func4, 
    void *func5, 
    void *func6, 
    void *func7, 
    void *func8
);

int GcaCodeGetInfo
(
  HANDLE hGrd,
  DWORD dwAlgoName,
  DWORD dwLng,
  void *pBuf,
  void *pReserved
);

/**
    Макрос дляинициализации 
 */
#define DEBUGDLL_INIT(hHandle, dwAlgoNum)              \
            (GrdIsValidHandle(hHandle) ? (             \
            SetFuncAddresses(                          \
                hHandle,                               \
                dwAlgoNum,                             \
                GrdCodeRun,                            \
                NULL,                                  \
                GrdLock,                               \
                GrdSetWorkMode,                        \
                GrdUnlock,                             \
                GrdProtect,                            \
                GrdWrite,                              \
                GrdCodeLoad)) : 48)


#ifdef __cplusplus
}
#endif // __cplusplus

#endif //__GCAAPIDLL_H__
