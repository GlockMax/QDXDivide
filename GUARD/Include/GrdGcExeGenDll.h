#ifndef __GRD_GCEXE_GEN_H___
#define __GRD_GCEXE_GEN_H___

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

__declspec(dllexport) extern uint32_t __stdcall GenerateGcExe(void* codeBin,          // compiled binary buffer
                                                              uint32_t binSize,       // size of compiled binary
                                                              void* codeBMap,         // .bmap file buffer
                                                              uint32_t mapSize,       // .bmap file size
                                                              void* publicKeyEncrypt, // public key of encryption pair
                                                              void* privateKeySign,   // private key of verification pair
                                                              uint32_t dwDongleID,    // dongle id (if 0 do not bind to ID)
                                                              void* outBuffer,        // output buffer
                                                              uint32_t* outBufferSize // output buffer size
                                                              );

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //__GRD_GCEXE_GEN_H___

