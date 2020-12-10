EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L DomIO32-rescue:ESP-02-DomIO32 U3
U 1 1 5DCB6F71
P 7050 1800
F 0 "U3" H 8600 1167 50  0000 C CNN
F 1 "ESP-02" H 8600 1076 50  0000 C CNN
F 2 "RF_Module:ESP-WROOM-02" H 8250 -150 50  0001 C CIN
F 3 "" H 8600 150 50  0001 C CNN
	1    7050 1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	2800 5750 2800 3750
Wire Wire Line
	2800 3750 3800 3750
Wire Wire Line
	2900 5750 2900 4950
Wire Wire Line
	2900 4950 3800 4950
Wire Wire Line
	3000 4450 3800 4450
Wire Wire Line
	3000 4450 3000 5750
Wire Wire Line
	2900 4950 2900 3200
Wire Wire Line
	2900 3200 5500 3200
Wire Wire Line
	5500 3200 5500 3750
Wire Wire Line
	5500 3750 5300 3750
Connection ~ 2900 4950
Connection ~ 3000 4450
$Comp
L power:GND #PWR012
U 1 1 5DCB75BF
P 5650 7300
F 0 "#PWR012" H 5650 7050 50  0001 C CNN
F 1 "GND" H 5655 7127 50  0001 C CNN
F 2 "" H 5650 7300 50  0001 C CNN
F 3 "" H 5650 7300 50  0001 C CNN
	1    5650 7300
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C10
U 1 1 5DCB76F2
P 5600 4550
F 0 "C10" V 5650 4350 50  0000 L CNN
F 1 "100uf / 16v" V 5450 4250 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D6.3mm_P2.50mm" H 5638 4400 50  0001 C CNN
F 3 "~" H 5600 4550 50  0001 C CNN
	1    5600 4550
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3100 5750 3100 4050
Wire Wire Line
	3100 4050 3800 4050
Wire Wire Line
	3800 4150 3200 4150
Wire Wire Line
	3200 4150 3200 5750
$Comp
L Device:C C1
U 1 1 5DCB7E02
P 1600 4150
F 0 "C1" H 1715 4196 50  0000 L CNN
F 1 "100nf / 50v" H 1715 4105 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 1638 4000 50  0001 C CNN
F 3 "~" H 1600 4150 50  0001 C CNN
	1    1600 4150
	1    0    0    -1  
$EndComp
Wire Wire Line
	1600 4450 1600 4300
$Comp
L Device:R R4
U 1 1 5DCB8187
P 1850 3750
F 0 "R4" V 1850 3750 50  0000 C CNN
F 1 "1K" V 1734 3750 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 1780 3750 50  0001 C CNN
F 3 "~" H 1850 3750 50  0001 C CNN
	1    1850 3750
	0    1    1    0   
$EndComp
Wire Wire Line
	1600 4000 1600 3750
Wire Wire Line
	1600 3750 1700 3750
$Comp
L Device:R R1
U 1 1 5DCB9129
P 1600 3500
F 0 "R1" V 1600 3450 50  0000 L CNN
F 1 "10K" H 1350 3500 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 1530 3500 50  0001 C CNN
F 3 "~" H 1600 3500 50  0001 C CNN
	1    1600 3500
	1    0    0    -1  
$EndComp
Wire Wire Line
	1600 3200 1600 3350
Wire Wire Line
	1600 3750 1600 3650
Connection ~ 1600 3750
$Comp
L Switch:SW_Push SW1
U 1 1 5DCBABE2
P 1400 4150
F 0 "SW1" V 1550 4350 50  0000 R CNN
F 1 "TactSwitch" V 1250 4550 50  0000 R CNN
F 2 "Button_Switch_THT:SW_PUSH_6mm_H5mm" H 1400 4350 50  0001 C CNN
F 3 "" H 1400 4350 50  0001 C CNN
	1    1400 4150
	0    -1   -1   0   
$EndComp
Wire Wire Line
	1600 3750 1400 3750
Wire Wire Line
	1400 3750 1400 3950
Wire Wire Line
	1600 4450 1400 4450
Wire Wire Line
	1400 4450 1400 4350
Connection ~ 1600 4450
$Comp
L Device:R R16
U 1 1 5DCCB3AF
P 8450 3950
F 0 "R16" V 8450 3950 50  0000 C CNN
F 1 "1K" V 8400 4150 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 8380 3950 50  0001 C CNN
F 3 "~" H 8450 3950 50  0001 C CNN
	1    8450 3950
	0    1    1    0   
$EndComp
$Comp
L Device:R R17
U 1 1 5DCCB482
P 8450 4050
F 0 "R17" V 8450 4050 50  0000 C CNN
F 1 "1K" V 8400 4250 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 8380 4050 50  0001 C CNN
F 3 "~" H 8450 4050 50  0001 C CNN
	1    8450 4050
	0    1    1    0   
$EndComp
$Comp
L Device:D D5
U 1 1 5DCCEAC1
P 5500 3000
F 0 "D5" V 5546 2921 50  0000 R CNN
F 1 "LL4148" V 5455 2921 50  0000 R CNN
F 2 "Diode_SMD:D_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 5500 3000 50  0001 C CNN
F 3 "~" H 5500 3000 50  0001 C CNN
	1    5500 3000
	0    -1   -1   0   
