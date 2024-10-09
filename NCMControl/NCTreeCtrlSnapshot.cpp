#include "StdAfx.h"
#include "NCTreeCtrlSnapshot.h"

CNCTreeCtrlSnapshot::CNCTreeCtrlSnapshot(void)
{
}

CNCTreeCtrlSnapshot::~CNCTreeCtrlSnapshot(void)
{
}

bool CNCTreeCtrlSnapshot::GetSectSelected(CString sectName)
{
	for(unsigned int i = 0; i < snapshot.size(); i++)
	{
		if(snapshot[i].node.type == NT_SECTION && snapshot[i].node.Sect.sectName == sectName)
		{
			return snapshot[i].Selected;
			break;
		}//if
	}//for
	return false;
}//bool CNCTreeCtrlSnapshot::GetSectSelected(CString sectName)

bool CNCTreeCtrlSnapshot::GetSectExpanded(CString sectName)
{
	for(unsigned int i = 0; i < snapshot.size(); i++)
	{
		if(snapshot[i].node.type == NT_SECTION && snapshot[i].node.Sect.sectName == sectName)
		{
			return snapshot[i].Expanded;
			break;
		}//if
	}//for
	return false;
}//bool CNCTreeCtrlSnapshot::GetSectExpanded(CString sectName)

bool CNCTreeCtrlSnapshot::GetDefSelected(CString sectName, CString defName)
{
	for(unsigned int i = 0; i < snapshot.size(); i++)
	{
		if(snapshot[i].node.type == NT_DEFINITION && snapshot[i].node.Def.sectName == sectName &&
			snapshot[i].node.Def.defName == defName)
		{
			return snapshot[i].Selected;
			break;
		}//if
	}//for
	return false;
}//bool CNCTreeCtrlSnapshot::GetDefSelected(CString sectName, CString defName)

bool CNCTreeCtrlSnapshot::GetWReplSelected(CString srcString, CString dstString)
{
	for(unsigned int i = 0; i < snapshot.size(); i++)
	{
		if(snapshot[i].node.type == NT_WREPLACEMENT && snapshot[i].node.WRepl.srcString == srcString &&
			snapshot[i].node.WRepl.dstString == dstString)
		{
			return snapshot[i].Selected;
			break;
		}//if
	}//for
	return false;
}//bool CNCTreeCtrlSnapshot::GetWReplSelected(CString srcString, CString dstString)

bool CNCTreeCtrlSnapshot::GetGroupSelected(CString sectName, CString groupName)
{
	for(unsigned int i = 0; i < snapshot.size(); i++)
	{
		if(snapshot[i].node.type == NT_GROUP && snapshot[i].node.Group.sectName == sectName &&
			snapshot[i].node.Group.groupName == groupName)
		{
			return snapshot[i].Selected;
			break;
		}//if
	}//for
	return false;
}//bool CNCTreeCtrlSnapshot::GetGroupSelected(CString sectName, CString groupName)

bool CNCTreeCtrlSnapshot::GetGroupExpanded(CString sectName, CString groupName)
{
	for(unsigned int i = 0; i < snapshot.size(); i++)
	{
		if(snapshot[i].node.type == NT_GROUP && snapshot[i].node.Group.sectName == sectName &&
			snapshot[i].node.Group.groupName == groupName)
		{
			return snapshot[i].Expanded;
			break;
		}//if
	}//for
	return false;
}//bool CNCTreeCtrlSnapshot::GetGroupExpanded(CString sectName, CString groupName)