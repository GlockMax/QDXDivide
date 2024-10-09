#include "MTPartAxis.h"

class MTToolAxis : public MTPartAxis
{
public:
	MTToolAxis();
	virtual ~MTToolAxis();
	MTToolAxis* Clone() const override;

	virtual AXIS_TYPE GetType() const override;
	virtual int GetTurret();
	virtual int GetAttachment();
	virtual void SetTurret(int turret);
	virtual void SetAttachment(int attachment);
};