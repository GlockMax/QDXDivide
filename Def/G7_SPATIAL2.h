;FILE CNC CONTROL: NCM_HEIDENHAIN_MILL.sm3
;FILE NAME: G7_SPATIAL2.h
;CREAT: 27.04.2017 
;UPDATE: 04.04.2024 
;VERSION: 1.2
;PLANE SPATIAL SUB
O0007
;G07X120Y121Z122A=Q120B=Q121C=Q122
;G07A+0 B+0 C+0
L A=Q120 C=Q122
CYCL DEF 19.0 ;THE MACHINING PLANE
CYCL DEF 19.1 A=Q120 C=Q122
END PGM
O966
G7A=Q501B=Q502b=Q503X=117w1d2
;G1FQ509X=Q121Y=Q122Z=Q123
G1A=Q120B=Q121C=Q122
Q120=0 Q121=0 Q122=0
G67
END PGM
O980
L A+0B+0b+0 FMAX
G7
G67
END PGM
O208
END PGM
O9661
G5078A=Q501B=Q502b=Q507
G1A=Q501B=Q502b=Q507F500
G67
END PGM
;����� �� ����� �� ������ ��������
O99
IF (Q99=208) CALL PGM 208
IF (Q99=267) CALL PGM 267
IF (Q99=256) CALL PGM 256
IF (Q99=257) CALL PGM 257
END PGM
;CYCLE DEF 267 ;������
BEGIN PGM 267 MM
Q41=Q8215 Q42=Q8216; ����� �������� ������
Q43=Q201+Q239/2+Q203 ; ���������� Z ����� ������.
L Z=Q203 F MAX
;����� �����
Q40=Q335/2+2*Q108
L IX=-Q40 F=Q207
L IZ=-Q239/2
CC IX=Q108/2 IY=0
IX=+Q108 F=Q207 DR+
CC X=Q41  Y=Q42
LBL 11
CP IPA=360 IZ=-Q239 DR- 
IF (Q8217 GT Q43) GOTO LBL 11
CC IX=-Q108/2 IY=0
IX=-Q108 DR+
L Z=Q203+Q204 FMAX
END PGM
;CYCLE DEF 208
BEGIN PGM 208 MM 
CC X=Q8215  Y=Q8216
Q1=Q335/2-Q108
L Z=Q200 FMAX
L IX=Q1 F=Q206
LBL 1
CP IPA=360 IZ=-Q334 DR- 
IF (Q8217 GT Q201+Q334) GOTO LBL 1
L IX=-Q1
L Z=Q200 FMAX
LBL 0
END PGM
;CYCLE DEF 256
BEGIN PGM 256 MM 
;�������� ����� �� �����
END PGM
;CYCLE DEF 257
BEGIN PGM 257 MM
;�������� ����� �� ����� 
END PGM
