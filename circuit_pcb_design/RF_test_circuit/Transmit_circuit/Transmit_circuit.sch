EESchema Schematic File Version 4
LIBS:Transmit_circuit-cache
EELAYER 26 0
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
L Device:R R2
U 1 1 5CD1F583
P 3250 2850
F 0 "R2" V 3350 2850 50  0000 C CNN
F 1 "4k" V 3134 2850 50  0000 C CNN
F 2 "EllBayInd-McFly:R_0201_0603Metric" V 3180 2850 50  0001 C CNN
F 3 "~" H 3250 2850 50  0001 C CNN
	1    3250 2850
	0    1    1    0   
$EndComp
$Comp
L EllBayInd-McFly:+5V #PWR0101
U 1 1 5CD1F640
P 3000 2850
F 0 "#PWR0101" H 3000 2700 50  0001 C CNN
F 1 "+5V" V 3015 2978 50  0000 L CNN
F 2 "" H 3000 2850 50  0001 C CNN
F 3 "" H 3000 2850 50  0001 C CNN
	1    3000 2850
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R1
U 1 1 5CD1F65F
P 2900 3250
F 0 "R1" V 2693 3250 50  0000 C CNN
F 1 "100" V 2784 3250 50  0000 C CNN
F 2 "EllBayInd-McFly:R_0402" V 2830 3250 50  0001 C CNN
F 3 "~" H 2900 3250 50  0001 C CNN
	1    2900 3250
	0    1    1    0   
$EndComp
$Comp
L EllBayInd-McFly:NFET U1
U 1 1 5CD1F6C1
P 3450 3250
F 0 "U1" H 3578 3296 50  0000 L CNN
F 1 "NFET" H 3578 3205 50  0000 L CNN
F 2 "EllBayInd-McFly:NFET" H 3450 3250 50  0001 C CNN
F 3 "" H 3450 3250 50  0001 C CNN
	1    3450 3250
	1    0    0    -1  
$EndComp
$Comp
L EllBayInd-McFly:GND #PWR0102
U 1 1 5CD1F722
P 3450 3550
F 0 "#PWR0102" H 3450 3300 50  0001 C CNN
F 1 "GND" H 3455 3377 50  0000 C CNN
F 2 "" H 3450 3550 50  0001 C CNN
F 3 "" H 3450 3550 50  0001 C CNN
	1    3450 3550
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x01_Male J1
U 1 1 5CD1F814
P 2500 3250
F 0 "J1" H 2500 3350 50  0000 C CNN
F 1 "Conn_01x01_Male" H 2500 3100 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x01_P2.54mm_Vertical" H 2500 3250 50  0001 C CNN
F 3 "~" H 2500 3250 50  0001 C CNN
	1    2500 3250
	1    0    0    -1  
$EndComp
$Comp
L EllBayInd-McFly:MAAL-011130 U2
U 1 1 5CD1F875
P 4100 2700
F 0 "U2" H 4100 3075 50  0000 C CNN
F 1 "MAAL-011130" H 4100 2984 50  0000 C CNN
F 2 "EllBayInd-McFly:MAAL-011130" H 4100 2700 50  0001 C CNN
F 3 "" H 4100 2700 50  0001 C CNN
	1    4100 2700
	1    0    0    -1  
$EndComp
$Comp
L EllBayInd-McFly:TCBT-14+ U3
U 1 1 5CD1F9C5
P 5400 2750
F 0 "U3" H 5350 3075 50  0000 C CNN
F 1 "TCBT-14+" H 5350 2984 50  0000 C CNN
F 2 "EllBayInd-McFly:TCBT-14+" H 5400 2750 50  0001 C CNN
F 3 "" H 5400 2750 50  0001 C CNN
	1    5400 2750
	1    0    0    -1  
$EndComp
$Comp
L EllBayInd-McFly:GND #PWR0103
U 1 1 5CD1FA1B
P 3600 2550
F 0 "#PWR0103" H 3600 2300 50  0001 C CNN
F 1 "GND" V 3605 2422 50  0000 R CNN
F 2 "" H 3600 2550 50  0001 C CNN
F 3 "" H 3600 2550 50  0001 C CNN
	1    3600 2550
	0    1    1    0   
$EndComp
$Comp
L EllBayInd-McFly:GND #PWR0104
U 1 1 5CD1FA38
P 4600 2550
F 0 "#PWR0104" H 4600 2300 50  0001 C CNN
F 1 "GND" V 4605 2422 50  0000 R CNN
F 2 "" H 4600 2550 50  0001 C CNN
F 3 "" H 4600 2550 50  0001 C CNN
	1    4600 2550
	0    -1   -1   0   