$EndComp
Wire Wire Line
	9650 4150 9650 3050
Wire Wire Line
	9650 3050 9400 3050
$Comp
L power:GND #PWR018
U 1 1 5DCD7E79
P 9750 4300
F 0 "#PWR018" H 9750 4050 50  0001 C CNN
F 1 "GND" H 9755 4127 50  0001 C CNN
F 2 "" H 9750 4300 50  0001 C CNN
F 3 "" H 9750 4300 50  0001 C CNN
	1    9750 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	9400 2750 9750 2750
Connection ~ 9750 3800
Wire Wire Line
	9750 3800 9750 4300
Wire Wire Line
	7800 2750 7600 2750
$Comp
L Device:C C11
U 1 1 5DCDBCE7
P 5800 3400
F 0 "C11" H 5915 3446 50  0000 L CNN
F 1 "100nf / 50v" H 5915 3355 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 5838 3250 50  0001 C CNN
F 3 "~" H 5800 3400 50  0001 C CNN
	1    5800 3400
	1    0    0    -1  
$EndComp
$Comp
L Device:C C13
U 1 1 5DCDBD3D
P 7600 3050
F 0 "C13" H 7250 3150 50  0000 L CNN
F 1 "100nf / 50v" H 7000 3050 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 7638 2900 50  0001 C CNN
F 3 "~" H 7600 3050 50  0001 C CNN
	1    7600 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	7600 3800 7600 3550
Wire Wire Line
	7600 3800 9750 3800
Wire Wire Line
	7600 3550 7800 3550
Connection ~ 7600 3550
Connection ~ 7600 2750
Connection ~ 5500 3200
$Comp
L Device:C C4
U 1 1 5DCE37BC
P 4000 1700
F 0 "C4" H 4050 1550 50  0000 L CNN
F 1 "100nf / 50v" H 3950 1200 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 4038 1550 50  0001 C CNN
F 3 "~" H 4000 1700 50  0001 C CNN
	1    4000 1700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR09
U 1 1 5DCE38D1
P 4000 2050
F 0 "#PWR09" H 4000 1800 50  0001 C CNN
F 1 "GND" H 4005 1877 50  0001 C CNN
F 2 "" H 4000 2050 50  0001 C CNN
F 3 "" H 4000 2050 50  0001 C CNN
	1    4000 2050
	1    0    0    -1  
$EndComp
Wire Wire Line
	7800 2850 7600 2850
Wire Wire Line
	7600 2850 7600 2750
Wire Wire Line
	8300 3950 5300 3950
Wire Wire Line
	8300 4050 5300 4050
Wire Wire Line
	8600 3950 9550 3950
Wire Wire Line
	9550 3950 9550 3350
Wire Wire Line
	9550 3350 9400 3350
Wire Wire Line
	9600 4050 9600 3450
Wire Wire Line
	9600 3450 9400 3450
Wire Wire Line
	8600 4050 9600 4050
$Comp
L DomIO32-rescue:PIC32MM0064GPL028-I-SO-DomIO32 U2
U 1 1 5DD039C1
P 3950 3450
F 0 "U2" H 4550 3415 50  0000 C CNN
F 1 "PIC32MM0064GPL028-I-SS" H 4550 3324 50  0000 C CNN
F 2 "Package_SO:SSOP-28_5.3x10.2mm_P0.65mm" H 3950 3300 50  0001 C CNN
F 3 "" H 3950 3300 50  0001 C CNN
	1    3950 3450
	1    0    0    -1  
$EndComp
$Comp
L Device:R R5
U 1 1 5DD03A2E
P 4150 6900
F 0 "R5" V 4150 6850 50  0000 L CNN
F 1 "1K" H 4220 6855 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 4080 6900 50  0001 C CNN
F 3 "~" H 4150 6900 50  0001 C CNN
	1    4150 6900
	1    0    0    -1  
$EndComp
$Comp
L Device:R R6
U 1 1 5DD03B19
P 4450 6900
F 0 "R6" V 4450 6850 50  0000 L CNN
F 1 "1K" H 4520 6855 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 4380 6900 50  0001 C CNN
F 3 "~" H 4450 6900 50  0001 C CNN
	1    4450 6900
	1    0    0    -1  
$EndComp
$Comp
L Device:R R7
U 1 1 5DD03BBD
P 4750 6900
F 0 "R7" V 4750 6850 50  0000 L CNN
F 1 "1K" H 4820 6855 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 4680 6900 50  0001 C CNN
F 3 "~" H 4750 6900 50  0001 C CNN
	1    4750 6900
	1    0    0    -1  
$EndComp
$Comp
L Device:R R8
U 1 1 5DD03C27
P 5050 6900
F 0 "R8" V 5050 6800 50  0000 L CNN
F 1 "1K" H 5120 6855 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 4980 6900 50  0001 C CNN
F 3 "~" H 5050 6900 50  0001 C CNN
	1    5050 6900
	1    0    0    -1  
