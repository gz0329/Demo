
'''
Copyright (c) 2018, Elliott Bay Industries. All rights reserved.

Purpose:
Do a frequency sweep on SG6000 RF signal generator.
Read data back from hp8563e spectrum analyzer.

History:
2019-06-13  1.00  George Zhang   Initial implementation.
'''
import serial
import time
#import visa
from openpyxl import Workbook
from openpyxl.chart import (
    ScatterChart,
    AreaChart3D,
    LineChart,
    Reference,
    Series,
)

s_sg = serial.Serial( "COM6", 115200, timeout = 1 )
s_hp = serial.Serial( "COM9", 115200, timeout = 1 )

File_name = "Fanout_8p5_layered_test_2_2to9Ghz_-10to10dBm"
CSV_name = File_name + ".csv"
XLSX_name = File_name + ".xlsx"

f_w = open( CSV_name, "w")

f_r = open("Calibration.csv", "r")
#Lines = f_r.readlines()

Hp_result_array = []
Cat_size = 0
'''
def hp_get_data_auto( Curr_dBm, Curr_Mhz, Start_Mhz, Stop_Mhz ):
	if s_hp.isOpen():
		s_hp.write( b'MKPK;\n')
		time.sleep( 0.1 )
		s_hp.write( b'MKA?;\n')
		time.sleep( 0.1 )
		Amp = s_hp.readline().decode( 'UTF-8' )
		s_hp.write( b'MKF?;\n')
		time.sleep( 0.1 )
		Freq = s_hp.readline().decode( 'UTF-8' )
		Freq_float = float( Freq[1:-1] ) / 1000000.0
		Amp_float = float( Amp[1:-1] )
		Result_str = str( Curr_dBm ) + ' ' + Freq[1:-1] + ' ' + Amp[1:-1]
		print( Result_str )
		if Amp_float >= -50.0 and Amp_float <= 10.0 and Freq_float >= Start_Mhz and Freq_float <= Stop_Mhz:
			Result = [ Amp_float, Freq_float, Curr_dBm ]
			Hp_result_array.append( Result )
			global Cat_size
			Cat_size += 1
		#print( Hp_result_array )
'''

Calibration_container = []
def set_calibration_container():
	for Lines in f_r:
		Strings = Lines.split()
		#print( Strings )
		Amp = Strings[ 0 ][ : -1 ]
		Freq = Strings[ 1 ][ : -1 ]
		Compensate_dBm = Strings[ 3 ]
		Result = [ float( Amp ), float( Freq ), float( Compensate_dBm )]
		Calibration_container.append( Result )

	#print( Calibration_container )

def get_compensate_dBm( Curr_dBm, Curr_Mhz ):

	for Samples in Calibration_container:
		if Samples[ 0 ] == float( Curr_dBm ) and Samples[ 1 ] == float( Curr_Mhz ):			
			return Samples[ 2 ]

	return -1

