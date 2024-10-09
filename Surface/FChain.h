#pragma once
enum ChainType
{
	CHAIN_2D,
	CHAIN_2D_SMALL_ARC,
	CHAIN_2D_NEARSMALL_ARC,
	CHAIN_2D_SMALL_FULL_ARC,
	CHAIN_2D_FULL_ARC,
	CHAIN_VERTICAL,
	CHAIN_EMPTY,
	CHAIN_GENERIC,
	CHAIN_3D,
	CHAIN_ADDCOORD
};
class FChain
{
public:
	FChain(void);
	~FChain(void);
	enum ChainType GetType() const;
	void SetType(enum ChainType Type);
	bool ChainEndsBis;
	INT_PTR Start;
	INT_PTR End;
	INT_PTR MaxNumber;
	bool PrevChainEndsBis;
protected:
	enum ChainType Type;
};
