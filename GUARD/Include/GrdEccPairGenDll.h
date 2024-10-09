#ifndef __GRD_ECC_PAIR_GEN_DLL_H__
#define __GRD_ECC_PAIR_GEN_DLL_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus
      // Initialize recognition library
__declspec(dllexport) extern uint32_t __stdcall GenerateECCPair(unsigned char* privateCode, uint32_t privateLen, unsigned char* publicCode, uint32_t publicLen);
#ifdef __cplusplus
}
#endif //__cplusplus

#endif //__GRD_ECC_PAIR_GEN_DLL_H__