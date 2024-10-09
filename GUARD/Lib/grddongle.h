#include "NCMB_API.h"

#ifndef GRD_DONGLE_H
#define GRD_DONGLE_H

#if defined (__GNUC__) || (defined (_MSC_VER) && _MSC_VER >= 1600)
#include <stdint.h>
#include <memory>
class CGrdDongle;
typedef std::shared_ptr<CGrdDongle> CGrdDonglePtr;
#elif (defined (__BORLANDC__))
#include <stdint.h>
#include <memory>
class CGrdDongle;
typedef std::auto_ptr<CGrdDongle> CGrdDonglePtr;
#else
#include <stdint.h>
#endif // _MSC_VER
#include <string>

#ifndef _WIN32
#include <unistd.h>
#include <pthread.h>
#endif // !_WIN32

#include "..\include\grdapi.h"

class NCMB_API CGrdMutex
{
public:
    CGrdMutex();
    virtual ~CGrdMutex();

private:
#ifdef _WIN32
    HANDLE m_hMutex;
#else
    static pthread_mutex_t m_hMutex;
#endif // _WIN32
};

class NCMB_API CGrdDongle
{
protected:
    /// Protected attributes
#ifdef _WIN32
    static long m_lGrdDongleClassCounter;
#else
    static uint32_t m_lGrdDongleClassCounter;
#endif // _WIN32
    static bool m_bGrdDongleStartup;
    uint8_t m_abyGrdContainer[GrdContainerSize];
    HANDLE m_hGrd;
    uint32_t m_dwRemoteMode;
    // parameters for call SetFindMode()
    uint32_t m_dwFlags;                     // Combination of GrdFM_XXXXX flags
    uint32_t m_dwProg;
    uint32_t m_dwID;
    uint32_t m_dwSN;
    uint32_t m_dwVer;
    uint32_t m_dwMask;
    uint32_t m_dwType;
    uint32_t m_dwModels;                    // Possible dongle model bits. See GrdFMM_GSX definition
    uint32_t m_dwInterfaces;                // Possible dongle interface bits. See GrdFMI_XXX definition
    uint32_t m_dwStartupFlags;              // See GrdRCS_XXXXX definition

    uint32_t m_dwDongleModel;               // dongle model after login
    std::string m_szNetworkClientIniPath;   // Path to gnClient.ini, depends on m_dwStartupFlags value
    bool m_useNotificationApi;              // use notification api (true by default)

private:
    /// startup stuff
    int DoStartup() const
    {
        CGrdMutex lock;
        if (!m_bGrdDongleStartup)
        {
            int nRet = ::GrdStartupEx(m_dwRemoteMode, m_szNetworkClientIniPath.c_str(), m_dwStartupFlags);
            if (nRet != GrdE_OK)
                return nRet;
#ifdef _WIN32
            if (m_useNotificationApi)
            {
                nRet = ::GrdInitializeNotificationAPI();
                if (nRet != GrdE_OK)
                {
                    ::GrdCleanup();
                    return nRet;
                }
            }
#endif      // _WIN32
            m_bGrdDongleStartup = true;
        }
        return GrdE_OK;
    }

    /// cleanup stuff
    void DoCleanup() const
    {
        CGrdMutex lock;
        if (--m_lGrdDongleClassCounter == 0 && m_bGrdDongleStartup)
        {
#ifdef _WIN32
            if (m_useNotificationApi)
                ::GrdUnInitializeNotificationAPI();
#endif      // _WIN32
            ::GrdCleanup();
            m_bGrdDongleStartup = false;
        }
    }

    /// copy and assignment not allowed
    CGrdDongle(const CGrdDongle& from);
    const CGrdDongle& operator=(const CGrdDongle& from);

public:
    /// Constructor
    CGrdDongle(
                                     // parameters for call GrdStartup() & GrdSetFindMode
               uint32_t dwRemoteMode = GrdFMR_Local + GrdFMR_Remote,
                                     // parameters for call SetFindMode()
               uint32_t dwFlags = 0, // Combination of GrdFM_XXXXX flags
               uint32_t dwProg = 0,
               uint32_t dwID = 0,
               uint32_t dwSN = 0,
               uint32_t dwVer = 0,
               uint32_t dwMask = 0,
               uint32_t dwType = 0,
               uint32_t dwModels = GrdFMM_ALL,               // Possible dongle model bits. See GrdFMM_GSX definition
               uint32_t dwInterfaces = GrdFMI_ALL,           // Possible dongle interface bits. See GrdFMI_XXX definition
               uint32_t dwStartupFlags = GrdRCS_UserDefined, // See GrdRCS_XXXXX definition
               const std::string& szNetworkClientIniPath = "",
               bool useNotificationApi = true)
        :  m_hGrd(NULL),
        m_dwRemoteMode(dwRemoteMode),
        m_dwFlags(dwFlags),
        m_dwProg(dwProg),
        m_dwID(dwID),
        m_dwSN(dwSN),
        m_dwVer(dwVer),
        m_dwMask(dwMask),
        m_dwType(dwType),
        m_dwModels(dwModels),
        m_dwInterfaces(dwInterfaces),
        m_dwStartupFlags(dwStartupFlags),
        m_dwDongleModel(0xffffffff),
        m_szNetworkClientIniPath(szNetworkClientIniPath),
        m_useNotificationApi(useNotificationApi)
    {
        CGrdMutex lock;
        ++m_lGrdDongleClassCounter;
    }

