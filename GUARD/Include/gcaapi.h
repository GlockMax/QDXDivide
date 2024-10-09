/**
    GcaAPI.h
    
    Файл с описанием системных вызовов, доступных загружаемому приложению.
    Так же содержит некоторые макроопределения для использования в загружаемой микропрограмме.
    
    __KEIL__   - Определить, если сборка в Keil uVision (RealView MDK).
    __GNUC__   - Опрелелить, если сборка в ARM GCC.
    
    Aktiv Co. / 2008-2010
 */
#ifndef __GCA_API_H__
#define __GCA_API_H__

#ifndef NULL
    #define NULL ((void*)0)
#endif 

#ifndef TRUE
    #define TRUE    1    /**< TRUE */
#endif 

#ifndef FALSE
    #define FALSE   0    /**< FALSE */
#endif


/**
    KC - Kernel Calls
    Данные номера неявно используются в Startup.S.
 */
#define KC_GcaCrash                 0x00400000
#define KC_GcaExit                  0x00400001
#define KC_GcaLedOn                 0x00400002
#define KC_GcaLedOff                0x00400003
#define KC_GcaRead                  0x00400009
#define KC_GcaWrite                 0x0040000A
#define KC_GcaPI_Read               0x0040000B
#define KC_GcaPI_Update             0x0040000C
#define KC_GcaGetTime               0x0040000D
#define KC_GcaPI_GetTimeLimit       0x0040000E
#define KC_GcaPI_GetCounter         0x0040000F
#define KC_GcaGetLastError          0x00400010
#define KC_GcaGetRTCQuality         0x00400011
#define KC_GcaCryptEx               0x00400012
#define KC_GcaSign                  0x00400013
#define KC_GcaVerifySign            0x00400014
#define KC_GcaGenerateKeyPair       0x00400015
#define KC_GcaHash                  0x00400016
#define KC_GcaRandom                0x00400017
#define KC_GcaSetTimeout            0x00400018
#define KC_GcaCodeRun               0x00400019
#define KC_GcaCodeGetInfo           0x0040001A

#ifndef GrdSA_SoftAlgo
    #define GrdSA_SoftAlgo          0x80000000
#endif
#define GrdSC_AES128                (GrdSA_SoftAlgo + 1)

#define GrdAES128_CONTEXT_SIZE      (512+8+32+4+64)

#if defined(__WIN32__) || defined(_WIN32) || defined(WIN32)
    #define NO_INIT
    #define INLINE
    #define ALIGNED
#else
    #define HANDLE int
    #define GRD_API
    #define BOOL int

    #if !defined( __KEIL__ ) && !defined( __GNUC__ )
        #error Only Support KEIL RealView MDK or GCC
    #endif

    #if defined( __KEIL__ ) && defined( __GNUC__ )
        #error Only one of __KEIL__ or __GNUC__ directive may be defined
    #endif

    #if defined( __KEIL__ )
        /*
            INLINE
            Макроопределение для inline-функций. Рекомендуется 
            объявлять inline все простые функции.
         */
        #define INLINE __attribute__((always_inline))
        /*
            ALIGNED
            Макроопределение для выравнивания данных и более 
            эффективному доступу к ним. Требуется использовать 
            в объявлении структур. Можно использовать и для 
            выравнивания полей внутри структур.
         */
        #define ALIGNED __attribute__((aligned (4)))

        /*
            NO_INIT
            Секция, которая не проходит начальную инициализацию. 
            Используется для хранения буферов ввода-вывода и 
            для использования данных, которые будут сохраняться 
            между сеансами вызова загруженной микропрограммы
            (при условии отсутствия перебоев с питанием ключа).
         */
        #define NO_INIT __attribute__ ((section ("DataNoInit"),zero_init))

    #elif defined ( __GNUC__ )
        /*
            NO_INIT
            Секция, которая не проходит начальную инициализацию. 
            Используется для хранения буферов ввода-вывода и 
            для использования данных, которые будут сохраняться 
            между сеансами вызова загруженной микропрограммы
            (при условии отсутствия перебоев с питанием ключа).
         */
        #define NO_INIT __attribute__((section (".noinit")))
        /*
            INLINE
            Макроопределение для inline-функций. Рекомендуется 
            объявлять inline все простые функции.
         */
        #define INLINE __attribute__((always_inline))

        /*
            ALIGNED
            Макроопределение для выравнивания данных и более 
            эффективному доступу к ним. Требуется использовать 
            в объявлении структур. Можно использовать и для 
            выравнивания полей внутри структур.
         */
        #define ALIGNED __attribute__((aligned (4)))
    #endif