Freq_span_percentage = 0.01
def hp_get_data_manual( Curr_dBm, Curr_Mhz, Compensate_dBm, Repeat_number ):
	Min_Mhz = ( 1 - Freq_span_percentage ) * Curr_Mhz
	Max_Mhz = ( 1 + Freq_span_percentage ) * Curr_Mhz

	Valid_counter = 0
	Amp_sum = 0
	Freq_sum = 0
	Amp_average = 0
	Freq_average = 0

	if s_hp.isOpen():
		while Valid_counter < Repeat_number:
			#Command = 'CF ' + str( Curr_Mhz * 3 ) + 'MHZ;\n'
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
			Amp_float_compensated = Amp_float + Compensate_dBm

			# Try different settings to filter out noise in different test boards
			# Setting for Amplifiers
			#if Amp_float >= ( Curr_dBm - 10.0 ) and Amp_float <= ( Curr_dBm + 30.0 ) and Freq_float >= Min_Mhz and Freq_float <= Max_Mhz:
			# Setting for Fanouts
			if Amp_float >= ( Curr_dBm - 30.0 ) and Amp_float <= ( Curr_dBm ) and Freq_float >= Min_Mhz and Freq_float <= Max_Mhz:
			# Setting for Tripler
			#if Amp_float >= ( Curr_dBm - 35.0 ) and Amp_float <= ( Curr_dBm ) and Freq_float >= Min_Mhz and Freq_float <= Max_Mhz:
			# Setting for filter
			#if  Freq_float >= Min_Mhz and Freq_float <= Max_Mhz and ( Curr_Mhz < 8000.0 or Curr_Mhz > 9000.0 ):
				print( 'Passed\n')
				Valid_counter = Valid_counter + 1
				Amp_float_compensated = Amp_float + Compensate_dBm
				Amp_sum = Amp_sum + Amp_float_compensated
				Freq_sum = Freq_sum + Freq_float

				#This block is for testing the filter
				'''
			elif Curr_Mhz >= 8000.0 and Curr_Mhz <= 9000.0 and Amp_float >= ( Curr_dBm - 36.0 ) and Amp_float <= ( Curr_dBm ):
				print( 'In range ones passed\n')
				Valid_counter = Valid_counter + 1
				Amp_float_compensated = Amp_float + Compensate_dBm
				Amp_sum = Amp_sum + Amp_float_compensated
				Freq_sum = Freq_sum + Freq_float
			'''	
			else:
				print( 'Failed\n' )

		Amp_average = Amp_sum / Repeat_number
		Freq_average = Freq_sum / Repeat_number

		Result = [ round( Amp_average, 1 ), Freq_average, Curr_dBm ]
		Hp_result_array.append( Result )
		global Cat_size
		Cat_size += 1

		Result_str = str( Curr_dBm ) + ', ' + str( Amp_average ) + ', ' + str( Freq_average ) + ', ' + str( round( Amp_float_compensated, 1 ) ) +'\n'
		f_w.write( Result_str )
		f_w.flush()
		print( Result_str )

wb = Workbook()
ws = wb.active
Size = []
def draw_chart_excel():
	for row in Hp_result_array:
		ws.append( row )

	c1 = ScatterChart()
	c1.title = "Amp vs Freq at different input levels"
	c1.style = 13
	c1.x_axis.title = 'Freq (Mhz)'
	c1.y_axis.title = 'Amp (dBm)'
	
	Pre_stop = 0
	for i in range ( len( Size ) ):
		Start_row = Pre_stop + 1
		Stop_row = Start_row + Size[ i ] - 1

		if Start_row < Stop_row:
			#print( 'Start from: ' + str( Start_row ) )
			#print( 'Stop from: ' + str( Stop_row ) )
			data_y = Reference(ws, min_col=1, min_row= Start_row, max_row= Stop_row )
			data_x = Reference(ws, min_col=2, min_row= Start_row, max_row= Stop_row )

			Series_name = str( Hp_result_array[ Start_row ] [ 2 ] ) + 'dBm input'
			s = Series( data_y, xvalues = data_x, title = Series_name )
			#print( Series_name )
			c1.append( s )

			Pre_stop = Stop_row
		else:
			print( "Wrong setting\n")
	
	ws.add_chart( c1, "A10" )
	wb.save( XLSX_name )

def hp_display_config():
	if s_hp.isOpen():
		s_hp.write( b'FS;\n')
		time.sleep( 0.1 )
		s_hp.write( b'SP 100MHZ;\n')
		time.sleep( 0.1 )

