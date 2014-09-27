EESchema Schematic File Version 2
LIBS:con-headers-jp
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:special
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:level
LIBS:level-cache
EELAYER 27 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 4 9
Title ""
Date "27 sep 2014"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L DBTC-7-152X+ U?
U 1 1 5424F11F
P 4200 3700
F 0 "U?" H 4250 3500 60  0000 C CNN
F 1 "DBTC-7-152X+" H 4250 4100 60  0000 C CNN
F 2 "" H 4050 4200 60  0000 C CNN
F 3 "" H 4050 4200 60  0000 C CNN
	1    4200 3700
	1    0    0    -1  
$EndComp
$Comp
L MMCX U?
U 1 1 5424F1C0
P 6700 4750
F 0 "U?" H 6900 4500 60  0000 C CNN
F 1 "MMCX" H 6700 5000 60  0000 C CNN
F 2 "" H 6700 4750 60  0000 C CNN
F 3 "" H 6700 4750 60  0000 C CNN
	1    6700 4750
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 5424F1CF
P 4200 2700
F 0 "R?" V 4280 2700 40  0000 C CNN
F 1 "0" V 4207 2701 40  0000 C CNN
F 2 "~" V 4130 2700 30  0000 C CNN
F 3 "~" H 4200 2700 30  0000 C CNN
	1    4200 2700
	0    1    1    0   
$EndComp
Wire Wire Line
	4450 2700 5500 2700
Wire Wire Line
	5500 2700 5500 3500
Wire Wire Line
	4900 3500 6150 3500
Wire Wire Line
	4900 3700 6200 3700
Wire Wire Line
	6200 3700 6200 4650
$Comp
L GND #PWR?
U 1 1 5424F205
P 6700 5450
F 0 "#PWR?" H 6700 5450 30  0001 C CNN
F 1 "GND" H 6700 5380 30  0001 C CNN
F 2 "" H 6700 5450 60  0000 C CNN
F 3 "" H 6700 5450 60  0000 C CNN
	1    6700 5450
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 5424F214
P 3550 4050
F 0 "#PWR?" H 3550 4050 30  0001 C CNN
F 1 "GND" H 3550 3980 30  0001 C CNN
F 2 "" H 3550 4050 60  0000 C CNN
F 3 "" H 3550 4050 60  0000 C CNN
	1    3550 4050
	1    0    0    -1  
$EndComp
Wire Wire Line
	3950 2700 3600 2700
Wire Wire Line
	3600 2700 3600 3500
Wire Wire Line
	3600 3500 3050 3500
Connection ~ 3600 3500
Wire Wire Line
	3600 3700 3550 3700
Wire Wire Line
	3550 3700 3550 4050
Connection ~ 5500 3500
Wire Wire Line
	6600 5250 6600 5350
Wire Wire Line
	6600 5350 6800 5350
Wire Wire Line
	6700 5250 6700 5450
Connection ~ 6700 5350
Wire Wire Line
	6800 5350 6800 5250
Text Notes 2150 2500 0    60   ~ 0
The 0 ohm resistor can be added and removed to add the directional coupler to the transmission line.
Text HLabel 6150 3500 2    60   BiDi ~ 0
LO
Text HLabel 3050 3500 0    60   BiDi ~ 0
LO_TAP
Text Notes 6950 4700 0    60   ~ 0
Digikey: ARF1725CT-ND
$EndSCHEMATC
