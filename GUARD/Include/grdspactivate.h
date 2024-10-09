#ifndef __GRDSPCTIVATE_H__
#define __GRDSPCTIVATE_H__

#include <stdint.h>

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#define GRDSP_DECLARE(type) type __stdcall
#elif defined(linux) || defined(__linux__)
#define EXPORT __attribute__((visibility("default")))
#define GRDSP_DECLARE(type) type
#else
#error Unknown platform!
#endif

#define GrdSP_OK                                 0
#define GrdSP_InvalidSerialNumber                1   // Serial number is invalid
#define GrdSP_KeyResourceExhaust                 2   // Dongle activation resource is exhausted
#define GrdSP_SerialNumberBlocked                3   // Serial number is blocked by vendor
#define GrdSP_AlreadyActivated                   4   // License file has been already activated
#define GrdSP_InvalidVendor                      5   // Specified serial number is registered for another vendor
#define GrdSP_InvalidProgramNumber               6   // Specified serial number is registered for another product
#define GrdSP_ErrorTransferTemplate              7   // Unable to transfer data through the network
#define GrdSP_NoService                          8   // No service

#define GrdSP_SerialNumberLenWrong               101 // Invalid serial number length
#define GrdSP_InvalidArg                         102 // Invalid parameter specified in API function
#define GrdSP_InvalidFileFormat                  112 // Invalid license file format

#define GrdSP_DataNotFound                       120 // No data found
#define GrdSP_NoRequiredParams                   121 // No required parameters found
#define GrdSP_InvalidSerialNumberStatus          122 // Invalid serial number status
#define GrdSP_SystemError                        123 // System error
#define GrdSP_GetPCNError                        124 // Unable to get unique PCN
#define GrdSP_InvalidBufferSize                  125 // Invalid buffer size
#define GrdSP_ActivationError                    126 // Unable to activate container
#define GrdSP_InvalidVendorPassword              127 // Invalid access code
#define GrdSP_GuardantDongleError                128 // Guardant dongle error

#define GrdSP_TrialReactivationExpired           134 // Guardant SP Trial activation period expired
#define GrdSP_CantOpenFile                       135 // Unable to open the license file
#define GrdSP_NotEnoughtHardwareToBind           136 // Insufficient number of computer components to activate
#define GrdSP_WorkOnVMNotAllowed                 137 // Activation of the virtual machine is prohibited
#define GrdSP_DriverVersionTooLow                138 // Outdated Guardant drivers. Please, update the driver

#define GrdSP_ActivationRequestFailed            201 // Unable to send activation request
#define GrdSP_ActivationResponseFailed           202 // Unable to receive activation response
#define GrdSP_ActivationStatusNotFound           203 // Unable to find activation server
#define GrdSP_ActivationException                210 // Activation server internal error

#define GrdSP_InvalidCmdLineParameter            300 // Invalid command line parameter
#define GrdSP_ErrorLoadLicenseIntermediateFile   301 // Unable to load license intermediate file
#define GrdSP_SerialNumberIsNotSpecified         302 // Serial number is not specified
#define GrdSP_ErrorIniCurlLibrary                303 // Error initializing curl library
#define GrdSP_ErrorFileOperation                 304 // Error during an operation with a text file
#define GrdSP_LicenseFileNotFound                305 // License file not found
#define GrdSP_ExtractLicenseInfo                 306 // Unable to extract license info from license file
#define GrdSP_InvalidLicenseFile                 307 // License file is invalid or corrupted
#define GrdSP_InvalidLicenseDataFromResponse     308 // Unable to extract license data from response
#define GrdSP_DongleDoesNotExist                 309 // Guardant SP Dongle with specified ID does not exist

/////////////////////////////////////////////////////////////////////
// Online mode

// Make whole activation process
//  szGrdSPLicenseFileName  - License file name
//  szURL           - Server URL (optional, may be NULL).
//              Address by default is https://activation.guardant.ru/activationservice.svc
//  szSerialNumber  - License serial number
//  szProxyName     - Proxy name (optional, may be NULL)
//  szProxyPassword - Proxy password (optional, may be NULL)
EXPORT
GRDSP_DECLARE(int) ActivateGuardantSPDongleA(
                                             const char* szGrdSPLicenseFileName,
                                             const char* szURL,
                                             const char* szSerialNumber,
                                             const char* szProxyName,
                                             const char* szProxyPassword);

#ifdef _WIN32
EXPORT
GRDSP_DECLARE(int) ActivateGuardantSPDongleW(
                                             const wchar_t* szGrdSPLicenseFileName,
                                             const char* szURL,
                                             const char* szSerialNumber,
                                             const char* szProxyName,
                                             const char* szProxyPassword);
#endif // _WIN32

