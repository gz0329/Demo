
'''
Copyright (c) 2018, Elliott Bay Industries. All rights reserved.

Purpose:
Read files and take average of the power levels.

History:
2019-07-08  1.00  George Zhang   Initial implementation.
'''

f1 = open("Direct_1cable_test_2to4Ghz_-10to10dBm.csv", "r")
f2 = open("Direct_1cable_test_2to4Ghz_tripled_-10to10dBm.csv", "r")
f3 = open("Direct_2cable_test_7to9Ghz_-20to0dBm_C.csv", "r")

f = open("Direct_2cable_test_2to4Ghz_-10to10dBm_combined.csv", "w")

Inject_dBm = []
Output_dBm = []
Freq_Mhz = []

Size = 0
for Lines in f1:
    Size = Size + 1
    Strings = Lines.split()
    Inject_dBm.append( Strings[0][:-1] )
    Output_dBm.append( Strings[2] )
    Freq_Mhz.append( Strings[1][:-1] )

for Lines in f2:
    Strings = Lines.split()
    Inject_dBm.append( Strings[0][:-1] )
    Output_dBm.append( Strings[2] )
    Freq_Mhz.append( Strings[1][:-1] )

for Lines in f3:
    Strings = Lines.split()
    Inject_dBm.append( Strings[0][:-1] )
    Output_dBm.append( Strings[2] )
    Freq_Mhz.append( Strings[1][:-1] )

for i in range( 0 , Size ):
	#print( Inject_dBm[i] + ' ' + Inject_dBm[ i + Size ] + ' ' + Inject_dBm[ i + 2 * Size] + '\n')
	'''
	Mean_inject_dBm = ( float( Inject_dBm[i] ) + float( Inject_dBm[ i + Size ] ) + float( Inject_dBm[ i + 2 * Size] ) ) / 3
	Mean_output_dBm = ( float( Output_dBm[i] ) + float( Output_dBm[ i + Size ] ) + float( Output_dBm[ i + 2 * Size] ) ) / 3
	Mean_freq_dBm = ( float( Freq_Mhz[i] ) + float( Freq_Mhz[ i + Size ] ) + float( Freq_Mhz[ i + 2 * Size] ) ) / 3
	Compensate_dBm = Mean_inject_dBm - Mean_output_dBm
	Result = str( Mean_inject_dBm ) + ', ' + str( Mean_freq_dBm ) + ', ' + str( Mean_output_dBm ) + ', ' + str( Compensate_dBm) + '\n'
	'''
	Low_compensate_dBm = ( float( Inject_dBm[i] ) ) - ( float( Output_dBm[i] ) )
	High_compensate_dBm = ( float( Inject_dBm[i + Size] ) ) - ( float( Output_dBm[i + Size] ) )
	Compensate_dBm = Low_compensate_dBm + High_compensate_dBm
	Mean_output_dBm = 0
	Result = Inject_dBm[i] + ', ' + Freq_Mhz[i] + ', ' + str( Mean_output_dBm ) + ', ' + str( Compensate_dBm ) + '\n'
		
	f.write( Result )
	f.flush()
	print( Result )
#print( Inject_dBm )
#print( Freq_Mhz )
#print( Output_dBm )