$EndComp
$Comp
L Device:C C5
U 1 1 5DD03CA1
P 4150 6300
F 0 "C5" H 4150 6400 50  0000 L CNN
F 1 "100nf / 50v" H 3900 6600 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 4188 6150 50  0001 C CNN
F 3 "~" H 4150 6300 50  0001 C CNN
	1    4150 6300
	1    0    0    -1  
$EndComp
$Comp
L Device:C C6
U 1 1 5DD03D7A
P 4450 6300
F 0 "C6" H 4450 6400 50  0000 L CNN
F 1 "100nf / 50v" H 4250 6700 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 4488 6150 50  0001 C CNN
F 3 "~" H 4450 6300 50  0001 C CNN
	1    4450 6300
	1    0    0    -1  
$EndComp
$Comp
L Device:C C7
U 1 1 5DD03DE8
P 4750 6300
F 0 "C7" H 4750 6400 50  0000 L CNN
F 1 "100nf / 50v" H 4550 6600 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 4788 6150 50  0001 C CNN
F 3 "~" H 4750 6300 50  0001 C CNN
	1    4750 6300
	1    0    0    -1  
$EndComp
$Comp
L Device:C C8
U 1 1 5DD03E52
P 5050 6300
F 0 "C8" H 5050 6400 50  0000 L CNN
F 1 "100nf / 50v" H 4850 6700 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 5088 6150 50  0001 C CNN
F 3 "~" H 5050 6300 50  0001 C CNN
	1    5050 6300
	1    0    0    -1  
$EndComp
Wire Wire Line
	4150 6150 4150 6100
Wire Wire Line
	4150 6100 4450 6100
Wire Wire Line
	4450 6150 4450 6100
Connection ~ 4450 6100
Wire Wire Line
	4450 6100 4750 6100
Wire Wire Line
	4750 6150 4750 6100
Connection ~ 4750 6100
Wire Wire Line
	4750 6100 5050 6100
Wire Wire Line
	5050 6150 5050 6100
Connection ~ 5050 6100
Wire Wire Line
	4750 7250 4750 7200
Wire Wire Line
	4750 7200 4150 7200
Wire Wire Line
	4150 7200 4150 7050
Wire Wire Line
	4850 7250 4850 7150
Wire Wire Line
	4850 7150 4450 7150
Wire Wire Line
	4450 7150 4450 7050
Wire Wire Line
	4950 7250 4950 7100
Wire Wire Line
	4950 7100 4750 7100
Wire Wire Line
	4750 7100 4750 7050
Wire Wire Line
	5050 7250 5050 7050
Wire Wire Line
	5150 7250 5150 7100
$Comp
L Device:CP C3
U 1 1 5DD06DD2
P 3600 1700
F 0 "C3" H 3650 1550 50  0000 L CNN
F 1 "100uf / 16v" H 3300 1200 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D6.3mm_P2.50mm" H 3638 1550 50  0001 C CNN
F 3 "~" H 3600 1700 50  0001 C CNN
	1    3600 1700
	1    0    0    -1  
$EndComp
$Comp
L Device:C C2
U 1 1 5DD06FAC
P 2150 1700
F 0 "C2" H 2200 1550 50  0000 L CNN
F 1 "100nf / 50v" H 1950 1200 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 2188 1550 50  0001 C CNN
F 3 "~" H 2150 1700 50  0001 C CNN
	1    2150 1700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR02
U 1 1 5DD07258
P 2500 2050
F 0 "#PWR02" H 2500 1800 50  0001 C CNN
F 1 "GND" H 2505 1877 50  0001 C CNN
F 2 "" H 2500 2050 50  0001 C CNN
F 3 "" H 2500 2050 50  0001 C CNN
	1    2500 2050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR06
U 1 1 5DD33E6A
P 2950 2050
F 0 "#PWR06" H 2950 1800 50  0001 C CNN
F 1 "GND" H 2955 1877 50  0001 C CNN
F 2 "" H 2950 2050 50  0001 C CNN
F 3 "" H 2950 2050 50  0001 C CNN
	1    2950 2050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR07
U 1 1 5DD33FF8
P 3600 2050
F 0 "#PWR07" H 3600 1800 50  0001 C CNN
F 1 "GND" H 3605 1877 50  0001 C CNN
F 2 "" H 3600 2050 50  0001 C CNN
F 3 "" H 3600 2050 50  0001 C CNN
	1    3600 2050
	1    0    0    -1  
$EndComp
$Comp
L Device:D D3
U 1 1 5DD71293
P 1950 1300
F 0 "D3" H 1950 1400 50  0000 C CNN
F 1 "M7" H 1950 1175 50  0000 C CNN
F 2 "Diode_SMD:D_SMA" H 1950 1300 50  0001 C CNN
F 3 "~" H 1950 1300 50  0001 C CNN
	1    1950 1300
	-1   0    0    1   
$EndComp
Wire Wire Line
	1750 1300 1800 1300
Wire Wire Line
	2100 1300 2150 1300
Connection ~ 2150 1300
Connection ~ 3600 1300
Wire Wire Line
	3600 1300 4000 1300
Wire Wire Line
	7150 2750 5500 2750
Wire Wire Line
	5500 2750 5500 2850
Wire Wire Line
	5500 3150 5500 3200
Wire Wire Line
	7450 2750 7600 2750
