#include "MTPartAxis.h"

class MTStockAxis : public MTPartAxis
{
public:
	MTStockAxis();
	virtual ~MTStockAxis();
	MTStockAxis* Clone() const override;
	int GetPos() const;

	virtual AXIS_TYPE GetType() const override;
};