    static CGrdDonglePtr CreateDongle(
                                                            // parameters for call GrdStartup() & GrdSetFindMode
                                      uint32_t dwRemoteMode = GrdFMR_Local + GrdFMR_Remote,
                                                            // parameters for call SetFindMode()
                                      uint32_t dwFlags = 0, // Combination of GrdFM_XXXXX flags
                                      uint32_t dwProg = 0,
                                      uint32_t dwID = 0,
                                      uint32_t dwSN = 0,
                                      uint32_t dwVer = 0,
                                      uint32_t dwMask = 0,
                                      uint32_t dwType = 0,
                                      uint32_t dwModels = GrdFMM_ALL,               // Possible dongle model bits. See GrdFMM_GSX definition
                                      uint32_t dwInterfaces = GrdFMI_ALL,           // Possible dongle interface bits. See GrdFMI_XXX definition
                                      uint32_t dwStartupFlags = GrdRCS_UserDefined, // See GrdRCS_XXXXX definition
                                      const std::string& szNetworkClientIniPath = "",
                                      bool m_useNotificationApi = true)
    {
        CGrdDonglePtr grdDonglePtr(new CGrdDongle(dwRemoteMode, dwFlags, dwProg, dwID, dwSN, dwVer, dwMask, dwType,
                                                  dwModels, dwInterfaces, dwStartupFlags, szNetworkClientIniPath, m_useNotificationApi));
        return grdDonglePtr;
    }

    /// Destructor
    virtual ~CGrdDongle()
    {
        // Close hGrd handle. If need log out from dongle/server & free allocated memory
        if (m_hGrd)
        {
            int nRet = ::GrdCloseHandle(m_hGrd);
            if (nRet == GrdE_OK)
                m_hGrd = NULL;
        }
        DoCleanup();
    }

    /// Create class object. It call GrdStartup, GrdCreateHandle, GrdSetAccessCodes, GrdSetWorkMode, GrdSetFindMode
    /// and must by call after constructor and before any first other member function calls
    int Open(
             // parameters for call SetAccessCodes()
             uint32_t dwPublic,
             uint32_t dwPrivateRD,
             uint32_t dwPrivateWR = 0,
             uint32_t dwPrivateMST = 0,
                                                        // parameters for call SetWorkMode()
             uint32_t dwFlagsWork = GrdWM_UAM,          // combination of GrdWM_XXX flags
             uint32_t dwFlagsMode = GrdWMFM_DriverAuto) // combination of GrdWMFM_XXX flags
    {
        int nRet = DoStartup();
        if (nRet != GrdE_OK)
            return nRet;

        if (m_hGrd == NULL)
        {
            // Create Grd protected object
            m_hGrd = ::GrdCreateHandle(m_abyGrdContainer, GrdCHM_MultiThread, NULL);
            if (m_hGrd == NULL)
                return GrdE_Internal;
        }

        // Store dongle codes in Guardant protected container
        nRet = ::GrdSetAccessCodes(m_hGrd, dwPublic, dwPrivateRD, dwPrivateWR, dwPrivateMST);
        if (nRet != GrdE_OK)
            return nRet;
        // Set System Address Mode (SAM) as default mode
        nRet = ::GrdSetWorkMode(m_hGrd, dwFlagsWork, dwFlagsMode);
        if (nRet != GrdE_OK)
            return nRet;
        // Set dongle search parameters
        return ::GrdSetFindMode(m_hGrd, m_dwRemoteMode, m_dwFlags, m_dwProg, m_dwID, m_dwSN, m_dwVer, m_dwMask, m_dwType, m_dwModels, m_dwInterfaces);
    }

    /// Get last error information from protected container with specified handle
    /// The last error code is maintained on a per-handle basis
    /// Multiple handles do not overwrite each other's last-error code
    int GetLastError() const
    {
        return ::GrdGetLastError(m_hGrd, NULL);
    }

    /// Convert error code to text message string
    static std::string FormatErrMessage(int nErrorCode, // Guardant error code
                                        int nLanguage)  // Guardant language ID
    {
        char szBuf[1024];
        ::GrdFormatMessage(NULL, nErrorCode, nLanguage, szBuf, sizeof(szBuf), NULL);
        return std::string(szBuf);
    }

    /// Convert last error code (stored in handle) to text message string
    std::string FormatErrMessage(int nLanguage) const // Guardant language ID
    {
        char szBuf[1024];
        ::GrdFormatMessage(m_hGrd, 0, nLanguage, szBuf, sizeof(szBuf), NULL);
        return std::string(szBuf);
    }

    /// Get requested information
    int GetInfo(uint32_t dwInfoCode,       // Code of requested information. See GrdGIX_XXXXX
                void* pInfoData,           // Pointer to buffer for return data
                uint32_t dwInfoSize) const // Number of bytes for returning
    {
        return ::GrdGetInfo(m_hGrd, dwInfoCode, pInfoData, dwInfoSize);
    }
    /// template for get requested information of different types
    template<class Type>
    int GetInfo(uint32_t dwInfoCode, // Code of requested information. See GrdGIX_XXXXX
                Type* ptData) const  // Pointer to variable to be returned
    {
        return GetInfo(dwInfoCode, ptData, sizeof(*ptData));
    }

