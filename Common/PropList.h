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

		int ID;//����� ����c���
		UINT Name;//�������� ��������
		int Type;// ��� ��������: 1 - combobox, 2 - ������, 3 - ����, 4 - ������������, 5 - �����, 6 - ����� 
		UINT Comment;//�����������
		double Time;//����� (��� 6)
		UINT Group;//������ �������
		int Color;// ����������� �������� (0 - ����� �� �������������, 1 - ������)
		double Max;//�������� (��� 4 � 5)
		double Min;//������� (��� 4 � 5)
		CString Value;//�������� �������� ��� ���� 2
		UINT ST_Value;//�������� �������� ��� ���� 1
		CArray<UINT,UINT> For_Enum;//������ �������� ��� ���� 1
		double V_Value;//�������� �������� ��� ���� 3,4,5

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