#ifdef UNICODE
#define ActivateGuardantSPDongle       ActivateGuardantSPDongleW
#else
#define ActivateGuardantSPDongle       ActivateGuardantSPDongleA
#endif // UNICODE

/////////////////////////////////////////////////////////////////////
// Offline mode

// Step 1: Prepare license file on target workstation to send it to a server
//  szGrdSPLicenseFileName  - License file name
//  szSerialNumber          - License serial number
//  szOutFileName           - Name of the license file to be sent to a server
//              (Optional, may be NULL). By default szGrdSPLicenseFileName + ".toserver"
EXPORT
GRDSP_DECLARE(int) PrepareSPLicenseToSendA(
                                           const char* szGrdSPLicenseFileName,
                                           const char* szSerialNumber,
                                           const char* szOutFileName);

#ifdef _WIN32
EXPORT
GRDSP_DECLARE(int) PrepareSPLicenseToSendW(
                                           const wchar_t* szGrdSPLicenseFileName,
                                           const char* szSerialNumber,
                                           const wchar_t* szOutFileName);
#endif // _WIN32

#ifdef UNICODE
#define PrepareSPLicenseToSend       PrepareSPLicenseToSendW
#else
#define PrepareSPLicenseToSend       PrepareSPLicenseToSendA
#endif // UNICODE

// Step 2: Send license file to a server on workstation with WAN
//  szGrdSPLicenseFileNameToServer  - File name of license prepared to be sent
//              to a server (with an additional '.toserver' extension)
//  szURL                 - Server URL (Optional, may be NULL). Address by default
//              is https://activation.guardant.ru/activationservice.svc
//  szProxyName           - Proxy name (Optional, may be NULL)
//  szProxyPassword       - Proxy password (Optional, may be NULL)
//  szOutFileName         - Name of the license file to activate (Optional, may be NULL).
//              By default szGrdSPLicenseFileName + ".fromserver"
EXPORT
GRDSP_DECLARE(int) SendSPLicenseFileToServerA(
                                              const char* szGrdSPLicenseFileNameToServer,
                                              const char* szURL,
                                              const char* szProxyName,
                                              const char* szProxyPassword,
                                              const char* szOutFileName);

#ifdef _WIN32
EXPORT
GRDSP_DECLARE(int) SendSPLicenseFileToServerW(
                                              const wchar_t* szGrdSPLicenseFileNameToServer,
                                              const char* szURL,
                                              const char* szProxyName,
                                              const char* szProxyPassword,
                                              const wchar_t* szOutFileName);
#endif // _WIN32

#ifdef UNICODE
#define SendSPLicenseFileToServer       SendSPLicenseFileToServerW
#else
#define SendSPLicenseFileToServer       SendSPLicenseFileToServerA
#endif // UNICODE

// Step 3: Activate license file received from a server on target workstation
//  szGrdSPLicenseFileNameFromServer  - File name of license received from a server
EXPORT
GRDSP_DECLARE(int) ActivateSPLicenseFileFromServerA(const char* szGrdSPLicenseFileNameFromServer);

#ifdef _WIN32
EXPORT
GRDSP_DECLARE(int) ActivateSPLicenseFileFromServerW(const wchar_t* szGrdSPLicenseFileNameFromServer);
#endif // _WIN32

#ifdef UNICODE
#define ActivateSPLicenseFileFromServer       ActivateSPLicenseFileFromServerW
#else
#define ActivateSPLicenseFileFromServer       ActivateSPLicenseFileFromServerA
#endif // UNICODE

// Get dongle ID from serial number string
// szSerialNumber               - [in] Serial number string
// dongleID                     - [out] Dongle id
EXPORT
GRDSP_DECLARE(int) GetSPDongleIdFromSerialNumber(const char* szSerialNumber, uint32_t * dongleID);

// Delete dongle by serial number or string with hex ID
// szSerialNumber               - [in] Serial number string
EXPORT
GRDSP_DECLARE(int) DeleteSPDongle(const char* szSerialNumber);

// Delete dongle by serial number or string with hex ID
// dongleID                     - [in] Dongle id
EXPORT
GRDSP_DECLARE(int) DeleteSPDongleByID(uint32_t dongleID);

// Backup working dongle to a file
// dongleID                     - [in] Dongle id
// szBackupFileName             - [in] Output file name of stored dongle
EXPORT
GRDSP_DECLARE(int) BackupSPDongleA(uint32_t dongleID, const char* szBackupFileName);

#ifdef _WIN32
EXPORT
GRDSP_DECLARE(int) BackupSPDongleW(uint32_t dongleID, const wchar_t* szBackupFileName);
#endif // _WIN32

#ifdef UNICODE
#define BackupSPDongle       BackupSPDongleW
#else
#define BackupSPDongle       BackupSPDongleA
#endif // UNICODE