    uint32_t GetDongleModel()
    {
        if (m_dwDongleModel != 0xffffffff)
            return m_dwDongleModel;

        if (GetInfo(GrdGIL_Model, &m_dwDongleModel, sizeof(m_dwDongleModel)) != GrdE_OK)
            return 0xffffffff;

        switch (m_dwDongleModel)
        {
        case GrdDM_FidusUSB:
        case GrdDM_FidusLPT:
            m_dwDongleModel = GrdFMM_Fidus;
            break;
        case GrdDM_Stealth1LPT:
        case GrdDM_Stealth1USB:
            m_dwDongleModel = GrdFMM_Stealth1;
            break;
        case GrdDM_Stealth2LPT:
        case GrdDM_Stealth2USB:
            m_dwDongleModel = GrdFMM_Stealth2;
            break;
        case GrdDM_Stealth3USB:
            m_dwDongleModel = GrdFMM_Stealth3;
            break;
        case GrdDM_SignUSB:
            m_dwDongleModel = GrdFMM_SignUSB;
            break;
        case GrdDM_Soft:
            m_dwDongleModel = GrdFMM_Soft;
            break;
        case GrdDM_CodeUSB:
            m_dwDongleModel = GrdFMM_CodeUSB;
            break;
        default:
            m_dwDongleModel = 0xffffffff;
        }
        return m_dwDongleModel;
    }

    /// Store dongle codes in Guardant protected container
    int SetAccessCodes(uint32_t dwPublic,         // Must be already specified
                       uint32_t dwPrivateRD,      // Must be already specified
                       uint32_t dwPrivateWR = 0,  // == 0 if not needed
                       uint32_t dwPrivateMST = 0) // == 0 if not needed
    {
        return ::GrdSetAccessCodes(m_hGrd, dwPublic, dwPrivateRD, dwPrivateWR, dwPrivateMST);
    }

    /// Set dongle working mode to Guardant protected container
    int SetWorkMode(uint32_t dwFlagsWork = GrdWM_UAM,          // combination of GrdWM_XXX flags
                    uint32_t dwFlagsMode = GrdWMFM_DriverAuto) // combination of GrdWMFM_XXX flags
    {
        return ::GrdSetWorkMode(m_hGrd, dwFlagsWork, dwFlagsMode);
    }

    /// Set dongle search conditions and operation modes to Guardant protected container
    int SetFindMode(uint32_t dwRemoteMode = GrdFMR_Local + GrdFMR_Remote, // Local and/or remote (GrdFMR_Local + GrdFMR_Remote)
                    uint32_t dwFlags = 0,                                 // Combination of GrdFM_XXXXX flags
                    uint32_t dwProg = 0,
                    uint32_t dwID = 0,
                    uint32_t dwSN = 0,
                    uint32_t dwVer = 0,
                    uint32_t dwMask = 0,
                    uint32_t dwType = 0,
                    uint32_t dwModels = GrdFMM_ALL,      // Possible dongle model bits. See GrdFMM_GSX definition
                    uint32_t dwInterfaces = GrdFMI_ALL)  // Possible dongle interface bits. See GrdFMI_XXX definition
    {
        return ::GrdSetFindMode(m_hGrd, dwRemoteMode, dwFlags, dwProg, dwID, dwSN, dwVer, dwMask, dwType, dwModels, dwInterfaces);
    }

    int GetFindMode(uint32_t& dwRemoteMode, // Local and/or remote (GrdFMR_Local + GrdFMR_Remote)
                    uint32_t& dwFlags,      // Combination of GrdFM_XXXXX flags
                    uint32_t& dwProg,
                    uint32_t& dwID,
                    uint32_t& dwSN,
                    uint32_t& dwVer,
                    uint32_t& dwMask,
                    uint32_t& dwType,
                    uint32_t& dwModels,      // Possible dongle model bits. See GrdFMM_GSX definition
                    uint32_t& dwInterfaces)  // Possible dongle interface bits. See GrdFMI_XXX definition
    {
        int nRet = ::GrdGetInfo(m_hGrd, GrdGIF_Remote, &dwRemoteMode, sizeof(dwRemoteMode));
        if (GrdE_OK != nRet)
            return nRet;
        nRet = ::GrdGetInfo(m_hGrd, GrdGIF_Flags, &dwFlags, sizeof(dwFlags));
        if (GrdE_OK != nRet)
            return nRet;
        nRet = ::GrdGetInfo(m_hGrd, GrdGIF_Model, &dwModels, sizeof(dwModels));
        if (GrdE_OK != nRet)
            return nRet;
        nRet = ::GrdGetInfo(m_hGrd, GrdGIF_Interface, &dwInterfaces, sizeof(dwInterfaces));
        if (GrdE_OK != nRet)
            return nRet;
        nRet = ::GrdGetInfo(m_hGrd, GrdGIF_Ver, &dwVer, sizeof(dwVer));
        if (GrdE_OK != nRet)
            return nRet;
        nRet = ::GrdGetInfo(m_hGrd, GrdGIF_Prog, &dwProg, sizeof(dwProg));
        if (GrdE_OK != nRet)
            return nRet;
        nRet = ::GrdGetInfo(m_hGrd, GrdGIF_SN, &dwSN, sizeof(dwSN));
        if (GrdE_OK != nRet)
            return nRet;
        nRet = ::GrdGetInfo(m_hGrd, GrdGIF_ID, &dwID, sizeof(dwID));
        if (GrdE_OK != nRet)
            return nRet;
        nRet = ::GrdGetInfo(m_hGrd, GrdGIF_Type, &dwType, sizeof(dwType));
        if (GrdE_OK != nRet)
            return nRet;
        return ::GrdGetInfo(m_hGrd, GrdGIF_Mask, &dwMask, sizeof(dwMask));
    }

