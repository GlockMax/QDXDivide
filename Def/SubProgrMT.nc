;��� ������ NCM_FANUC_C_MILLTURN
;AddCoord00	w	_	_	TUR_ROT
;AddCoord01	x	_	_	spin
;AddCoord02	d	_	_	MoveZ
;AddCoord03	A	_	_	RotX
;
:30
G0G90G53X#24Y#25Z#26
G0C#3
M99
O30
U#21W#23
G0C0
X300.Z500.
;N3 (T#20)
;(#20=0)
M99
:6
;����� �����������
;#24 - X ����� �����������
;#26 - Z ����� �����������
;#20 - ����� �����
#124=#8324 ; �������� ������� =1 ��� =0
#126=#8326 ; �������� ������� =1 ��� =0
#120=#8320 ; �������� ������� =1 ��� =0
IF #8324 G0 X#24
IF #8326 G0 Z#26
IF [#8320 EQ 0] M(��� ������ �����������)
T0M6
G0w=-[30*#20-30]
T#20
M99