// Restore dongle from a file
// szBackupFileName            - [in] File name of stored dongle
EXPORT
GRDSP_DECLARE(int) RestoreSPDongleA(const char* szBackupFileName);

#ifdef _WIN32
EXPORT
GRDSP_DECLARE(int) RestoreSPDongleW(const wchar_t* szBackupFileName);
#endif // UNICODE

#ifdef UNICODE
#define RestoreSPDongle       RestoreSPDongleW
#else
#define RestoreSPDongle       RestoreSPDongleA
#endif // UNICODE

// Get the time of dongle's first activation
// dongleID                    - [in] Dongle id
EXPORT
GRDSP_DECLARE(int) GetSPDongleFirstActivationDateA(uint32_t dongleID, uint64_t * firstActivationTime);

#ifdef _WIN32
EXPORT
GRDSP_DECLARE(int) GetSPDongleFirstActivationDateW(uint32_t dongleID, uint64_t * firstActivationTime);
#endif // _WIN32

#ifdef UNICODE
#define GetSPDongleFirstActivationDate       GetSPDongleFirstActivationDateW
#else
#define GetSPDongleFirstActivationDate       GetSPDongleFirstActivationDateA
#endif // UNICODE

// Check serial number for validness
// szSerialNumber      - [in] License serial number
// if returns 0 serial number is valid
EXPORT
GRDSP_DECLARE(int) CheckSPDongleSerialNumber(const char* szSerialNumber);

/////////////////////////////////////////////////////////////////////
// Automating the creation of license files without GrdUtil

// Additional params for the dongle mask
typedef struct
{
    uint32_t addrWrProt;        // SAM address of the first byte available for writing in bytes
    uint32_t addrRdProt;        // SAM address of the first byte available for reading in bytes
    uint32_t totalNumberOfAlgo; // Number of algorithms in the dongle including all protected items
    uint32_t numTableLMS;       // Protected item number that contains LMS Table

} GrdSPMaskParams;              // see GrdProtect for other comments

// General information about the license
typedef struct
{
    uint64_t orderTime;                        // Creation time as time_t (since 00:00 hours, Jan 1, 1970 UTC)
    uint64_t lastActivationTime;               // Last activation time as time_t (when creating should be zero, this time will be written later on the activation server)
    char szVendorCompanyName[256];             // Vendor company name
    char szVendorName[256];                    // Vendor name
    char szVendorProgramName[256];             // Vendor program name
    char szEndUserCompanyName[256];            // End-user company name
    char szEndUserName[256];                   // End-user name
    char szActivationServiceURL[256];          // Activation service URL. Address by default is https://activation.guardant.ru/activationservice.svc
    char szVendorEMail[256];                   // Vendor email
    uint32_t numberOfRemainActivations;        // The number of remaining activations (this field will be written on the activation server)
    uint32_t percentBinding;                   // Deprecated

} GrdSPLicenseInfo;

// License creation flags
#define GrdSP_Flag_ReadOnly                             1
#define GrdSP_Flag_Prevent_Execute_On_Virtual_Machine   4
#define GrdSP_Flag_Driver_Only                          8
#define GrdSP_Flag_Time_From_Inet_Only                  16 // get time from Interntet only (if Inet is unavailable - GrdE_RTC_Error returns)
#define GrdSP_Flag_Time_From_System_Only                32 // get time from OS only.
// Remark: if nor GrdSP_Flag_Time_From_Inet_Only nor GrdSP_Flag_Time_From_System_Only are set -
//  the time firstly will be tried to receive from Internet (and if Internet time is unavailable - system time returns)
#define GrdSP_Flag_Time_Back_Protection                 64

// Creates a non-activated license file
// pathToSPLicenseFileName   - [in] Full path to the destination license file
// pathToVendorFileName      - [in] Full path to the vendor private key-pair file (nvcodes.dat)
// flags                     - [in] Additional flags, see GrdSP_Flag_ReadOnly, GrdSP_Flag_Prevent_Execute_On_Virtual_Machine, GrdSP_Flag_Driver_Only,
// GrdSP_Flag_Time_From_Inet_Only, GrdSP_Flag_Time_From_System_Only
// maskDump                  - [in] Pointer to the dongle memory mask
// maskDumpSize              - [in] Size of dongle memory mask (max 3938 bytes)
// maskParams                - [in] Pointer to GrdSPMaskParams struct
// maskParamsSize            - [in] Size of GrdSPMaskParams struct
// hardwareBindMask          - [in] The mask used for hardware binding. It is 32 bit number. Bits from 0 to 5 are flags those responsible for
//      binding to specific PC components (0 - CPU, 1 - HDD, 2 - DVD, 3 - OS, 4 - Motherboard, 5 - MAC addres). Bits from 16 to 21 are flags
//      those responsible for possible substitution of PC components defined in bits 0-5, correspondingly.
// maxHardwareChangeNumber   - [in] The maximal number PC components allowed to be substituted without dognle breakdown. The 8 bit number that
//      defines how much PC components (marked as possible for substitution in bits 16-21 of 'hardwareBindMask' parameter) can be changed
//      in user's PC without dongle breakdown
// licenseInfo               - [in] Pointer to GrdSPLicenseInfo struct
// licenseInfoSize           - [in] Size of GrdSPLicenseInfo struct

