#pragma once

class AFX_EXT_CLASS PropEditor
{
public:
	PropEditor(class PropPane *pPane);
	class PropList * pList;
	class PropPane * pPropPane;
	class PAbstractObject * pabstractObject;
	void Put(PAbstractObject * Obj);
	void Clear(void);
	~PropEditor();
	void Change(int id, const CString &value);

};