    /// Obtain the ID and other dongle info of the first or next dongle found
    int Find(uint32_t dwMode,                          // GrdF_First or GrdF_Next
             uint32_t* pdwID,                          // Pointer to variable for return dongle ID
             TGrdFindInfo* pFindInfo = NULL) const     // Place for return dongle info or NULL for ignore it in other languages
    {
        return ::GrdFind(m_hGrd, dwMode, reinterpret_cast<DWORD*>(pdwID), pFindInfo);
    }

    /// Login to Guardant dongle
    int Login(uint32_t dwModuleLMS = -1,                 // module number or -1 if License Management System functions are not used
              uint32_t dwLoginFlags = GrdLM_PerStation)  // Login flags (GrdLLM_PerStation or GrdLLM_PerHandle)
    {
        return ::GrdLogin(m_hGrd, dwModuleLMS, dwLoginFlags);
    }

    /// Log out from Guardant dongle
    int Logout()
    {
        return ::GrdLogout(m_hGrd, 0);
    }

    /// Increment lock counter of specified dongle
    int Lock(uint32_t dwTimeoutWaitForUnlock = 10000,    // Max GrdAPI unlock waiting time. -1 == infinity. 0 == no waiting
             uint32_t dwTimeoutAutoUnlock = 10000,       // Max dongle locking time in ms.  -1 == infinity. 0 == 10000 ms (10 sec)
             uint32_t dwMode = GrdLM_Nothing)            // in default works like critical section
    {
        return ::GrdLock(m_hGrd, dwTimeoutWaitForUnlock, dwTimeoutAutoUnlock, dwMode);
    }

    /// Unlock specified dongle
    int Unlock()
    {
        return ::GrdUnlock(m_hGrd);
    }

    /// Check for dongle availability
    int Check() const
    {
        return ::GrdCheck(m_hGrd);
    }

    /// Check for dongle availability and decrement GP executions counter
    /// Requires Private write code to be specified in SetAccessCodes
    int DecGP()
    {
        return ::GrdDecGP(m_hGrd);
    }

    /// GrdDongle fSeek analog
    /// Moves the dongle memory pointer (if any) associated with handle to a new location
    int Seek(uint32_t dwSeek)
    {
        return ::GrdSeek(m_hGrd, dwSeek);
    }

    /// Read a block of bytes from the dongle's memory
    int Read(uint32_t dwAddr,                        // Starting address in dongle memory to be read
             uint32_t dwLng,                         // Length of data to be read
             void* pData) const                      // Buffer for data to be read
    {
        return ::GrdRead(m_hGrd, dwAddr, dwLng, pData, NULL);
    }

    /// Read a variable from the dongle's memory
    template<class Type>
    int Read(uint32_t dwAddr,                        // Starting address in dongle memory to be read
             Type* ptData) const                     // Pointer to variable to be read
    {
        return Read(dwAddr, sizeof(*ptData), ptData);
    }

    /// Read a block of bytes from the dongle's memory
    int Read(uint32_t dwLng,                         // Length of data to be read
             void* pData) const                      // Buffer for data to be read
    {
        return Read(static_cast<uint32_t>(GrdSeekCur), dwLng, pData);
    }

    /// Read a variable from the dongle's memory
    template<class Type>
    int Read(Type* ptData) const                          // Pointer to variable to be read
    {
        return Read(static_cast<uint32_t>(GrdSeekCur), sizeof(*ptData), ptData);
    }

    /// Write a block of bytes into the dongle's memory
    /// Requires Private write code to be specified in SetAccessCodes
    int Write(uint32_t dwAddr,                          // Starting address in dongle memory to be written
              uint32_t dwLng,                           // Length of data to be written
              const void* pData)                        // Buffer for data to be written
    {
        return ::GrdWrite(m_hGrd, dwAddr, dwLng, pData, NULL);
    }

    /// write a variable into the dongle's memory
    template<class Type>
    int Write(uint32_t dwAddr,                        // Starting address in dongle memory to be written
              const Type* ptData)                     // Pointer to variable to be written
    {
        return Write(dwAddr, sizeof(*ptData), ptData);
    }

    /// Write a block of bytes into the dongle's memory
    /// Requires Private write code to be specified in SetAccessCodes
    int Write(uint32_t dwLng,                         // Length of data to be written
              const void* pData)                      // Buffer for data to be written
    {
        return Write(static_cast<uint32_t>(GrdSeekCur), dwLng, pData);
    }

    /// write a variable into the dongle's memory
    template<class Type>
    int Write(const Type* ptData)                         // Pointer to variable to be written
    {
        return Write(static_cast<uint32_t>(GrdSeekCur), sizeof(*ptData), ptData);
    }

