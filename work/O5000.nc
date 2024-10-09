%
O1006
(DETALAZPI2041112124,VILETZAGOTOVKI64)
(VSUBSPSIRIEKUKACHKIH=70)
G21G80G40
G50S3000
G54G99G97
/G52Z40.
G0G28U0.W0.
/T101(UPOR)
/G0X0Z20.
/Z1.
/M98P8888
/(ENDBORFIDER)
/G0Z20.
/G0G28U0.W0.
/G4X3.

M1
T0202
G0X28.Z5.G96S180M03
M8
G94X-1.Z0F.1
G90X24.2Z-40.F.2
G0G28U0.W0.M09
M01
(CHISTOVOETOCHENIE)
(SDJCR2525M11)
(INSERT-DCMT11T304-PF)
T1010
G0X28.Z5.G96S180M03
G0X23.3Z1.M8
G1G42Z0F.05
X23.7Z-.2
Z-12.5F.1
X23.87W-.1F.05
Z-23.5F.1
G0G40X30.M09
G28U0.W0.
M01
(CENTRALSVERLENIE)
(DRILLD14-880D1400L20-05)
T0808
G0X0Z25.G97S2500M03
Z5.M8
G1Z-3.F.03
G1Z-30.F.06
G0Z25.
G0G80M9
G28U0.W0.
M01
(CENTRALSVERLENIE)
(DRILLD9.7-R840-0970-70-A1A)
G0T0909
G0X0Z25.G97S3000M03
G0Z-29.M08
G99G1Z-32.F.03
G74R.03
G74Z-41.Q1000F.05
G0Z25.M9
G28U0.W0.
M01
(RASTOCHKACHERNOV)
(A10K-SCLPR06)
T0505
G0X14.Z10.G96S180M03
Z2.M08
G71U1.R1.
G71P250Q260U-.2W.2F.1
N250G0X21.588
G1Z0F.1
X18.288,A45.
Z-19.
N260X14.,A60.
G28U0.W0.M9
M01
(RASTOCHKA)
(A08H-SCLCL06)
(EF132L-4010105-B)
T0404
G0X21.588Z10.G96S150M03
Z2.M8
G99G1G41Z0F.05
X18.288,A45.
Z-19.
X14.48,A60.
Z-30.
X10.49
X9.5,A30.
G40U-.1W.1
G0Z10.M9
G28U0.W0.
M01
(REZBAM19)
(R166.0KF-16-1625-11B)
T0303
G0X16.283Z10.111G97S1500M03
M8
G76P010060Q70
G76X19.18Z-14.8P358Q143F.75
M9
G28U0.W0.
M01
(OTREZKA)
(LF151.22-2525-30)
(INSERT-N151.2-300-5E)
T1212
G0Z-36.3
G97X25.S1500M3
G1X24.F.02M210
M8
G75R.03
G75X13.P200F.05
G0X24.5M220
/G98G1B-476.2F500
/M211

B550.
G4X2.M221
G99G1X13.2F.2
G1X9.F.05
G0X200.
G0Z100.
Z0.
G52Z-550.
A-550.B0E-550.Z0.
M1
(SUBSHPINDEL)
(CHERNOVOETOCHENIE)
(DCLNR2525M12)
(INSERT-CNMG120404-PM4225)
G52Z4.
G50S5000
/G0G28U0.W0.
G0T2022
G0X25.Z-5.G96S180M203
M8
G99G1Z0.F.2
X6.F.1
Z-2.F.2
G0X23.84
G1Z13.395F.2
G2X24.274Z14.017R1.
G1X27.102Z12.603
G0Z-4.
X19.64
G1Z-2.F.2
Z3.106
X23.254Z4.913
G2X23.84Z5.62R1.
G1Z6.8
Z13.395
G2X24.04Z13.547R1.
G1X26.868Z12.133
G0Z-4.
X16.2
G1Z-2.F.2
Z1.386
X19.84Z3.206
X22.668Z1.792
G0Z-4.
X13.8
G1Z-2.2F.2
Z-.2
G2X16.2Z1.R1.2
G1Z1.386
X16.4Z1.486
X19.228Z.072
Z-2.
X13.8
G99G1Z0.F.1
G2X15.8Z.6R.6
G1Z1.234
X23.206Z4.937
G2X23.44Z5.22R.4
G1Z6.4
Z13.234
X23.64Z13.334
G2X23.874Z13.617R.4
G1Z13.9
X26.463Z13.
G0Z-25.
M9
G28U0.W0.M205
M01
(FREZEROVKA6-GRANNIK)
(R216.24-16050DCK36P)
(393.14-25160)
G0G28U0.W0.
G52Z4.
G0T0606
G0X65.Z-5.G97S1500M74
M8
G98G1X50.Z11.2F1000.
(A-6GRANNIKB-FREZA)
G65O5001A21.9B16.
G0Z-50.X150.
G28U0.W0.
M01
(FASKA0.3X45)
(R12316)
M30
%

%
:5001
(FREZEROVKA 6-GRANNIK) 
#9=#1+#2
#8=#9*2./1.732 
M219 
G1G28H0F15000
C0 
G12.1
G1X#8F300. 
X[#8/2.]C[-#9/2.]
X[-#8/2.] 
X[-#8]C0.
X[-#8/2.]C[#9/2.]
X[#8/2.]
X#8C0. 
G1X[#9+5.]M9 
G13.1
M75
M218 
M99
%













