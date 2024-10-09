#include "MTPartAxis.h"

class MTVirtToolAxis : public MTPartAxis
{
public:
	MTVirtToolAxis();
	virtual ~MTVirtToolAxis();
	MTVirtToolAxis* Clone() const override;

	virtual AXIS_TYPE GetType() const override;
	virtual int GetTurret();
	virtual int GetAttachment();
	virtual int GetPosition();
	virtual void SetTurret(int turret);
	virtual void SetAttachment(int attachment);
	virtual void SetPosition(int position);
};