#pragma once
class BStockGrID
{
	int val_;
public:
	BStockGrID() noexcept { Set(0);}
	int Get() const  noexcept { return val_; }
	const BStockGrID& Set(int val)  noexcept { val_ = val; return *this; }
	bool operator != (BStockGrID Gr) const  noexcept { return val_ != Gr.val_; }
	bool operator == (BStockGrID Gr) const  noexcept { return val_ == Gr.val_; }
	bool operator < (BStockGrID Gr) const  noexcept { return val_ < Gr.val_; }
};

