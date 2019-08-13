'''
Copyright (c) 2018, Elliott Bay Industries. All rights reserved.

Purpose:
Do a frequency sweep on SG6000 RF signal generator.
Read data back from hp8563e spectrum analyzer.
Generate a calibration file for later tests.

History:
2019-07-12  1.00  George Zhang   Initial implementation.
'''

import serial
import time
import json

s_sg = serial.Serial( "COM6", 115200, timeout = 1 )
s_hp = serial.Serial( "COM9", 115200, timeout = 1 )

f_w = open("Calibration.csv", "w")

def hp_display_config():
	if s_hp.isOpen():
		s_hp.write( b'FS;\n')
		time.sleep( 0.1 )
		s_hp.write( b'SP 100MHZ;\n')
		time.sleep( 0.1 )

Min_dBm = -21.5
Max_dBm = 10.0

Freq_span_percentage = 0.01
def hp_filter_data( Curr_dBm, Curr_Mhz, Repeat_number ):
	Min_Mhz = ( 1 - Freq_span_percentage ) * Curr_Mhz
	Max_Mhz = ( 1 + Freq_span_percentage ) * Curr_Mhz

	print( 'Min freq is ' + str( Min_Mhz ) + 'Mhz\n' )
	print( 'Max freq is ' + str( Max_Mhz ) + 'Mhz\n' )

	Valid_counter = 0
	Amp_sum = 0
	Amp_average = 0

	if s_hp.isOpen():
		while Valid_counter < Repeat_number:
			Command = 'CF ' + str( Curr_Mhz ) + 'MHZ;\n'
			s_hp.write( Command.encode( 'UTF-8' ) )
			time.sleep( 0.2 )
			s_hp.write( b'MKPK;\n')
			time.sleep( 0.2 )
			s_hp.write( b'MKA?;\n')
			time.sleep( 0.2 )
			Amp = s_hp.readline().decode( 'UTF-8' )
			s_hp.write( b'MKF?;\n')
			time.sleep( 0.2 )
			Freq = s_hp.readline().decode( 'UTF-8' )

			Freq_float = float( Freq[1:-1] ) / 1000000.0
			Amp_float = float( Amp[1:-1] )
			print( 'Current freq is ' + str( Freq_float ) + 'Mhz\n' )
			print( 'Current amp is ' + str( Amp_float ) + 'dBm\n' )
			if Amp_float >= ( Curr_dBm - 30.0 ) and Amp_float <= Curr_dBm and Freq_float >= Min_Mhz and Freq_float <= Max_Mhz:
				print( 'Passed\n')
				Valid_counter = Valid_counter + 1
				Amp_sum = Amp_sum + Amp_float
		
		Amp_average = Amp_sum / Repeat_number	
		print( 'Averaged amp is ' + str( Amp_average ) + 'dBm\n' )

		Amp_compensate = Curr_dBm - Amp_average

		Result_str = str( Curr_dBm ) + ', ' + str( Curr_Mhz ) + ', ' + str( Amp_average ) + ', ' + str( Amp_compensate ) + '\n'
		f_w.write( Result_str )
		f_w.flush()
		print( Result_str )

def sg_sweep_manual( Start_dBm, Stop_dBm, Step_dBm, Start_Mhz, Stop_Mhz, Points, Repeat_number ):

	if s_sg.isOpen():
		if Start_dBm < Min_dBm:
			Start_dBm = Min_dBm
		if Stop_dBm > Max_dBm:
			Stop_dBm = Max_dBm

	Step_Mhz = float( Stop_Mhz - Start_Mhz ) / float( Points - 1 )
	Curr_dBm = Start_dBm

	while Curr_dBm <= Stop_dBm:
		Curr_Mhz = float( Start_Mhz )
		time.sleep( 0.1 )
		Command = 'POWER ' + str( Curr_dBm ) + '\n'
		s_sg.write( Command.encode( 'UTF-8' ) )
		time.sleep( 0.1 )
		#print( 'At ' + str( Curr_dBm ) + 'dBm: \n' )
		if s_sg.isOpen():
			while Curr_Mhz <= Stop_Mhz:
				#print( str( Curr_Mhz ) )
				s_sg.write( b'OUTP:STAT OFF\n')
				time.sleep( 0.1 )
				Command = 'POWER ' + str( Curr_dBm ) + '\n'
				s_sg.write( Command.encode( 'UTF-8' ) )
				time.sleep( 0.1 )
				Command = 'FREQ:CW ' + str( Curr_Mhz ) + 'MHZ\n'
				#Command = 'FREQ:CW ' + str( Curr_Mhz * 3 ) + 'MHZ\n'
				s_sg.write( Command.encode( 'UTF-8' ) )
				time.sleep( 0.1 )
				s_sg.write( b'OUTP:STAT ON\n')
				time.sleep( 0.2 )

				hp_filter_data( Curr_dBm, Curr_Mhz, Repeat_number )
				Curr_Mhz = Curr_Mhz + Step_Mhz
		Curr_dBm = Curr_dBm + Step_dBm

def main():
	hp_display_config()
	print ('Running...')
	#sg_sweep_manual( Start_dBm, Stop_dBm, Step_dBm, Start_Mhz, Stop_Mhz, Points, Repeat_number )
	sg_sweep_manual( -20.0, 10.0, 2.0, 1000, 12000, 111, 3 )
	s_hp.close()
	s_sg.close()
	print ('Complete!')

main()