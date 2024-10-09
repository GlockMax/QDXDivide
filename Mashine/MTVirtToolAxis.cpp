#include "stdafx.h"
#include "MTConfig.h"
#include "MTVirtToolAxis.h"
#include "MTMachine.h"

MTVirtToolAxis::MTVirtToolAxis()
{
	LocalDisplVec = BPoint(0.,0.,0.,0.);
}

MTVirtToolAxis::~MTVirtToolAxis()
{
}
MTVirtToolAxis* MTVirtToolAxis::Clone() const
{
	return new MTVirtToolAxis(*this);
}
//MTVirtToolAxis::~MTVirtToolAxis()

MTAxis::AXIS_TYPE MTVirtToolAxis::GetType() const
{
	return AT_VTOOL;
}//int MTVirtToolAxis::GetType()

int MTVirtToolAxis::GetTurret()
{
	return MTMachine::GetToolTurret(GetName());
}//int MTVirtToolAxis::GetTurret()

int MTVirtToolAxis::GetAttachment()
{
	return MTMachine::GetToolAttachement(GetName());
}//int MTVirtToolAxis::GetAttachment()

int MTVirtToolAxis::GetPosition()
{
	return MTMachine::GetToolPosition(GetName());
}//int MTVirtToolAxis::GetPosition()

void MTVirtToolAxis::SetTurret(int turret)
{
	SetName(MTConfig::GetToolPlaceAxisName(GetPosition(),turret, GetAttachment()));
}//void MTVirtToolAxis::SetTurret(int turret)

void MTVirtToolAxis::SetAttachment(int attachment)
{
	SetName(MTConfig::GetToolPlaceAxisName(GetPosition(),GetTurret(), attachment));
}//void MTVirtToolAxis::SetAttachment(int attachment)
void MTVirtToolAxis::SetPosition(int position)
{
	SetName(MTConfig::GetToolPlaceAxisName(position ,GetTurret(), GetAttachment()));
}//void MTVirtToolAxis::SetPosition(int position)