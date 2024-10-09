#pragma once
#include <vector>

using namespace std;

enum NodeType {NT_SECTION, NT_DEFINITION, NT_WREPLACEMENT, NT_GROUP};

/// Класс предназначен для хранения состояния дерева CTreeCtrl в панели дерева.
/**
Список троек из узла дерева, флага выделения и флага сворачивания/разворачивания

TreeCtrlSnapshot :: [(Node, Selected, Expanded)]
type Selected = bool
type Expanded = bool
data Node = Sect { sectName :: String }
          | Def { sectName :: String,
				  defName :: String, }
		  | WRepl { srcString :: String, 
					dstString :: String }
		  | Group { sectName :: String, 
					groupName :: String }
*/
class CNCTreeCtrlSnapshot
{
public:
	class Node
	{
	public:
		struct Section
		{
			CString sectName;
		} Sect;
		struct Definition
		{
			CString sectName;
			CString defName;
		} Def;
		struct WReplacement
		{
			CString srcString;
			CString dstString;
		} WRepl;
		struct Group_
		{
			CString sectName;
			CString groupName;
		} Group;
		NodeType type;
	};
	class NodeStatus
	{
	public:
		Node node;
		bool Selected;
		bool Expanded;
	};
public:
	CNCTreeCtrlSnapshot(void);
	~CNCTreeCtrlSnapshot(void);

	bool GetSectSelected(CString sectName);
	bool GetSectExpanded(CString sectName);
	bool GetDefSelected(CString sectName, CString defName);
	bool GetWReplSelected(CString srcString, CString dstString);
	bool GetGroupSelected(CString sectName, CString groupName);
	bool GetGroupExpanded(CString sectName, CString groupName);

	vector <NodeStatus> snapshot;
	
};