EXPORT
GRDSP_DECLARE(int) CreateSPLicenseFileA(
                                        const char* pathToSPLicenseFileName,
                                        const char* pathToVendorFileName,
                                        uint32_t flags,
                                        const uint8_t * maskDump,
                                        uint32_t maskDumpSize,
                                        const GrdSPMaskParams * maskParams,
                                        uint32_t maskParamsSize,
                                        uint32_t hardwareBindMask,
                                        uint8_t maxHardwareChangeNumber,
                                        const GrdSPLicenseInfo * licenseInfo,
                                        uint32_t licenseInfoSize);

#ifdef _WIN32
EXPORT
GRDSP_DECLARE(int) CreateSPLicenseFileW(
                                        const wchar_t* pathToSPLicenseFileName,
                                        const wchar_t* pathToVendorFileName,
                                        uint32_t flags,
                                        const uint8_t * maskDump,
                                        uint32_t maskDumpSize,
                                        const GrdSPMaskParams * maskParams,
                                        uint32_t maskParamsSize,
                                        uint32_t hardwareBindMask,
                                        uint8_t maxHardwareChangeNumber,
                                        const GrdSPLicenseInfo * licenseInfo,
                                        uint32_t licenseInfoSize);
#endif // _WIN32

#ifdef UNICODE
#define CreateSPLicenseFile       CreateSPLicenseFileW
#else
#define CreateSPLicenseFile       CreateSPLicenseFileA
#endif // UNICODE

// Check license file for validness
// pathToSPLicenseFileName      - [in] Full path to the existing license file
// if returns 0 license file is valid
EXPORT
GRDSP_DECLARE(int) CheckSPLicenseFileA(const char* pathToSPLicenseFileName);

#ifdef _WIN32
EXPORT
GRDSP_DECLARE(int) CheckSPLicenseFileW(const wchar_t* pathToSPLicenseFileName);
#endif // _WIN32

#ifdef UNICODE
#define CheckSPLicenseFile       CheckSPLicenseFileW
#else
#define CheckSPLicenseFile       CheckSPLicenseFileA
#endif // UNICODE

// pathToSPLicenseFileName      - [in] Full path to the existing license file
// programNumberField           - [out] Pointer to output for program number field (see GrdUtil)
// publicCode                   - [out] Pointer to output for public code
// licenseInfo                  - [out] Pointer to output GrdSPLicenseInfo struct
// licenseInfoSize              - [in] Size of GrdSPLicenseInfo struct
EXPORT
GRDSP_DECLARE(int) ReadLicenseInfoA(
                                    const char* pathToSPLicenseFileName,
                                    uint32_t * programNumberField,
                                    uint32_t * publicCode,
                                    GrdSPLicenseInfo * licenseInfo,
                                    uint32_t licenseInfoSize);

#ifdef _WIN32
EXPORT
GRDSP_DECLARE(int) ReadLicenseInfoW(
                                    const wchar_t* pathToSPLicenseFileName,
                                    uint32_t * programNumberField,
                                    uint32_t * publicCode,
                                    GrdSPLicenseInfo * licenseInfo,
                                    uint32_t licenseInfoSize);
#endif // _WIN32

#ifdef UNICODE
#define ReadLicenseInfo       ReadLicenseInfoW
#else
#define ReadLicenseInfo       ReadLicenseInfoA
#endif // UNICODE


// Returns a unique name for the Guardant SP container by its ID.
// id                     - the Guardant SP dongle id
// pathToFileNameMaxLen   - maximum buffer length in characters
// pathToFileName         - pointer to file name output buffer
EXPORT
GRDSP_DECLARE(int) GetFileNameByIdA(
                                    uint32_t dongleID,
                                    uint32_t pathToFileNameMaxLen,
                                    char* pathToFileName);

#ifdef _WIN32
EXPORT
GRDSP_DECLARE(int) GetFileNameByIdW(
                                    uint32_t dongleID,
                                    uint32_t pathToFileNameMaxLen,
                                    wchar_t* pathToFileName);
#endif // _WIN32

#ifdef UNICODE
#define GetFileNameById       GetFileNameByIdW
#else
#define GetFileNameById       GetFileNameByIdA
#endif // UNICODE

#endif //__GRDSPCTIVATE_H__
