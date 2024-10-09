// NObject.cpp : implementation file
//

#include "stdafx.h"
#include "NCMProject.h"
#include "BObjectManager.h"
#include "NObject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// NObject
IMPLEMENT_SERIAL(NObject, CObject, 0)

BSelectAttrs NObject::SelectAttrs;

NObject::NObject()
{
	Group = OG_GR_EMPTY;
}

NObject::~NObject()
{
}
BObjectManager *NObject::GetObjectManager()
{
	return NCMProject::GetProject().GetObjectManager();
}

void NObject::SetSelectAttrs(BSelectAttrs SAttrs)
{
	SelectAttrs = SAttrs;
}

const BSelectAttrs& NObject::GetSelectAttrs() const
{
	return SelectAttrs;
}

const BObjectManager *NObject::GetObjectManager() const
{
	return NCMProject::GetProject().GetObjectManager();
}

void NObject::Serialize(CArchive &ar)
{
	if (!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("4.7")))
		SerializeElements(ar, &Group, 1);
}

