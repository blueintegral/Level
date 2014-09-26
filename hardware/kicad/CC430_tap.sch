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
Sheet 5 9
Title ""
Date "26 sep 2014"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MMCX U?
U 1 1 5424F71B
P 7600 4000
F 0 "U?" H 7800 3750 60  0000 C CNN
F 1 "MMCX" H 7600 4250 60  0000 C CNN
F 2 "" H 7600 4000 60  0000 C CNN
F 3 "" H 7600 4000 60  0000 C CNN
	1    7600 4000
	1    0    0    -1  
$EndComp
$Comp
L DBTC-7-152X+ U?
U 1 1 5424F739
P 4850 3350
F 0 "U?" H 4900 3150 60  0000 C CNN
F 1 "DBTC-7-152X+" H 4900 3750 60  0000 C CNN
F 2 "" H 4700 3850 60  0000 C CNN
F 3 "" H 4700 3850 60  0000 C CNN
	1    4850 3350
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 5424F748
P 4950 2400
F 0 "R?" V 5030 2400 40  0000 C CNN
F 1 "0" V 4957 2401 40  0000 C CNN
F 2 "~" V 4880 2400 30  0000 C CNN
F 3 "~" H 4950 2400 30  0000 C CNN
	1    4950 2400
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR?
U 1 1 5424F757
P 4200 3650
F 0 "#PWR?" H 4200 3650 30  0001 C CNN
F 1 "GND" H 4200 3580 30  0001 C CNN
F 2 "" H 4200 3650 60  0000 C CNN
F 3 "" H 4200 3650 60  0000 C CNN
	1    4200 3650
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 5424F766
P 7600 4700
F 0 "#PWR?" H 7600 4700 30  0001 C CNN
F 1 "GND" H 7600 4630 30  0001 C CNN
F 2 "" H 7600 4700 60  0000 C CNN
F 3 "" H 7600 4700 60  0000 C CNN
	1    7600 4700
	1    0    0    -1  
$EndComp
Wire Wire Line
	3700 3150 4250 3150
Wire Wire Line
	4700 2400 4100 2400
Wire Wire Line
	4100 2400 4100 3150
Connection ~ 4100 3150
Wire Wire Line
	4250 3350 4200 3350
Wire Wire Line
	4200 3350 4200 3650
Wire Wire Line
	5550 3150 6750 3150
Text HLabel 3700 3150 0    60   BiDi ~ 0
CC430_TAP
Text HLabel 6750 3150 2    60   BiDi ~ 0
MIX_IN
Wire Wire Line
	5200 2400 5750 2400
Wire Wire Line
	5750 2400 5750 3150
Connection ~ 5750 3150
Wire Wire Line
	5550 3350 7100 3350
Wire Wire Line
	7100 3350 7100 3900
Wire Wire Line
	7500 4500 7500 4600
Wire Wire Line
	7500 4600 7700 4600
Wire Wire Line
	7600 4500 7600 4700
Connection ~ 7600 4600
Wire Wire Line
	7700 4600 7700 4500
Text Notes 2700 2150 0    60   ~ 0
The 0 ohm resistor can be added and removed to add the directional coupler to the transmission line.\n
Text Notes 7850 4000 0    60   ~ 0
Digikey: ARF1725CT-ND
$EndSCHEMATC
