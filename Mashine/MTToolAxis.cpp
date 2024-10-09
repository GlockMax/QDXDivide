#include "stdafx.h"
#include "MTConfig.h"
#include "MTToolAxis.h"
#include "MTMachine.h"

MTToolAxis::MTToolAxis()
{
	LocalDisplVec = BPoint(0.,0.,0.,0.);
}

MTToolAxis::~MTToolAxis()
{
}
MTToolAxis* MTToolAxis::Clone() const
{
	return new MTToolAxis(*this);
}
//MTToolAxis::~MTToolAxis()

MTAxis::AXIS_TYPE MTToolAxis::GetType() const
{
	return AT_TOOL;
}//int MTToolAxis::GetType()

int MTToolAxis::GetTurret()
{
	return MTMachine::GetToolTurret(GetName());	
}//int MTToolAxis::GetTurret()

int MTToolAxis::GetAttachment()
{
	return MTMachine::GetToolAttachement(GetName());
}//int MTToolAxis::GetAttachment()

void MTToolAxis::SetTurret(int turret)
{
	SetName(MTConfig::GetToolAxisName(turret,GetAttachment()));
}//void MTToolAxis::SetTurret(int turret)

void MTToolAxis::SetAttachment(int attachment)
{
	SetName(MTConfig::GetToolAxisName(GetTurret(),attachment));
}//void MTToolAxis::SetAttachment(int attachment)