#pragma once


class AFX_EXT_CLASS PropSingle
{
public:
	PropSingle()
	{
		clear();
	};
	void clear()
	{
		ID=0;
		Name=NULL;
		Type=0;
		Comment=NULL;
		Time=0;
		Group=NULL;
		Color=1;
		Max=0;
		Value="";
		ST_Value=NULL;
		Min=0;
		V_Value=0;
		For_Enum.RemoveAll();
	};

		int ID;//номер свойcтва
		UINT Name;//название свойства
		int Type;// тип свойства: 1 - combobox, 2 - строка, 3 - цвет, 4 - вещественные, 5 - целые, 6 - время 
		UINT Comment;//комментарий
		double Time;//время (тип 6)
		UINT Group;//группа свойств
		int Color;// отображение свойства (0 - серое не редактируемое, 1 - черное)
		double Max;//максимум (тип 4 и 5)
		double Min;//минимум (тип 4 и 5)
		CString Value;//значение свойства для типа 2
		UINT ST_Value;//значение свойства для типа 1
		CArray<UINT,UINT> For_Enum;//список значений для типа 1
		double V_Value;//значение свойства для типа 3,4,5

		~PropSingle(){
			For_Enum.RemoveAll();
		};
};
class AFX_EXT_CLASS PropList : public CArray<PropSingle *,PropSingle *>
{
public:
	PropList();
	PropSingle * Copy(PropSingle * Single);
	void Clear();
	~PropList();
	bool operator == (const PropList &InList);
};