$Comp
L Connector_Generic:Conn_02x05_Odd_Even J5
U 1 1 5DE6D22A
P 7600 4950
F 0 "J5" V 7500 4650 50  0000 R CNN
F 1 "Conector2X5" V 7600 4650 50  0000 R CNN
F 2 "Connector_Molex:Molex_Mini-Fit_Jr_5566-10A_2x05_P4.20mm_Vertical" H 7600 4950 50  0001 C CNN
F 3 "~" H 7600 4950 50  0001 C CNN
	1    7600 4950
	0    -1   1    0   
$EndComp
$Comp
L Connector_Generic:Conn_01x05 J2
U 1 1 5DE6D3C2
P 3000 5950
F 0 "J2" V 2873 5662 50  0000 R CNN
F 1 "ICSP" V 3100 6000 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x05_P2.54mm_Vertical" H 3000 5950 50  0001 C CNN
F 3 "~" H 3000 5950 50  0001 C CNN
	1    3000 5950
	0    -1   1    0   
$EndComp
$Comp
L Device:R R9
U 1 1 5DE922F2
P 6500 4250
F 0 "R9" V 6500 4250 50  0000 C CNN
F 1 "1K" V 6550 4050 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 6430 4250 50  0001 C CNN
F 3 "~" H 6500 4250 50  0001 C CNN
	1    6500 4250
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R10
U 1 1 5DE9237C
P 6500 4350
F 0 "R10" V 6500 4350 50  0000 C CNN
F 1 "1K" V 6550 4150 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 6430 4350 50  0001 C CNN
F 3 "~" H 6500 4350 50  0001 C CNN
	1    6500 4350
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R11
U 1 1 5DE92402
P 6500 4450
F 0 "R11" V 6500 4450 50  0000 C CNN
F 1 "1K" V 6550 4250 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 6430 4450 50  0001 C CNN
F 3 "~" H 6500 4450 50  0001 C CNN
	1    6500 4450
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R12
U 1 1 5DE92480
P 6500 4550
F 0 "R12" V 6500 4550 50  0000 C CNN
F 1 "1K" V 6550 4350 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 6430 4550 50  0001 C CNN
F 3 "~" H 6500 4550 50  0001 C CNN
	1    6500 4550
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6650 4550 7500 4550
Wire Wire Line
	7500 4550 7500 4750
Wire Wire Line
	6650 4450 7600 4450
Wire Wire Line
	7600 4450 7600 4750
Wire Wire Line
	6650 4350 7700 4350
Wire Wire Line
	7700 4350 7700 4750
Wire Wire Line
	6650 4250 7800 4250
Wire Wire Line
	7800 4250 7800 4750
Wire Wire Line
	5650 6100 5650 7100
$Comp
L Device:R R13
U 1 1 5DF5D0E2
P 6500 4650
F 0 "R13" V 6500 4650 50  0000 C CNN
F 1 "1K" V 6550 4450 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 6430 4650 50  0001 C CNN
F 3 "~" H 6500 4650 50  0001 C CNN
	1    6500 4650
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR016
U 1 1 5DF86986
P 7600 6200
F 0 "#PWR016" H 7600 5950 50  0001 C CNN
F 1 "GND" H 7605 6027 50  0001 C CNN
F 2 "" H 7600 6200 50  0001 C CNN
F 3 "" H 7600 6200 50  0001 C CNN
	1    7600 6200
	1    0    0    -1  
$EndComp
$Comp
L power:+4V #PWR08
U 1 1 5DF9C9D5
P 4000 900
F 0 "#PWR08" H 4000 750 50  0001 C CNN
F 1 "+4V" H 4015 1073 50  0000 C CNN
F 2 "" H 4000 900 50  0001 C CNN
F 3 "" H 4000 900 50  0001 C CNN
	1    4000 900 
	1    0    0    -1  
$EndComp
$Comp
L power:+4V #PWR015
U 1 1 5DF9CD51
P 7000 5700
F 0 "#PWR015" H 7000 5550 50  0001 C CNN
F 1 "+4V" H 7015 5873 50  0000 C CNN
F 2 "" H 7000 5700 50  0001 C CNN
F 3 "" H 7000 5700 50  0001 C CNN
	1    7000 5700
	1    0    0    -1  
$EndComp
$Comp
L power:+12V #PWR03
U 1 1 5DF9D05F
P 1750 1100
F 0 "#PWR03" H 1750 950 50  0001 C CNN
F 1 "+12V" H 1765 1273 50  0000 C CNN
F 2 "" H 1750 1100 50  0001 C CNN
F 3 "" H 1750 1100 50  0001 C CNN
	1    1750 1100
	1    0    0    -1  
$EndComp
$Comp
L power:+12V #PWR017
U 1 1 5DF9D3DB
P 8000 5250
F 0 "#PWR017" H 8000 5100 50  0001 C CNN
F 1 "+12V" H 8015 5423 50  0000 C CNN
F 2 "" H 8000 5250 50  0001 C CNN
F 3 "" H 8000 5250 50  0001 C CNN
	1    8000 5250
	1    0    0    -1  
$EndComp
Wire Wire Line
	4000 1300 4000 900 
Connection ~ 4000 1300
Wire Wire Line
	1750 1300 1750 1100