    /// Initialize the dongle's memory
    /// Requires Private master code to be specified in SetAccessCodes
    int Init()
    {
        return ::GrdInit(m_hGrd);
    }

    /// Implement locks / Specify the number of hardware algorithms
    /// and LMS table address
    /// Requires Private master code to be specified in SetAccessCodes
    int Protect(uint32_t dwWrProt,                       // address of first BYTES which can by write in SAM
                uint32_t dwRdProt,                       // address of first BYTES which can by read  in SAM
                uint32_t dwNumFunc,                      // Number of hardware-implemented algorithms in the dongle
                uint32_t dwTableLMS = 0,                 // For Net II: SAM address of the first byte of LMS Table in 2-byte words; For Net III: number of protected item that contains LMS Table
                uint32_t dwGlobalFlags = 0)              // Reserved, must be 0
    {
        return ::GrdProtect(m_hGrd, dwWrProt, dwRdProt, dwNumFunc, dwTableLMS, dwGlobalFlags, NULL);
    }

    ///--- Cryptographic functions

    /// Transform a block of bytes using dongle's hardware-implemented algorithm (including GSII64)
    int Transform(uint32_t dwAlgoNum, uint32_t dwLng, void* pData,
                  uint32_t dwMethod = 0,  // GrdAM_ECB + GrdAM_Encrypt,// if Stealth I or Fidus it must be 0. Otherwise - combination of GrdAM_XXX flags
                  void* pIV = NULL) const // if Stealth I or Fidus it must be NULL. Otherwise - 8-bytes initialization vector
    {
        return ::GrdTransform(m_hGrd, dwAlgoNum, dwLng, pData, dwMethod, pIV);
    }

    /// Transform a block of bytes using dongle's hardware-implemented algorithm (including GSII64)
    int TransformEx(uint32_t dwAlgoNum, uint32_t dwLng, void* pData,
                    uint32_t dwMethod = 0,  // GrdAM_ECB + GrdAM_Encrypt,// if Stealth I or Fidus it must be 0. Otherwise - combination of GrdAM_XXX flags
                    uint32_t dwIVLng = 0,
                    void* pIV = NULL) const // if Stealth I or Fidus it must be NULL. Otherwise - n-bytes initialization vector
    {
        return ::GrdTransformEx(m_hGrd, dwAlgoNum, dwLng, pData, dwMethod, dwIVLng, pIV, NULL);
    }

    /// Encrypt/decrypt a block of bytes using encryption algorithm
    int Crypt(uint32_t dwAlgo,                       // Number of hardware- or software-implemented algorithm
              uint32_t dwDataLng,                    // Data length
              void* pData,                           // Data for Encryption/Decryption
              uint32_t dwMethod = 0,                 // Encrypt/Decrypt, First/Next/Last, block chaining modes (ECB/OFB/...)
              void* pIV = NULL,                      // Initialization Vector
              const void* pKeyBuf = NULL,            // Encryption/decryption secret key for software-implemented algorithm (NULL if not used)
              void* pContext = NULL) const           // Context for multiple-buffer encryption. Must be corresponded GrdXXXXXX_CONTEXT_SIZE bytes size
    {
        return ::GrdCrypt(m_hGrd, dwAlgo, dwDataLng, pData, dwMethod, pIV, pKeyBuf, pContext);
    }

    /// Encrypt/decrypt a block of bytes using encryption algorithm
    int CryptEx(uint32_t dwAlgo,                     // Number of hardware- or software-implemented algorithm
                uint32_t dwDataLng,                  // Data length
                void* pData,                         // Data for Encryption/Decryption
                uint32_t dwMethod = 0,               // Encrypt/Decrypt, First/Next/Last, block chaining modes (ECB/OFB/...)
                uint32_t dwIVLng = 0,
                void* pIV = NULL,                    // Initialization Vector
                const void* pKeyBuf = NULL,          // Encryption/decryption secret key for software-implemented algorithm (NULL if not used)
                void* pContext = NULL) const         // Context for multiple-buffer encryption. Must be corresponded GrdXXXXXX_CONTEXT_SIZE bytes size
    {
        return ::GrdCryptEx(m_hGrd, dwAlgo, dwDataLng, pData, dwMethod, dwIVLng, pIV, pKeyBuf, pContext, NULL);
    }

    /// Hash calculation of a block of bytes
    int Hash(uint32_t dwSoftHash,                          // Number of hardware- or software-implemented algorithm
             uint32_t dwDataLng,                           // Data length
             const void* pData,                            // Data for hash calculation
             uint32_t dwMethod,                            // GrdSC_First/GrdSC_Next/GrdSC_Last
             void* pDigest,                                // Pointer to memory allocated for hash on GrdSC_Last step
             const void* pKeyBuf = NULL,                   // Hash calculation secret key for software-implemented algorithm (NULL if not used)
             void* pContext = NULL)                        // Context for multiple buffer calculation. Must be corresponded GrdXXXXXX_CONTEXT_SIZE bytes size
    {
        return ::GrdHash(m_hGrd, dwSoftHash, dwDataLng, pData, dwMethod, pDigest, pKeyBuf, pContext);
    }

