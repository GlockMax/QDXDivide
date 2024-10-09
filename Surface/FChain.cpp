#include "stdafx.h"
#include "FChain.h"

FChain::FChain(void)
{
	Type = CHAIN_GENERIC;
	ChainEndsBis = false;
	PrevChainEndsBis = false;
	Start = 0;
	End = 0;
	MaxNumber = 0;
}

FChain::~FChain(void)
{
}
enum ChainType FChain::GetType() const
{
	return Type;
}

void FChain::SetType(enum ChainType iType)
{
	Type = iType;
}