$Comp
L Device:LED D1
U 1 1 5DFBC605
P 1450 4700
F 0 "D1" H 1300 4750 50  0000 C CNN
F 1 "Led Rojo" H 950 4650 50  0000 C CNN
F 2 "Diode_SMD:D_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 1450 4700 50  0001 C CNN
F 3 "~" H 1450 4700 50  0001 C CNN
	1    1450 4700
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D2
U 1 1 5DFBC761
P 1450 5000
F 0 "D2" H 1300 5050 50  0000 C CNN
F 1 "Led Verde" H 950 4950 50  0000 C CNN
F 2 "Diode_SMD:D_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 1450 5000 50  0001 C CNN
F 3 "~" H 1450 5000 50  0001 C CNN
	1    1450 5000
	1    0    0    -1  
$EndComp
$Comp
L Device:R R2
U 1 1 5DFBC7EE
P 1800 4700
F 0 "R2" V 1800 4700 50  0000 C CNN
F 1 "1K" V 1916 4700 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 1730 4700 50  0001 C CNN
F 3 "~" H 1800 4700 50  0001 C CNN
	1    1800 4700
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R3
U 1 1 5DFBC896
P 1800 5000
F 0 "R3" V 1800 5000 50  0000 C CNN
F 1 "1K" V 1916 5000 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 1730 5000 50  0001 C CNN
F 3 "~" H 1800 5000 50  0001 C CNN
	1    1800 5000
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR01
U 1 1 5DFDC0B9
P 1200 5150
F 0 "#PWR01" H 1200 4900 50  0001 C CNN
F 1 "GND" H 1205 4977 50  0001 C CNN
F 2 "" H 1200 5150 50  0001 C CNN
F 3 "" H 1200 5150 50  0001 C CNN
	1    1200 5150
	1    0    0    -1  
$EndComp
Wire Wire Line
	1650 4700 1600 4700
Wire Wire Line
	1650 5000 1600 5000
Wire Wire Line
	1300 4700 1200 4700
Wire Wire Line
	1200 4700 1200 5000
Wire Wire Line
	1300 5000 1200 5000
Connection ~ 1200 5000
Wire Wire Line
	1200 5000 1200 5150
Text Label 8600 5000 0    50   ~ 0
Expansion
Text Label 8600 5100 0    50   ~ 0
1)_Exp01
Text Label 8600 5200 0    50   ~ 0
2)_+3v3
Text Label 8600 5300 0    50   ~ 0
3)_Exp03
Text Label 8600 5400 0    50   ~ 0
4)_Exp04_(d0)
Text Label 8600 5500 0    50   ~ 0
5)_Exp05
Text Label 8600 5600 0    50   ~ 0
6)_Gnd
Text Label 8600 5700 0    50   ~ 0
7)_Exp07
Text Label 8600 5800 0    50   ~ 0
8)_Exp08_(d1)
Text Label 8600 5900 0    50   ~ 0
9)_Exp09
Text Label 8550 6000 0    50   ~ 0
10)_+12v
Wire Notes Line
	8550 4950 8550 6050
Wire Notes Line
	8600 6100 9100 6100
Wire Notes Line
	9150 6050 9150 4950
Wire Notes Line
	9100 4900 8600 4900
Text Label 850  4150 0    50   ~ 0
RESET
Text Label 850  4700 0    50   ~ 0
STATUS
Text Label 850  5000 0    50   ~ 0
MODO
$Comp
L Connector:Screw_Terminal_01x03 J3
U 1 1 5E0FF274
P 4850 7450
F 0 "J3" V 4850 7150 50  0000 L CNN
F 1 "OSTTC032162" V 4950 6850 50  0000 L CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_PT-1,5-3-5.0-H_1x03_P5.00mm_Horizontal" H 4850 7450 50  0001 C CNN
F 3 "~" H 4850 7450 50  0001 C CNN
	1    4850 7450
	0    1    1    0   
$EndComp
$Comp
L Connector:Screw_Terminal_01x03 J4
U 1 1 5E0FF3DF
P 5150 7450
F 0 "J4" V 5150 7650 50  0000 L CNN
F 1 "OSTTC032162" V 5250 7650 50  0000 L CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_PT-1,5-3-5.0-H_1x03_P5.00mm_Horizontal" H 5150 7450 50  0001 C CNN
F 3 "~" H 5150 7450 50  0001 C CNN
	1    5150 7450
	0    1    1    0   
$EndComp
$Comp
L power:+12V #PWR010
U 1 1 5E10927A
P 5250 6900
F 0 "#PWR010" H 5250 6750 50  0001 C CNN
F 1 "+12V" H 5265 7073 50  0000 C CNN
F 2 "" H 5250 6900 50  0001 C CNN
F 3 "" H 5250 6900 50  0001 C CNN
	1    5250 6900
	1    0    0    -1  
$EndComp
Text Label 7450 5050 0    50   ~ 0
Expansion
$Comp
L power:+4V #PWR011
U 1 1 5E12FBB0
P 5500 2650
F 0 "#PWR011" H 5500 2500 50  0001 C CNN
F 1 "+4V" H 5515 2823 50  0000 C CNN
F 2 "" H 5500 2650 50  0001 C CNN
F 3 "" H 5500 2650 50  0001 C CNN
	1    5500 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	5500 2750 5500 2650