    /// Hash calculation of a block of bytes
    int HashEx(uint32_t dwSoftHash,                        // Number of hardware- or software-implemented algorithm
               uint32_t dwDataLng,                         // Data length
               const void* pData,                          // Data for hash calculation
               uint32_t dwMethod,                          // GrdSC_First/GrdSC_Next/GrdSC_Last
               uint32_t dwDigestLng,                       // Digest length
               void* pDigest,                              // Pointer to memory allocated for hash on GrdSC_Last step
               uint32_t dwKeyBufLng,                       // Not used, must be 0
               const void* pKeyBuf,                        // Hash calculation secret key for software-implemented algorithm (NULL if not used)
               uint32_t dwContextLng,                      // Context length
               void* pContext)                             // Context for multiple buffer calculation. Must be corresponded GrdXXXXXX_CONTEXT_SIZE bytes size
    {
        return ::GrdHashEx(m_hGrd, dwSoftHash, dwDataLng, pData, dwMethod, dwDigestLng, pDigest, dwKeyBufLng, pKeyBuf, dwContextLng, pContext, NULL);
    }

    /// Calculate 32-bit CRC of a memory block
    static uint32_t CRC(const void* pData, uint32_t dwLng, uint32_t dwPrevCRC = Grd_StartCRC)
    {
        return ::GrdCRC(pData, dwLng, dwPrevCRC);
    }

    ///--- Protected Item functions

    /// Activate dongle Algorithm or Protected Item
    int PI_Activate(uint32_t dwItemNum,               // Algorithm or Protected Item number to be activated
                    uint32_t dwActivatePsw = 0)       // Optional password. If not used, must be 0
    {
        return ::GrdPI_Activate(m_hGrd, dwItemNum, dwActivatePsw);
    }

    /// Deactivate dongle Algorithm or Protected Item
    int PI_Deactivate(uint32_t dwItemNum,              // Algorithm or Protected Item number to be deactivated
                      uint32_t dwDeactivatePsw = 0)    // Optional password. If not used, must be 0
    {
        return ::GrdPI_Deactivate(m_hGrd, dwItemNum, dwDeactivatePsw);
    }

    /// Read data from dongle Protected Item
    int PI_Read(uint32_t dwItemNum,                     // Algorithm or Protected Item number to be read
                uint32_t dwAddr,                        // Offset in Algorithm or Protected Item data
                uint32_t dwLng,                         // Number of bytes for reading
                void* pData,                            // Pointer to buffer for read data
                uint32_t dwReadPsw = 0) const           // Optional password. If not used, must be 0
    {
        return ::GrdPI_Read(m_hGrd, dwItemNum, dwAddr, dwLng, pData, dwReadPsw, NULL);
    }

    /// Update data in dongle Protected Item
    int PI_Update(uint32_t dwItemNum,                       // Algorithm or Protected Item number to be updated
                  uint32_t dwAddr,                          // Offset in Algorithm or Protected Item data
                  uint32_t dwLng,                           // Number of bytes for updating
                  const void* pData,                        // Pointer to buffer with data to be written
                  uint32_t dwUpdatePsw = 0,                 // Optional password. If not used, must be 0
                  uint32_t dwMethod = GrdUM_MOV)            // Update method. See GrdUM_XXX definitions
    {
        return ::GrdPI_Update(m_hGrd, dwItemNum, dwAddr, dwLng, pData, dwUpdatePsw, dwMethod, NULL);
    }

    /// --- Guardant Trusted Remote Update API

    /// Write secret GSII64 remote update key for Guardant Secured Remote Update to the dongle
    /// Requires Private master code to be specified in SetAccessCodes
    int TRU_SetKey(const void* pGSII64_Key128)                  // Unique Trusted Remote Update GSII64 128-bit secret key
    {
        return ::GrdTRU_SetKey(m_hGrd, pGSII64_Key128);
    }

    /// Generate encrypted question and initialize remote update procedure
    int TRU_GenerateQuestion(void* pQuestion,            // Pointer to Question                  8 bytes (64 bit)
                             uint32_t* pdwID,            // Pointer to ID                        4 bytes
                             uint32_t* pdwPublic,        // Pointer to Dongle Public Code        4 bytes
                             void* pHash) const          // Pointer to Hash of previous 16 bytes 8 bytes
    {
        return ::GrdTRU_GenerateQuestion(m_hGrd, pQuestion, reinterpret_cast<DWORD*>(pdwID),
                                         reinterpret_cast<DWORD*>(pdwPublic), pHash);
    }

    /// Generate encrypted question and initialize remote update procedure
    int TRU_GenerateQuestionTime(void* pQuestion,         // Pointer to Question                  8 bytes (64 bit)
                                 uint32_t* pdwID,         // Pointer to ID                        4 bytes
                                 uint32_t* pdwPublic,     // Pointer to Dongle Public Code        4 bytes
                                 uint64_t* pqwDongleTime, // Pointer to Dongle Time (encrypted)   8 bytes
                                 uint32_t dwDeadTimesSize,
                                 uint64_t* pqwDeadTimes,
                                 uint32_t* pdwDeadTimesNumbers,
                                 void* pHash) const   // Pointer to Hash of previous data
    {
        return ::GrdTRU_GenerateQuestionTime(m_hGrd, pQuestion, reinterpret_cast<DWORD*>(pdwID),
                                             reinterpret_cast<DWORD*>(pdwPublic), pqwDongleTime, dwDeadTimesSize, pqwDeadTimes,
                                             reinterpret_cast<DWORD*>(pdwDeadTimesNumbers), pHash, NULL);
    }