#endif

/** Определение инициализации отладочной библиотеки, внутри ключа не требуется. */
#define DEBUGDLL_INIT(hHandle, dwAlgoNum)

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
    syscall #0
    void GcaCrash(HANDLE hGrd);
    
    Аварийно завершает приложение. Вызывает возникновение ошибки 
    GrdE_gcException при выполнении GrdAPI-функции GrdCodeRun().
 */
#if defined( __KEIL__ )
    void __swi(KC_GcaCrash) GcaCrash
#elif defined( __GNUC__ ) || defined( WIN32 )
    extern void GcaCrash
#endif
(
  HANDLE hGrd // Reserved
);

/**
    syscall #1
    void GcaExit(HANDLE hGrd, DWORD dwRet);
    
    Завершает приложение. Параметр dwRet является кодом возврата приложения
    и возвращается в параметре pdwRet функции GrdCodeRun().
 */
#if defined( __KEIL__ )
    void __swi(KC_GcaExit) GcaExit
#elif defined( __GNUC__ ) || defined( WIN32 )
    extern void GRD_API GcaExit
#endif
(
  HANDLE hGrd, // Reserved
  DWORD dwRet // Return code
);


/** 
    syscall #2
    void GcaLedOn(void);
    
    Принудительно зажигает светодиод.
 */
#if defined( __KEIL__ )
    void __swi(KC_GcaLedOn) GcaLedOn
#elif defined( __GNUC__ ) || defined( WIN32 )
    extern void GcaLedOn
#endif
(
  HANDLE hGrd // Reserved
);


/** 
    syscall #3
    void GcaLedOff(void);
    
    Принудительно гасит светодиод.
 */
#if defined( __KEIL__ )
    void __swi(KC_GcaLedOff) GcaLedOff
#elif defined( __GNUC__ ) || defined( WIN32 )
    extern void GcaLedOff
#endif
(
  HANDLE hGrd // Reserved
);


/**
    syscall #9
    GcaRead()
 */
#if defined( __KEIL__ )
//  int __swi(KC_GcaRead) GRD_API GcaRead 
    extern int GcaRead
#elif defined( __GNUC__ ) || defined( WIN32 )
    extern int GcaRead
#endif
(
  HANDLE hGrd, // Reserved
  DWORD dwAddr,
  DWORD dwLng,
  void *pData,
  void *pReserved // Reserved
);


/**
    syscall #10
    GcaWrite()
 */
#if defined( __KEIL__ )
//  int __swi(KC_GcaWrite) GRD_API GcaWrite
    extern int GcaWrite
#elif defined( __GNUC__ ) || defined( WIN32 )
    extern int GcaWrite
#endif
(
  HANDLE hGrd, // Reserved
  DWORD dwAddr,
  DWORD dwLng,
  void *pData,
  void *pReserved // Reserved
);


/**
    syscall #11
    GcaPI_Read()
 */
#if defined( __KEIL__ )
//  int __swi(KC_GcaPI_Read) GRD_API GcaPI_Read
    extern int GcaPI_Read
#elif defined( __GNUC__ ) || defined( WIN32 )
    extern int GcaPI_Read
#endif
(
  HANDLE hGrd, // Reserved
  DWORD dwItemNum,
  DWORD dwAddr,
  DWORD dwLng,
  void *pData,
  DWORD dwReadPsw, // Reserved
  void *pReserved // Reserved
);


/**
    syscall #12
    GcaPI_Update()
 */
#if defined( __KEIL__ )
//  int __swi(KC_GcaPI_Update) GRD_API GcaPI_Update
    extern int GcaPI_Update