Connection ~ 5500 2750
Text Label 7200 6800 0    118  ~ 24
Domotica_IoT
Text Label 7200 7050 0    118  ~ 24
Modulo_de_expansion_I&O
Wire Notes Line
	4550 2400 4550 700 
Text Label 4850 7700 0    50   ~ 0
Entradas
Text Label 5250 7600 0    50   ~ 0
+
Text Label 5100 7600 0    50   ~ 0
-
Wire Wire Line
	4150 6450 4150 6650
Wire Wire Line
	4450 6450 4450 6550
Wire Wire Line
	4750 6450 4750 6550
Wire Wire Line
	5050 6450 5050 6650
Wire Wire Line
	1600 3200 2900 3200
Connection ~ 2900 3200
Wire Wire Line
	2000 3750 2800 3750
Connection ~ 2800 3750
Wire Wire Line
	1600 4450 2450 4450
Wire Wire Line
	1950 4700 2000 4700
Wire Wire Line
	2000 4700 2000 3850
Wire Wire Line
	2000 3850 3800 3850
Wire Wire Line
	1950 5000 2050 5000
Wire Wire Line
	2050 5000 2050 3950
Wire Wire Line
	2050 3950 3800 3950
$Comp
L power:GND #PWR05
U 1 1 5DDC056B
P 2450 4600
F 0 "#PWR05" H 2450 4350 50  0001 C CNN
F 1 "GND" H 2455 4427 50  0001 C CNN
F 2 "" H 2450 4600 50  0001 C CNN
F 3 "" H 2450 4600 50  0001 C CNN
	1    2450 4600
	1    0    0    -1  
$EndComp
Wire Wire Line
	2450 4600 2450 4450
Connection ~ 2450 4450
Wire Wire Line
	2450 4450 3000 4450
Wire Wire Line
	5300 3850 5800 3850
$Comp
L power:GND #PWR014
U 1 1 5DDE0DA2
P 6050 3700
F 0 "#PWR014" H 6050 3450 50  0001 C CNN
F 1 "GND" H 6055 3527 50  0001 C CNN
F 2 "" H 6050 3700 50  0001 C CNN
F 3 "" H 6050 3700 50  0001 C CNN
	1    6050 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	5500 3200 5800 3200
Wire Wire Line
	5800 3200 5800 3250
Wire Wire Line
	5800 3550 5800 3650
Wire Wire Line
	6050 3700 6050 3650
Wire Wire Line
	6050 3650 5800 3650
Connection ~ 5800 3650
Wire Wire Line
	5800 3650 5800 3850
$Comp
L Device:R R14
U 1 1 5DE507D7
P 6550 5350
F 0 "R14" V 6550 5350 50  0000 C CNN
F 1 "1K" V 6600 5150 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 6480 5350 50  0001 C CNN
F 3 "~" H 6550 5350 50  0001 C CNN
	1    6550 5350
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R15
U 1 1 5DE507DE
P 6550 5450
F 0 "R15" V 6550 5450 50  0000 C CNN
F 1 "1K" V 6600 5250 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 6480 5450 50  0001 C CNN
F 3 "~" H 6550 5450 50  0001 C CNN
	1    6550 5450
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6650 4650 7400 4650
Wire Wire Line
	7400 4650 7400 4750
Wire Wire Line
	6700 5350 7500 5350
Wire Wire Line
	7500 5350 7500 5250
Wire Wire Line
	7700 5450 7700 5250
Wire Wire Line
	8000 5250 8000 5300
Wire Wire Line
	8000 5300 7800 5300
Wire Wire Line
	7800 5300 7800 5250
Wire Wire Line
	6700 5450 7700 5450
Wire Wire Line
	3800 4550 3400 4550
Wire Wire Line
	3400 4550 3400 5450
Wire Wire Line
	3400 5450 6400 5450
Wire Wire Line
	3800 4650 3450 4650
Wire Wire Line
	5300 4250 6350 4250
Wire Wire Line
	5300 4350 6350 4350
Wire Wire Line
	3800 5050 3700 5050
Wire Wire Line
	3700 5050 3700 6550
Wire Wire Line
	3700 6550 4450 6550
Connection ~ 4450 6550
Wire Wire Line
	4450 6550 4450 6750
Wire Wire Line
	3800 4750 3650 4750
Wire Wire Line
	3650 4750 3650 6650
Wire Wire Line
	3650 6650 4150 6650
Connection ~ 4150 6650
Wire Wire Line
	4150 6650 4150 6750
Wire Wire Line
	5250 7250 5250 7200
Wire Wire Line
	5300 5050 5400 5050
Wire Wire Line
	5400 5050 5400 6550
Wire Wire Line
	5400 6550 4750 6550
Connection ~ 4750 6550
Wire Wire Line
	4750 6550 4750 6750
Wire Wire Line
	5300 4950 5450 4950
Wire Wire Line
	5450 4950 5450 6650
Wire Wire Line
	5450 6650 5050 6650
Connection ~ 5050 6650
Wire Wire Line
	5050 6650 5050 6750
