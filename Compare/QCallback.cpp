#include "stdafx.h"
#include "QCallback.h"


QCallback::QCallback(QComparison &iComparison) : Comparison(iComparison)
{
}


QCallback::~QCallback()
{
}

void QCallback::update(float percent)
{
	Comparison.progress = int(percent);
}

void QCallback::setMethodTitle(const char * methodTitle)
{
}

void QCallback::setInfo(const char * infoStr)
{
}

void QCallback::start()
{
}

void QCallback::stop()
{
}

bool QCallback::isCancelRequested()
{
	return Comparison.IsTerminationRequested();
}