#elif defined( __GNUC__ ) || defined( WIN32 )
    extern int GcaPI_Update
#endif
(
  HANDLE hGrd, // Reserved
  DWORD dwItemNum,
  DWORD dwAddr,
  DWORD dwLng,
  void *pData,
  DWORD dwUpdatePsw, // Reserved
  DWORD dwMethod,
  void *pReserved // Reserved
);


/**
    syscall #13
    GcaGetTime()
 */
#if defined( __KEIL__ )
    int __swi(KC_GcaGetTime) GcaGetTime
#elif defined( __GNUC__ ) || defined( WIN32 )
    extern int GcaGetTime
#endif
(
  HANDLE hGrd, // Reserved
  TGrdSystemTime *pGrdSystemTime,
  void *pReserved // Reserved
);


/**
    syscall #14
    GcaPI_GetTimeLimit()
 */
#if defined( __KEIL__ )
    int __swi(KC_GcaPI_GetTimeLimit) GRD_API GcaPI_GetTimeLimit
#elif defined( __GNUC__ ) || defined( WIN32 )
    extern int GRD_API GcaPI_GetTimeLimit
#endif
(
  HANDLE hGrd, // Reserved
  DWORD dwItemNum,
  TGrdSystemTime *pGrdSystemTime,
  void *pReserved // Reserved
);


/**
    syscall #15
    GcaPI_GetCounter()
 */
#if defined( __KEIL__ )
    int __swi(KC_GcaPI_GetCounter) GRD_API GcaPI_GetCounter
#elif defined( __GNUC__ ) || defined( WIN32 )
    extern int GRD_API GcaPI_GetCounter
#endif
(
  HANDLE hGrd, // Reserved
  DWORD dwItemNum,
  DWORD *pdwCounter,
  void *pReserved // Reserved
);


/**
    syscall #16
    GcaGetLastError(hGrd, ppLastErrFunc);
    
    Функция возвращает последний код ошибки.
    Параметр ppLastErrFunc должен быть равен NULL.
 */
#if defined( __KEIL__ )
    int __swi(KC_GcaGetLastError) GRD_API GcaGetLastError
#elif defined( __GNUC__ ) || defined( WIN32 )
    extern int GRD_API GcaGetLastError
#endif 
(
  HANDLE hGrd, // Reserved
  void **ppLastErrFunc
);


/**
    syscall #17
    GcaGetRTCQuality()
    
    Получение статуса RTC.
    
    GrdE_NoService - RTC отсутствует.
    GrdE_RTCError  - Ошибка RTC, показания RTC некорректны.
 */
#if defined( __KEIL__ )
    BOOL __swi(KC_GcaGetRTCQuality) GcaGetRTCQuality
#elif defined( __GNUC__ ) || defined( WIN32 )
    extern BOOL GcaGetRTCQuality
#endif 
(
  HANDLE hGrd // Reserved
);

/**
    syscall #18
    GccaCryptEx()
    
    Основная функция шифрования.
 */
#if defined( __KEIL__ )
//  int __swi(KC_CryptEx) GccaCryptEx
    extern int GccaCryptEx
#elif defined( __GNUC__ ) || defined( WIN32 )
    extern int GccaCryptEx
#endif 
(
  HANDLE hGrd,  // Reserved
  DWORD dwAlgo, // либо номер ячейки либо GrdSC_AES128
  DWORD dwDataLng,
  void *pData,
  DWORD dwMethod,
  DWORD dwIVLng,
  void *pIV,
  void *pKeyBuf,
  void *pContext,
  void *pReserved // Reserved
);


/**
    syscall #19
    GccaSign()
    
    Генерация подписи, логика ECC-160.
 */
#if defined( __KEIL__ )
//  int __swi(KC_GcaSign) GccaSign
    extern int GccaSign
#elif defined( __GNUC__ ) || defined( WIN32 )
    extern int GccaSign
#endif 
(
  HANDLE hGrd, // Reserved
  DWORD dwAlgoNum, // либо номер ячейки либо GrdVSC_ECC160
  DWORD dwDataLng,
  void *pData,
  DWORD dwSignResultLng,
  void *pSignResult,
  void *pPrivateKey,
  void *pReserved // Reserved
);