$Comp
L Device:C C12
U 1 1 5DF3C419
P 7400 5950
F 0 "C12" H 7200 5850 50  0000 L CNN
F 1 "100nf / 50v" H 6900 5750 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 7438 5800 50  0001 C CNN
F 3 "~" H 7400 5950 50  0001 C CNN
	1    7400 5950
	1    0    0    -1  
$EndComp
Wire Wire Line
	7000 5700 7000 5750
Wire Wire Line
	7000 5750 7050 5750
Wire Wire Line
	7350 5750 7400 5750
Wire Wire Line
	7400 5750 7400 5800
Wire Wire Line
	7400 5250 7400 5750
Connection ~ 7400 5750
Wire Wire Line
	7600 5250 7600 6150
Wire Wire Line
	7400 6100 7400 6150
Wire Wire Line
	7400 6150 7600 6150
Connection ~ 7600 6150
Wire Wire Line
	7600 6150 7600 6200
Wire Wire Line
	5650 7100 5650 7200
$Comp
L Device:C C9
U 1 1 5DFE4E5F
P 5450 7200
F 0 "C9" V 5400 7500 50  0000 C CNN
F 1 "100nf / 50v" V 5500 7700 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 5488 7050 50  0001 C CNN
F 3 "~" H 5450 7200 50  0001 C CNN
	1    5450 7200
	0    1    1    0   
$EndComp
Connection ~ 5650 7100
Wire Wire Line
	5150 7100 5650 7100
Wire Wire Line
	5050 6100 5650 6100
Wire Wire Line
	5300 7200 5250 7200
Connection ~ 5250 7200
Wire Wire Line
	5600 7200 5650 7200
Connection ~ 5650 7200
Wire Wire Line
	5650 7200 5650 7300
Wire Wire Line
	5250 6900 5250 7200
$Comp
L power:GND #PWR013
U 1 1 5E062795
P 5900 4950
F 0 "#PWR013" H 5900 4700 50  0001 C CNN
F 1 "GND" H 5905 4777 50  0001 C CNN
F 2 "" H 5900 4950 50  0001 C CNN
F 3 "" H 5900 4950 50  0001 C CNN
	1    5900 4950
	1    0    0    -1  
$EndComp
Wire Wire Line
	5300 4550 5450 4550
Wire Wire Line
	5900 4550 5900 4950
$Comp
L Regulator_Linear:LM1117-3.3 U1
U 1 1 5E867B53
P 2950 1300
F 0 "U1" H 2950 1542 50  0000 C CNN
F 1 "LM1117-3.3" H 2950 1451 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-223-3_TabPin2" H 2950 1300 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/lm1117.pdf" H 2950 1300 50  0001 C CNN
	1    2950 1300
	1    0    0    -1  
$EndComp
$Comp
L Device:D D4
U 1 1 5E88B807
P 2950 1850
F 0 "D4" V 2950 1750 50  0000 R CNN
F 1 "LL4148" V 2600 2000 50  0000 R CNN
F 2 "Diode_SMD:D_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 2950 1850 50  0001 C CNN
F 3 "~" H 2950 1850 50  0001 C CNN
	1    2950 1850
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2950 2000 2950 2050
Wire Wire Line
	2150 1300 2500 1300
$Comp
L power:GND #PWR04
U 1 1 5E8B0339
P 2150 2050
F 0 "#PWR04" H 2150 1800 50  0001 C CNN
F 1 "GND" H 2155 1877 50  0001 C CNN
F 2 "" H 2150 2050 50  0001 C CNN
F 3 "" H 2150 2050 50  0001 C CNN
	1    2150 2050
	1    0    0    -1  
$EndComp
Wire Wire Line
	2150 1300 2150 1550
Wire Wire Line
	3600 1300 3600 1550
Wire Wire Line
	4000 1300 4000 1550
Wire Notes Line
	4550 2400 650  2400
$Comp
L Connector_Generic:Conn_01x01 X1
U 1 1 5E85E185
P 5400 1300
F 0 "X1" V 5500 1300 50  0000 R CNN
F 1 "X" V 5500 1400 50  0000 R CNN
F 2 "MountingHole:MountingHole_3.5mm_Pad" H 5400 1300 50  0001 C CNN
F 3 "~" H 5400 1300 50  0001 C CNN
	1    5400 1300
	0    -1   -1   0   
$EndComp
$Comp
L Connector_Generic:Conn_01x01 X2
U 1 1 5E870C01
P 5650 1300
F 0 "X2" V 5750 1300 50  0000 R CNN
F 1 "X" V 5750 1400 50  0000 R CNN
F 2 "MountingHole:MountingHole_3.5mm_Pad" H 5650 1300 50  0001 C CNN
F 3 "~" H 5650 1300 50  0001 C CNN
	1    5650 1300
	0    -1   -1   0   
$EndComp
$Comp
L Connector_Generic:Conn_01x01 X3
U 1 1 5E879DFA
P 5900 1300
F 0 "X3" V 6000 1300 50  0000 R CNN
F 1 "X" V 6000 1400 50  0000 R CNN
F 2 "MountingHole:MountingHole_3.5mm_Pad" H 5900 1300 50  0001 C CNN
F 3 "~" H 5900 1300 50  0001 C CNN
	1    5900 1300
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR019
U 1 1 5E88CFA6
P 5400 1500
F 0 "#PWR019" H 5400 1250 50  0001 C CNN
F 1 "GND" H 5405 1327 50  0001 C CNN
F 2 "" H 5400 1500 50  0001 C CNN
F 3 "" H 5400 1500 50  0001 C CNN
	1    5400 1500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR020