$EndComp
$Comp
L EllBayInd-McFly:GND #PWR0105
U 1 1 5CD1FA5B
P 5350 3150
F 0 "#PWR0105" H 5350 2900 50  0001 C CNN
F 1 "GND" H 5355 2977 50  0000 C CNN
F 2 "" H 5350 3150 50  0001 C CNN
F 3 "" H 5350 3150 50  0001 C CNN
	1    5350 3150
	1    0    0    -1  
$EndComp
$Comp
L EllBayInd-McFly:GND #PWR0106
U 1 1 5CD1FA78
P 5950 3250
F 0 "#PWR0106" H 5950 3000 50  0001 C CNN
F 1 "GND" H 5955 3077 50  0000 C CNN
F 2 "" H 5950 3250 50  0001 C CNN
F 3 "" H 5950 3250 50  0001 C CNN
	1    5950 3250
	1    0    0    -1  
$EndComp
$Comp
L EllBayInd-McFly:+5V #PWR0107
U 1 1 5CD1FAB4
P 5950 2800
F 0 "#PWR0107" H 5950 2650 50  0001 C CNN
F 1 "+5V" H 5965 2973 50  0000 C CNN
F 2 "" H 5950 2800 50  0001 C CNN
F 3 "" H 5950 2800 50  0001 C CNN
	1    5950 2800
	1    0    0    -1  
$EndComp
$Comp
L Device:C C1
U 1 1 5CD1FB35
P 5950 3050
F 0 "C1" H 6065 3096 50  0000 L CNN
F 1 "0.1uF" H 6065 3005 50  0000 L CNN
F 2 "EllBayInd-McFly:C_0402" H 5988 2900 50  0001 C CNN
F 3 "~" H 5950 3050 50  0001 C CNN
	1    5950 3050
	1    0    0    -1  
$EndComp
$Comp
L EllBayInd-McFly:BFCN-8450+ U4
U 1 1 5CD1FC32
P 7000 2700
F 0 "U4" H 7000 3015 50  0000 C CNN
F 1 "BFCN-8450+" H 7000 2924 50  0000 C CNN
F 2 "EllBayInd-McFly:BFCN-8450+" H 7000 2700 50  0001 C CNN
F 3 "" H 7000 2700 50  0001 C CNN
	1    7000 2700
	1    0    0    -1  
$EndComp
$Comp
L EllBayInd-McFly:GND #PWR0108
U 1 1 5CD1FCC8
P 6450 2800
F 0 "#PWR0108" H 6450 2550 50  0001 C CNN
F 1 "GND" V 6455 2672 50  0000 R CNN
F 2 "" H 6450 2800 50  0001 C CNN
F 3 "" H 6450 2800 50  0001 C CNN
	1    6450 2800
	0    1    1    0   
$EndComp
$Comp
L EllBayInd-McFly:GND #PWR0109
U 1 1 5CD1FCEB
P 7550 2800
F 0 "#PWR0109" H 7550 2550 50  0001 C CNN
F 1 "GND" V 7555 2672 50  0000 R CNN
F 2 "" H 7550 2800 50  0001 C CNN
F 3 "" H 7550 2800 50  0001 C CNN
	1    7550 2800
	0    -1   -1   0   
$EndComp
$Comp
L EllBayInd-McFly:GND #PWR0111
U 1 1 5CD1FD9E
P 6750 3550
F 0 "#PWR0111" H 6750 3300 50  0001 C CNN
F 1 "GND" H 6755 3377 50  0000 C CNN
F 2 "" H 6750 3550 50  0001 C CNN
F 3 "" H 6750 3550 50  0001 C CNN
	1    6750 3550
	1    0    0    -1  
$EndComp
$Comp
L EllBayInd-McFly:RF_SMA CON1
U 1 1 5CD1FE46
P 3150 2650
F 0 "CON1" H 3240 2586 40  0000 L CNN
F 1 "RF_SMA" H 3240 2662 40  0000 L CNN
F 2 "EllBayInd-McFly:SMAConnector" H 3150 2650 40  0001 C CNN
F 3 "" H 3150 2650 40  0001 C CNN
	1    3150 2650
	-1   0    0    1   
$EndComp
$Comp
L EllBayInd-McFly:GND #PWR0112
U 1 1 5CD1FF0A
P 3150 2500
F 0 "#PWR0112" H 3150 2250 50  0001 C CNN
F 1 "GND" H 3155 2327 50  0000 C CNN
F 2 "" H 3150 2500 50  0001 C CNN
F 3 "" H 3150 2500 50  0001 C CNN
	1    3150 2500
	-1   0    0    1   
$EndComp
Wire Wire Line
	3650 2650 3250 2650