/**
    syscall #20
    GccaVerifySign()
 */
#if defined( __KEIL__ )
//  int __swi(KC_GcaVerifySign) GccaVerifySign
    extern int GccaVerifySign
#elif defined( __GNUC__ ) || defined( WIN32 )
    extern int GccaVerifySign
#endif 
(
  HANDLE hGrd, // Reserved
  DWORD dwAlgoType,
  DWORD dwPublicKeyLng,
  void *pPublicKey,
  DWORD dwDataLng,
  void *pData,
  DWORD dwSignLng,
  void *pSign,
  void *pReserved // Reserved
);


/**
    syscall #21
    GccaGenerateKeyPair()
 */
#if defined( __KEIL__ )
//  int __swi(KC_GcaGenerateKeyPair) GccaGenerateKeyPair
    extern int GccaGenerateKeyPair
#elif defined( __GNUC__ ) || defined( WIN32 )
    extern int GccaGenerateKeyPair
#endif 
(
  HANDLE hGrd, // Reserved
  DWORD dwAlgoType,
  DWORD dwPrivateKeyLng,
  void *pPrivateKey,
  DWORD dwPublicKeyLng,
  void *pPublicKey
);


/**
    syscall #22
    GccaHash()
    
    Подсчет SHA-256.
    
    @note dwHash может принимать значение только GrdSH_SHA256.
 */
#if defined( __KEIL__ )
//  BYTE __swi(KC_GcaHash) GccaHash
    extern int GccaHash
#elif defined( __GNUC__ ) || defined( WIN32 )
    extern int GccaHash
#endif 
(
  HANDLE hGrd, // Reserved
  DWORD dwHash,
  DWORD dwDataLng,
  void *pData,
  DWORD dwMethod,
  void *pDigest,
  void *pKeyBuf,
  void *pContext
);


/**
    syscall #23
    GccaGetRandom()
 */
#if defined( __KEIL__ )
    int __swi(KC_GcaGetRandom) GccaGetRandom
#elif defined( __GNUC__ ) || defined( WIN32 )
    extern int GccaGetRandom
#endif 
(
  HANDLE hGrd, // Reserved
  BYTE *pbRand
);

/**
    syscall #24
 */
#if defined( __KEIL__ )
    int __swi(KC_GcaSetTimeout) GcaSetTimeout
#elif defined( __GNUC__ ) || defined( WIN32 )
    extern int GcaSetTimeout
#endif
(
  HANDLE hGrd,    // Reserved
  DWORD dwTimeout // TimeOut value in ms.
);

/**
    syscall #25
 */
#if defined( __KEIL__ )
//  int __swi(KC_GcaCodeRun) GcaCodeRun
    extern int GcaCodeRun
#elif defined( __GNUC__ ) || defined( WIN32 )
    extern int GcaCodeRun
#endif
(
  HANDLE  hGrd,                // Reserved
  DWORD   dwAlgoName,          // Algorithm numerical name to be loaded
  DWORD   dwP1,                // Parameter (subfunction code) for loadable code
  DWORD   *pdwRet,             // Return code of loadable code
  DWORD   dwDataFromDongleLng, // The amount of data to be received from the dongle
  void    *pDataFromDongle,    // Pointer to a buffer for data to be received from the dongle
  DWORD   dwDataToDongleLng,   // The amount of data to be sent to the dongle
  void    *pDataToDongle,      // Pointer to a buffer for data to be sent to the dongle
  void    *pReserved           // Reserved
);

#ifdef __cplusplus
}
#endif // __cplusplus

#if defined( __KEIL__ )
//  int __swi(KC_GcaCodeGetInfo) GcaCodeRun
    extern int GRD_API GcaCodeGetInfo
#elif defined( __GNUC__ ) || defined( WIN32 )
    extern int GRD_API GcaCodeGetInfo
#endif
(
  HANDLE  hGrd,       // Reserved
  DWORD   dwAlgoName, // Algorithm numerical name to be loaded
  DWORD   dwLng,
  void    *pBuf,
  void    *pReserved  // Reserved
);

#endif // __GCA_API_H__