U 1 1 5E896195
P 5650 1500
F 0 "#PWR020" H 5650 1250 50  0001 C CNN
F 1 "GND" H 5655 1327 50  0001 C CNN
F 2 "" H 5650 1500 50  0001 C CNN
F 3 "" H 5650 1500 50  0001 C CNN
	1    5650 1500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR021
U 1 1 5E89F384
P 5900 1500
F 0 "#PWR021" H 5900 1250 50  0001 C CNN
F 1 "GND" H 5905 1327 50  0001 C CNN
F 2 "" H 5900 1500 50  0001 C CNN
F 3 "" H 5900 1500 50  0001 C CNN
	1    5900 1500
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C14
U 1 1 5ED72089
P 2500 1700
F 0 "C14" H 2550 1550 50  0000 L CNN
F 1 "100uf / 16v" H 2300 1100 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D6.3mm_P2.50mm" H 2538 1550 50  0001 C CNN
F 3 "~" H 2500 1700 50  0001 C CNN
	1    2500 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	2500 1550 2500 1300
Connection ~ 2500 1300
Wire Wire Line
	2500 1300 2650 1300
$Comp
L Device:R R18
U 1 1 5ED8D71C
P 3150 1650
F 0 "R18" V 3150 1650 50  0000 C CNN
F 1 "1K" V 3250 1700 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 3080 1650 50  0001 C CNN
F 3 "~" H 3150 1650 50  0001 C CNN
	1    3150 1650
	0    1    1    0   
$EndComp
Wire Wire Line
	3600 1850 3600 2050
Wire Wire Line
	4000 1850 4000 2050
Wire Wire Line
	2500 1850 2500 2050
Wire Wire Line
	2150 1850 2150 2050
Wire Wire Line
	2950 1600 2950 1650
Wire Wire Line
	3250 1300 3350 1300
Wire Wire Line
	3000 1650 2950 1650
Connection ~ 2950 1650
Wire Wire Line
	2950 1650 2950 1700
Wire Wire Line
	3300 1650 3350 1650
Wire Wire Line
	3350 1650 3350 1300
Connection ~ 3350 1300
Wire Wire Line
	3350 1300 3600 1300
$Comp
L Device:D D7
U 1 1 5EE1D488
P 7300 2750
F 0 "D7" H 7300 2850 50  0000 C CNN
F 1 "M7" H 7300 2625 50  0000 C CNN
F 2 "Diode_SMD:D_SMA" H 7300 2750 50  0001 C CNN
F 3 "~" H 7300 2750 50  0001 C CNN
	1    7300 2750
	-1   0    0    1   
$EndComp
$Comp
L Device:D D6
U 1 1 5EE1D796
P 7200 5750
F 0 "D6" H 7200 5850 50  0000 C CNN
F 1 "M7" H 7200 5625 50  0000 C CNN
F 2 "Diode_SMD:D_SMA" H 7200 5750 50  0001 C CNN
F 3 "~" H 7200 5750 50  0001 C CNN
	1    7200 5750
	-1   0    0    1   
$EndComp
Wire Wire Line
	5300 4850 5550 4850
Wire Wire Line
	5550 4850 5550 5350
Wire Wire Line
	5550 5350 6400 5350
Wire Wire Line
	6350 4450 6050 4450
Wire Wire Line
	6050 4450 6050 5250
Wire Wire Line
	6050 5250 3450 5250
Wire Wire Line
	3450 5250 3450 4650
Wire Wire Line
	6350 4650 6000 4650
Wire Wire Line
	6000 4650 6000 4450
Wire Wire Line
	6000 4450 5300 4450
Wire Wire Line
	6350 4550 6150 4550
Wire Wire Line
	6150 4550 6150 4750
Wire Wire Line
	6150 4750 5300 4750
Wire Wire Line
	5900 4550 5750 4550
Wire Wire Line
	9750 2750 9750 3250
Wire Wire Line
	9400 3250 9750 3250
Connection ~ 9750 3250
Wire Wire Line
	9750 3250 9750 3800
$Comp
L Device:R R19
U 1 1 5EDD7724
P 8450 4150
F 0 "R19" V 8450 4150 50  0000 C CNN
F 1 "1K" V 8400 4350 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 8380 4150 50  0001 C CNN
F 3 "~" H 8450 4150 50  0001 C CNN
	1    8450 4150
	0    1    1    0   
$EndComp
Wire Wire Line
	5300 4150 8300 4150
Wire Wire Line
	8600 4150 9650 4150
Wire Wire Line
	7600 3200 7600 3250
Wire Wire Line
	7600 2900 7600 2850
Connection ~ 7600 2850
Wire Wire Line
	7800 3250 7600 3250
Connection ~ 7600 3250
Wire Wire Line
	7600 3250 7600 3550
$EndSCHEMATC