'''
def sg_sweep_auto( Start_Mhz, Stop_Mhz, Points, Dwell_ms, Amplitude ):
	if s_sg.isOpen():
		s_sg.write( b'SWE:MODE SCAN\n')
		time.sleep( 0.1 )
		Command = 'FREQ:START ' + str( Start_Mhz ) + 'MHZ\n'
		#s_sg.write( b( 'FREQ:START ' + str( Start_Mhz ) + 'MHZ\n' ) )
		s_sg.write( Command.encode( 'UTF-8' ) )
		time.sleep( 0.1 )
		Command = 'FREQ:STOP ' + str( Stop_Mhz ) + 'MHZ\n'
		s_sg.write( Command.encode( 'UTF-8' ) )
		time.sleep( 0.1 )
		Command = 'SWE:POINTS ' + str( Points ) + '\n'
		s_sg.write( Command.encode( 'UTF-8' ) )
		time.sleep( 0.1 )
		Command = 'SWE:DWELL ' + str( Dwell_ms ) + '\n'
		s_sg.write( Command.encode( 'UTF-8' ) )
		time.sleep( 0.1 )
		s_sg.write( b'LIST:DIR UP\n')
		time.sleep( 0.1 )
		s_sg.write( b'INIT:CONT 0\n')
		time.sleep( 0.1 )
		s_sg.write( b'TRIG:SWEEP\n')
		time.sleep( 0.1 )
		s_sg.write( b'INIT:IMM\n')

		Is_sweeping = True
		global Cat_size
		Cat_size = 0
		while Is_sweeping:
			time.sleep( 0.2 )
			hp_get_data( Amplitude, Start_Mhz, Stop_Mhz )
			s_sg.write( b'SWE:ACTIVE?\n' )
			r = s_sg.readline()
			if r == b'0\r\n':
				Is_sweeping = False
		#print( Cat_size )
		Size.append( Cat_size )

def sg_sweep_amp_freq( Start_dBm, Stop_dBm, Step_dBm, Start_Mhz, Stop_Mhz, Points, Dwell_ms ):
	if s_sg.isOpen():
		if Start_dBm < Min_dBm:
			Start_dBm = Min_dBm
		if Stop_dBm > Max_dBm:
			Stop_dBm = Max_dBm

		Current_dBm = Start_dBm
		while Current_dBm <= Stop_dBm:
			#s_sg.write( b'VERNIER 0\n')
			time.sleep( 0.1 )
			Command = 'POWER ' + str( Current_dBm ) + '\n'
			s_sg.write( Command.encode( 'UTF-8' ) )
			time.sleep( 0.1 )
			#print( 'At ' + str( Current_dBm ) + 'dBm: \n' )
			sg_sweep_auto( Start_Mhz, Stop_Mhz, Points, Dwell_ms, Current_dBm )
			
			Current_dBm = Current_dBm + Step_dBm
'''
Min_dBm = -21.5
Max_dBm = 10.0
def sg_sweep_manual( Start_dBm, Stop_dBm, Step_dBm, Start_Mhz, Stop_Mhz, Points, Repeat_number ):

	if s_sg.isOpen():
		if Start_dBm < Min_dBm:
			Start_dBm = Min_dBm
		if Stop_dBm > Max_dBm:
			Stop_dBm = Max_dBm

	Step_Mhz = float( Stop_Mhz - Start_Mhz ) / float( Points - 1 )
	Curr_dBm = Start_dBm

	while Curr_dBm <= Stop_dBm:
		#s_sg.write( b'VERNIER 0\n')
		Curr_Mhz = float( Start_Mhz )
		time.sleep( 0.1 )
		Command = 'POWER ' + str( Curr_dBm ) + '\n'
		s_sg.write( Command.encode( 'UTF-8' ) )
		time.sleep( 0.1 )
		#print( 'At ' + str( Curr_dBm ) + 'dBm: \n' )
		if s_sg.isOpen():
			global Cat_size
			Cat_size = 0
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
				time.sleep( 0.1 )
				# Upper line for Tripler
				#Compensate_dBm = get_compensate_dBm( Curr_dBm, Curr_Mhz * 3 )
				Compensate_dBm = get_compensate_dBm( Curr_dBm, Curr_Mhz )
				print( Compensate_dBm )
				if Compensate_dBm >= 0:
					# Upper line for Tripler
					#hp_get_data_manual( Curr_dBm, Curr_Mhz * 3, Compensate_dBm, Repeat_number ) 
					hp_get_data_manual( Curr_dBm, Curr_Mhz, Compensate_dBm, Repeat_number )
				Curr_Mhz = Curr_Mhz + Step_Mhz
			Size.append( Cat_size )
		Curr_dBm = Curr_dBm + Step_dBm
		#print( Cat_size )

def main():
	
	hp_display_config()
	print ('Running...')
	set_calibration_container()
	'''
	while 1:
		Freq = input( 'Frequency?\n')
		Amp = input( 'Amplitude?\n')
		Compensate_dBm = get_compensate_dBm( Amp, Freq )
		print( Compensate_dBm )
	'''
	#To get 100Mhz step frequency, Points = ( Stop_Mhz - Start_Mhz ) / 10 + 1
	#sg_sweep_manual( Start_dBm, Stop_dBm, Step_dBm, Start_Mhz, Stop_Mhz, Points, Repeat_number ):

	sg_sweep_manual( -10, 10, 2, 2000.0, 9000.0, 71, 3 )
	draw_chart_excel()
	s_hp.close()
	s_sg.close()
	f_r.close()
	f_w.close()

	print ('Complete!')

main()