    /// Decrypt and validate question
    int TRU_DecryptQuestion(uint32_t dwAlgoNum_GSII64, // Dongle GSII64 algorithm number with same key as in remote dongle
                            uint32_t dwAlgoNum_HashS3, // Dongle HASH algorithm number with same key as in remote dongle
                            void* pQuestion,           // Pointer to Question                  8 bytes (64 bit)
                            uint32_t dwID,             // ID                                   4 bytes
                            uint32_t dwPublic,         // Public Code                          4 bytes
                            const void* pHash) const   // Pointer to Hash of previous 16 bytes 8 bytes
    {
        return ::GrdTRU_DecryptQuestion(m_hGrd, dwAlgoNum_GSII64, dwAlgoNum_HashS3, pQuestion, dwID, dwPublic, pHash);
    }

    /// Decrypt and validate question
    int TRU_DecryptQuestionTime(                           // GSII64 algorithm with the same key as in remote dongle
                                uint32_t dwAlgoNum_GSII64, // Dongle GSII64 algorithm number with same key as in remote dongle
                                uint32_t dwAlgoNum_HashS3, // Dongle HASH64 algorithm number with same key as in remote dongle
                                void* pQuestion,           // Pointer to Question                  8 bytes (64 bit)
                                uint32_t dwID,             // ID                                   4 bytes
                                uint32_t dwPublic,         // Public Code                          4 bytes
                                uint64_t* pqwDongleTime,   // Pointer to Dongle Time (encrypted)   8 bytes
                                uint64_t* pqwDeadTimes,
                                uint32_t dwDeadTimesNumbers,
                                const void* pHash) const // Pointer to Hash of previous 16 bytes 8 bytes
    {
        return ::GrdTRU_DecryptQuestionTime(m_hGrd, dwAlgoNum_GSII64, dwAlgoNum_HashS3, pQuestion, dwID, dwPublic, pqwDongleTime,
                                            pqwDeadTimes, dwDeadTimesNumbers, pHash);
    }

    /// Set Init & Protect parameters for Trusted Remote Update
    /// This function must be called after GrdTRU_DecryptQuestion and before GrdTRU_EncryptAnswer functions
    /// only if Init & Protect operations will be executed during remote update (call GrdTRU_ApplyAnswer) procedure on remote PC
    int TRU_SetAnswerProperties(uint32_t dwTRU_Flags,       // Use Init & Protect or not
                                uint32_t dwReserved,        // Reserved, must be 0
                                uint32_t dwWrProt,          // remote GrdProtect parameters, SAM addres of the first byte available for writing in bytes
                                uint32_t dwRdProt,          // remote GrdProtect parameters, SAM addres of the first byte available for reading in bytes
                                uint32_t dwNumFunc,         // remote GrdProtect parameters, Number of hardware-implemented algorithms in the dongle including all protected items and LMS table of Net III
                                uint32_t dwTableLMS = 0,    // remote GrdProtect parameters, Net II: SAM address of the first byte of LMS Table in 2-byte words;
                                                            // Net III: number of protected item that contains LMS Table
                                uint32_t dwGlobalFlags = 0) // remote GrdProtect parameters, Reserved, must be 0
    {
        return ::GrdTRU_SetAnswerProperties(m_hGrd, dwTRU_Flags, dwReserved, dwWrProt, dwRdProt, dwNumFunc, dwTableLMS,
                                            dwGlobalFlags, NULL);
    }

    /// Prepare data for Trusted Remote Update
    int TRU_EncryptAnswer(uint32_t dwAddr,               // Starting address for writing in dongle
                          uint32_t dwLng,                // Size of data to be written
                          const void* pData,             // Buffer for data to be written
                          const void* pQuestion,         // Pointer to decrypted Question            8 bytes (64 bit)
                          uint32_t dwAlgoNum_GSII64,     // Dongle GSII64 algorithm number with the same key as in remote dongle
                          uint32_t dwAlgoNum_HashS3,     // Dongle HASH algorithm number with the same key as in remote dongle
                          void* pAnswer,                 // Pointer to the buffer for Answer data
                          uint32_t* pdwAnswerSize) const // IN: Maximum buffer size for Answer data, OUT: Size of pAnswer buffer
    {
        return ::GrdTRU_EncryptAnswer(m_hGrd, dwAddr, dwLng, pData, pQuestion, dwAlgoNum_GSII64, dwAlgoNum_HashS3,
                                      pAnswer, reinterpret_cast<DWORD*>(pdwAnswerSize));
    }

    /// Write data from secured buffer received via remote update procedure
    int TRU_ApplyAnswer(const void* pAnswer,   // Answer data update buffer prepared and encrypted by GrdTRU_EncryptAnswer
                        uint32_t dwAnswerSize) // Size of pAnswer buffer
    {
        return ::GrdTRU_ApplyAnswer(m_hGrd, pAnswer, dwAnswerSize);
    }