Wire Wire Line
	3650 2550 3600 2550
Wire Wire Line
	3150 2500 3150 2550
Wire Wire Line
	3000 2850 3100 2850
Wire Wire Line
	3200 3250 3050 3250
Wire Wire Line
	2750 3250 2700 3250
Wire Wire Line
	3450 3500 3450 3550
Wire Wire Line
	4550 2550 4600 2550
Wire Wire Line
	4550 2650 4850 2650
Wire Wire Line
	5850 2850 5950 2850
Wire Wire Line
	5950 2850 5950 2800
Wire Wire Line
	5950 2850 5950 2900
Connection ~ 5950 2850
Wire Wire Line
	5350 3100 5350 3150
Wire Wire Line
	5950 3200 5950 3250
Text Label 4800 2850 2    50   ~ 0
t_out
Wire Wire Line
	4850 2850 4800 2850
Text Label 6450 2650 2    50   ~ 0
t_out
Wire Wire Line
	6500 2650 6450 2650
$Comp
L EllBayInd-McFly:RF_SMA CON2
U 1 1 5CD20A41
P 6750 3300
F 0 "CON2" H 6840 3313 40  0000 L CNN
F 1 "RF_SMA" H 6840 3237 40  0000 L CNN
F 2 "EllBayInd-McFly:SMAConnector" H 6750 3300 40  0001 C CNN
F 3 "" H 6750 3300 40  0001 C CNN
	1    6750 3300
	1    0    0    -1  
$EndComp
Wire Wire Line
	6500 2800 6450 2800
Wire Wire Line
	7500 2800 7550 2800
Text Label 7550 2650 0    50   ~ 0
Filter_out
Wire Wire Line
	7550 2650 7500 2650
Text Label 6600 3300 2    50   ~ 0
Filter_out
Wire Wire Line
	6650 3300 6600 3300
$Comp
L Connector:Conn_01x02_Male J2
U 1 1 5CD22E56
P 4600 3350
F 0 "J2" H 4706 3528 50  0000 C CNN
F 1 "Conn_01x02_Male" H 4706 3437 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 4600 3350 50  0001 C CNN
F 3 "~" H 4600 3350 50  0001 C CNN
	1    4600 3350
	1    0    0    -1  
$EndComp
$Comp
L EllBayInd-McFly:GND #PWR0115
U 1 1 5CD22EA0
P 4850 3450
F 0 "#PWR0115" H 4850 3200 50  0001 C CNN
F 1 "GND" V 4855 3322 50  0000 R CNN
F 2 "" H 4850 3450 50  0001 C CNN
F 3 "" H 4850 3450 50  0001 C CNN
	1    4850 3450
	0    -1   -1   0   
$EndComp
$Comp
L EllBayInd-McFly:+5V #PWR0116
U 1 1 5CD22F1E
P 4850 3350
F 0 "#PWR0116" H 4850 3200 50  0001 C CNN
F 1 "+5V" V 4865 3478 50  0000 L CNN
F 2 "" H 4850 3350 50  0001 C CNN
F 3 "" H 4850 3350 50  0001 C CNN
	1    4850 3350
	0    1    1    0   
$EndComp
Wire Wire Line
	4850 3350 4800 3350
Wire Wire Line
	4850 3450 4800 3450
$Comp
L Device:C C2
U 1 1 5CE47A58
P 3900 3300
F 0 "C2" H 4015 3346 50  0000 L CNN
F 1 "0.01uF" H 4015 3255 50  0000 L CNN
F 2 "EllBayInd-McFly:C_0603" H 3938 3150 50  0001 C CNN
F 3 "~" H 3900 3300 50  0001 C CNN
	1    3900 3300
	1    0    0    -1  
$EndComp
Wire Wire Line
	3400 2850 3450 2850
Wire Wire Line
	3450 3000 3450 2850
Connection ~ 3450 2850
Wire Wire Line
	3450 2850 3550 2850
Wire Wire Line
	3550 2850 3550 3000
Wire Wire Line
	3550 3000 3900 3000
Wire Wire Line
	3900 3000 3900 3150
Connection ~ 3550 2850
Wire Wire Line
	3550 2850 3650 2850
$Comp
L EllBayInd-McFly:GND #PWR0117
U 1 1 5CE4A67F
P 3900 3550
F 0 "#PWR0117" H 3900 3300 50  0001 C CNN
F 1 "GND" H 3905 3377 50  0000 C CNN
F 2 "" H 3900 3550 50  0001 C CNN
F 3 "" H 3900 3550 50  0001 C CNN
	1    3900 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	3900 3550 3900 3450
Wire Wire Line
	6750 3400 6750 3550
$EndSCHEMATC