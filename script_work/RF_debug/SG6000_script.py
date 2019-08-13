
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

f_w = open("Direct_2cable_test_2to4Ghz_-20to0dBm_A.csv", "w")

f_r = open("Direct_2cable_test_2to9Ghz_-10to10dBm_Mean.csv", "r")
Lines = f_r.readlines()

def spectrum_analyzer_test():
	if s_hp.isOpen():
		print( s_hp.name + ' is open' )
		s_hp.write( b'FS;\n')
		time.sleep( 0.1 )
		s_hp.write( b'MKPK;\n')
		time.sleep( 0.1 )
		s_hp.write( b'MKA?;\n')
		time.sleep( 0.1 )
		Amp = s_hp.readline()
		s_hp.write( b'MKF?;\n')
		time.sleep( 0.1 )
		Freq = s_hp.readline()
		print( 'Frequency: ' + str( float(Freq[1:6]) ) +'Ghz Amplitude: ' + str( float(Amp[:4]) ) + 'dBm' )
		s_hp.close()
		if s_sg.isOpen():
			s_sg.close()
		
	#print( 'Device: ' + str( s_hp.readline() ) )
def signal_generator_test():
	if s_sg.isOpen():
		print( s_sg.name + ' is open' )

		s_sg.write( b'*IDN?\n')
		r = s_sg.readline()
		print( 'Device: ' + str( r ) )

		time.sleep( 0.1 )
		s_sg.write( b'SWE:MODE SCAN\n')
		time.sleep( 0.1 )
		s_sg.write( b'FREQ:START 2500MHZ\n')
		time.sleep( 0.1 )
		s_sg.write( b'FREQ:STOP 9000MHZ\n')
		time.sleep( 0.1 )
		s_sg.write( b'SWE:POINTS 5\n')
		time.sleep( 0.1 )
		s_sg.write( b'SWE:DWELL 1000\n')
		time.sleep( 0.1 )
		s_sg.write( b'LIST:DIR DOWN\n')
		time.sleep( 0.1 )
		s_sg.write( b'INIT:CONT 0\n')
		time.sleep( 0.1 )
		s_sg.write( b'TRIG:SWEEP\n')
		time.sleep( 0.1 )
		s_sg.write( b'INIT:IMM\n')

		Is_sweeping = True
		while Is_sweeping:
			time.sleep( 0.1 )
			s_sg.write( b'SWE:ACTIVE?\n' )
			r = s_sg.readline()
			#print( 'Device: ' + str( r ) )
			if r == b'0\r\n':
				Is_sweeping = False
				s_sg.write( b'FREQ:CW 3000 MHZ\n')
				s_sg.write( b'POWER 2\n')
				s_sg.write( b'OUTP:STAT ON\n')
				s_sg.close()
				if s_hp.isOpen():
					s_hp.close()
				#print( 'Off' )

Hp_result_array = []
Cat_size = 0
def hp_get_data( Curr_dBm, Curr_Mhz, Start_Mhz, Stop_Mhz ):
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

