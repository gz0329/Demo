from openpyxl import Workbook
from openpyxl.chart import (
    ScatterChart,
    Reference,
    Series,
)

f = open( "Test_mag_phase_726.txt", "r")

wb = Workbook()
ws = wb.active

name = ['Time', 'Phase_4', 'Phase_6', 'Mag_4', 'Mag_6']
phase_4 = []
phase_6 = []
mag_4 = []
mag_6 = []

line_count = 0

for lines in f:
    line_count = line_count + 1
    if ( line_count < 1000000 ):
        strings = lines.split()
        index_count = 0
        for words in strings:
            if ( "Phase_4:" in words ):
                phase_4.append( strings[ index_count + 1 ] )
            elif ( "Phase_6:" in words ):
                phase_6.append( strings[ index_count + 1 ] )
            elif ( "Mag_4:" in words ):
                mag_4.append( strings[ index_count + 1 ] )
            elif ( "Mag_6:" in words ):
                mag_6.append( strings[ index_count + 1 ] )
            else:
                pass

            index_count = index_count + 1
    else:
        pass

#print ( len(name), len( phase_4 ) + 1)
col_size = len( name )
row_size = len( phase_4 ) + 1

data = [ [ None for x in range( col_size ) ] for y in range( row_size ) ]

#print ( str( data ) )

for i in range ( col_size ):
    data[ 0 ][ i ] = name[ i ]

for j in range ( 1, row_size ):
    data[ j ][ 0 ] = int( j )
    data[ j ][ 1 ] = int( phase_4[ j - 1 ] )
    data[ j ][ 2 ] = int( phase_6[ j - 1 ] )
    data[ j ][ 3 ] = float( mag_4[ j - 1 ] )
    data[ j ][ 4 ] = float( mag_6[ j - 1 ] )

#print ( str( data ) )

#print( str(phase_4) + "\n" )
#print( str(phase_6) + "\n" )
#print( str(mag_4) + "\n" )
#print( str(mag_6) + "\n" )


for row in data:
    ws.append(row)

chart = ScatterChart()
chart.title = "Phase"
chart.style = 13
chart.x_axis.title = 'Degree'
chart.y_axis.title = 'Time'

xvalues = Reference(ws, min_col=1, min_row=2, max_row=row_size)
for i in range(2, 4):
    values = Reference(ws, min_col=i, min_row=1, max_row=row_size)
    series = Series(values, xvalues, title_from_data=True)
    chart.series.append(series)

ws.add_chart(chart, "H1")

chart2 = ScatterChart()
chart2.title = "Magnitude"
chart2.style = 13 
chart2.x_axis.title = 'Value'
chart2.y_axis.title = 'Time'

xvalues = Reference(ws, min_col=1, min_row=2, max_row=row_size)
for i in range(4, 6):
    values = Reference(ws, min_col=i, min_row=1, max_row=row_size)
    series = Series(values, xvalues, title_from_data=True)
    chart2.series.append(series)

ws.add_chart(chart2, "H20")

wb.save("Mag_phase_test.xlsx")