    /// Digitally sign a block of bytes by using dongle hardware implemented ECC algorithm
    int Sign(uint32_t dwAlgoNum,                // Number of hardware implemented ECC algorithm
             uint32_t dwDataLng,                // Data for sign length (20 bytes for ECC160)
             const void* pData,                 // Data for sign
             uint32_t dwSignResultLng,          // ECC sign length (40 bytes for ECC160)
             void* pSignResult) const           // ECC sign
    {
        return ::GrdSign(m_hGrd, dwAlgoNum, dwDataLng, pData, dwSignResultLng, pSignResult, NULL);
    }

    /// ECC algorithm digest verifying. Full software implemented
    static int VerifySign(uint32_t dwAlgoType,     // Type of asymmetric cryptoalgorithm. See GrdVSC_XXXXX definition
                          uint32_t dwPublicKeyLng, // Public ECC key length
                          const void* pPublicKey,  // Public ECC key
                          uint32_t dwDataLng,      // Data for sign length (20 bytes for ECC160)
                          const void* pData,       // Data for sign
                          uint32_t dwSignLng,      // ECC sign length (40 bytes for ECC160)
                          const void* pSign)       // ECC sign
    {
        return ::GrdVerifySign(NULL, dwAlgoType, dwPublicKeyLng, pPublicKey, dwDataLng, pData, dwSignLng, pSign, NULL);
    }

    /// Set dongle system time
    int SetTime(const TGrdSystemTime* pGrdSystemTime)     // Pointer to TGrdSystemTime
    {
        return ::GrdSetTime(m_hGrd, pGrdSystemTime, NULL);
    }

    /// Get dongle system time
    int GetTime(TGrdSystemTime* pGrdSystemTime) const     // Pointer to TGrdSystemTime
    {
        return ::GrdGetTime(m_hGrd, pGrdSystemTime, NULL);
    }

    /// Get time limit for specified item
    int PI_GetTimeLimit(uint32_t dwItemNum,                   // Algorithm or Protected Item number
                        TGrdSystemTime* pGrdSystemTime) const // Pointer to TGrdSystemTime
    {
        return ::GrdPI_GetTimeLimit(m_hGrd, dwItemNum, pGrdSystemTime, NULL);
    }

    /// Get counter for specified item
    int PI_GetCounter(uint32_t dwItemNum,         // Algorithm or Protected Item number
                      uint32_t* pdwCounter) const // Pointer to counter value
    {
        return ::GrdPI_GetCounter(m_hGrd, dwItemNum, reinterpret_cast<DWORD*>(pdwCounter), NULL);
    }

    /// Get information from user-defined loadable code descriptor
    int CodeGetInfo(uint32_t dwAlgoName,        // Algorithm numerical name to be loaded
                    TGrdCodeInfo& grdCodeInfo)
    {
        return ::GrdCodeGetInfo(m_hGrd, dwAlgoName, sizeof(TGrdCodeInfo), &grdCodeInfo, NULL);
    }

    /// Load GCEXE file to the dongle
    int CodeLoad(uint32_t dwAlgoName,                // Algorithm numerical name to be loaded
                 uint32_t dwFileSize,                // Buffer size for GCEXE-file
                 const void* pFileBuf)               // Pointer to the buffer for GCEXE-file
    {
        return ::GrdCodeLoad(m_hGrd, dwAlgoName, dwFileSize, pFileBuf, NULL);
    }

    /// Run user-defined loadable code
    int CodeRun(uint32_t dwAlgoName,                 // Algorithm numerical name to be loaded
                uint32_t dwP1,                       // Parameter (subfunction code) for loadable code
                uint32_t* pdwRet,                    // Return code of loadable code
                uint32_t dwDataFromDongleLng,        // The amount of data to be received from the dongle
                void* pDataFromDongle,               // Pointer to a buffer for data to be received from the dongle
                uint32_t dwDataToDongleLng,          // The amount of data to be sent to the dongle
                const void* pDataToDongle)           // Pointer to a buffer for data to be sent to the dongle
    {
        return ::GrdCodeRun(m_hGrd, dwAlgoName, dwP1, reinterpret_cast<DWORD*>(pdwRet), dwDataFromDongleLng, pDataFromDongle,
                            dwDataToDongleLng, pDataToDongle, NULL);
    }

    /// Switching driver type of USB-dongle
    int SetDriverMode(uint32_t dwMode)                    // New Guardant dongle USB driver mode. See GrdDM_XXX definitions
    {
        return ::GrdSetDriverMode(m_hGrd, dwMode, NULL);
    }

#ifdef _WIN32

    // manual InitializeNotificationAPI
    int InitializeNotificationAPI()
    {
        m_useNotificationApi = true;
        return ::GrdInitializeNotificationAPI();
    }

    /// Register dongle notification for specified handle
    int RegisterDongleNotification(GrdDongleNotifyCallBack* pCallBack) const // Pointer to Dongle Notification CallBack routine
    {
        if (m_useNotificationApi)
            return ::GrdRegisterDongleNotification(m_hGrd, pCallBack);
        return GrdE_OK;
    }

    /// Unregister dongle notification for specified handle
    int UnRegisterDongleNotification() const
    {
        if (m_useNotificationApi)
            return ::GrdUnRegisterDongleNotification(m_hGrd);
        return GrdE_OK;
    }
#endif // _WIN32

};

#endif // !GRD_DONGLE_H