def hp_get_data_manual( Curr_dBm, Curr_Mhz, Start_Mhz, Stop_Mhz, Compensate_dBm ):
	if s_hp.isOpen():
		Command = 'CF ' + str( Curr_Mhz ) + 'MHZ;\n'
		s_hp.write( Command.encode( 'UTF-8' ) )
		time.sleep( 0.3 )
		s_hp.write( b'MKPK;\n')
		time.sleep( 0.3 )
		s_hp.write( b'MKA?;\n')
		time.sleep( 0.1 )
		Amp = s_hp.readline().decode( 'UTF-8' )
		s_hp.write( b'MKF?;\n')
		time.sleep( 0.1 )
		Freq = s_hp.readline().decode( 'UTF-8' )
		Freq_float = float( Freq[1:-1] ) / 1000000.0
		Amp_float = float( Amp[1:-1] )
		Amp_float_compensated = Amp_float + Compensate_dBm
		Result_str = str( Curr_dBm ) + ', ' + Freq[1:-1] + ', ' + Amp[1:-1] + '\n'# + ', ' + str( Amp_float_compensated ) +'\n'
		f_w.write( Result_str )
		f_w.flush()
		print( Result_str )
		if Amp_float >= -50.0 and Amp_float <= 10.0 and Freq_float >= Start_Mhz and Freq_float <= Stop_Mhz and Amp_float_compensated <= 10 and Amp_float_compensated >= -30:
			#Result = [ Amp_float_compensated, Freq_float, Curr_dBm ]
			Result = [ Amp_float, Freq_float, Curr_dBm ]
			Hp_result_array.append( Result )
			global Cat_size
			Cat_size += 1

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
	
	#for i in range ( 1, len( Size ) ):
		#data = Reference(ws, min_col=1, min_row=Size[ i - 1 ] + 1, max_col=2, max_row= Size[ i - 1 ] + Size[ i ] )
		#c1.add_data(data, titles_from_data=False)

	#data = Reference(ws, min_col=1, min_row= 1, max_col=2, max_row= len(Hp_result_array) )
	#c1.add_data(data, titles_from_data=False)

	#print ( len(Size) )
	#print( Size )
	Pre_stop = 0
	for i in range ( len( Size ) ):
		Start_row = Pre_stop + 1
		Stop_row = Start_row + Size[ i ] - 1
		#print( 'Start from: ' + str( Start_row ) )
		#print( 'Stop from: ' + str( Stop_row ) )
		data_y = Reference(ws, min_col=1, min_row= Start_row, max_row= Stop_row )
		data_x = Reference(ws, min_col=2, min_row= Start_row, max_row= Stop_row )

		Series_name = str( Hp_result_array[ Start_row ] [ 2 ] ) + 'dBm input'
		s = Series( data_y, xvalues = data_x, title = Series_name )
		#print( Series_name )
		c1.append( s )

		Pre_stop = Stop_row
	
	ws.add_chart(c1, "A10")
	wb.save("Direct_2cable_test_2to4Ghz_-20to0dBm_A.xlsx")

def hp_display_config():
	if s_hp.isOpen():
		s_hp.write( b'FS;\n')
		time.sleep( 0.1 )
		s_hp.write( b'SP 500MHZ;\n')
		time.sleep( 0.1 )


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

Min_dBm = -21.5
Max_dBm = 10.0
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

def sg_sweep_manual( Start_dBm, Stop_dBm, Step_dBm, Start_Mhz, Stop_Mhz, Points ):

	if s_sg.isOpen():
		if Start_dBm < Min_dBm:
			Start_dBm = Min_dBm
		if Stop_dBm > Max_dBm:
			Stop_dBm = Max_dBm

	Step_Mhz = float( Stop_Mhz - Start_Mhz ) / float( Points )
	Curr_dBm = Start_dBm
	Line = 0
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
				s_sg.write( Command.encode( 'UTF-8' ) )
				time.sleep( 0.1 )
				s_sg.write( b'OUTP:STAT ON\n')
				time.sleep( 0.2 )

				Compensate_dBm = Lines[ Line ].split()[3]
				Line = Line + 1
				#print( Compensate_dBm )
				time.sleep( 0.2 )
				hp_get_data_manual( Curr_dBm, Curr_Mhz, Start_Mhz, Stop_Mhz, float( Compensate_dBm ) )
				Curr_Mhz = Curr_Mhz + Step_Mhz
			Size.append( Cat_size )
		Curr_dBm = Curr_dBm + Step_dBm
		#print( Cat_size )

def main():
	
	hp_display_config()
	#sg_sweep_config( 1000, 7000, 10, 500 )
	print ('Running...')
	
	#sg_sweep_amp_freq( -10, 10, 2, 2000, 9000, 70, 2000 )
	#sg_sweep_manual( Start_dBm, Stop_dBm, Step_dBm, Start_Mhz, Stop_Mhz, Points, Dwell_ms ):
	#sg_sweep_manual( -10, 10, 2, 2000, 9000, 70 )
	#sg_sweep_manual( -10, 10, 2, 2000, 4000, 40 )
	sg_sweep_manual( -20, 0, 2, 2000, 4000, 20 )
	draw_chart_excel()
	s_hp.close()
	s_sg.close()
	f_r.close()
	f_w.close()
	'''
	for i in range( 0, 100 ):
		print( Lines[i].split()[3] )
	'''
	print ('Complete!')

main()