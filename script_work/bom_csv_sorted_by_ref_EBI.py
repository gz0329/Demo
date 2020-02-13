#
# Example python script to generate a BOM from a KiCad generic netlist
#
# Example: Ungrouped (One component per row) CSV output
#

"""
    @package
    Generate a csv list file.
    Components are sorted by ref
    One component per line
    Fields are (if exist)
    Ref, value, Part, footprint, Datasheet, Manufacturer, Vendor

    Command line:
    python "pathToFile/bom_csv_sorted_by_ref.py" "%I" "%O.csv"
"""

from __future__ import print_function

# Import the KiCad python helper module
import csv
import sys
sys.path.append("E:\Program Files\KiCad\\bin\scripting\plugins")
import kicad_netlist_reader

from datetime import date
# Generate an instance of a generic netlist, and load the netlist tree from
# the command line option. If the file doesn't exist, execution will stop
net = kicad_netlist_reader.netlist(sys.argv[1])

# Open a file to write to, if the file cannot be opened output to stdout
# instead
try:
    f = open(sys.argv[2], 'w')
except IOError:
    e = "Can't open output file for writing: " + sys.argv[2]
    print( __file__, ":", e, sys.stderr )
    f = sys.stdout

# Create a new csv writer object to use as the output formatter
out = csv.writer(f, lineterminator='\n', delimiter=',', quotechar="\"", quoting=csv.QUOTE_ALL)

# override csv.writer's writerow() to support utf8 encoding:
def writerow( acsvwriter, columns ):
    utf8row = []
    for col in columns:
        utf8row.append( str(col) )
    acsvwriter.writerow( utf8row )

components = net.getInterestingComponents()

# Output a field delimited header line
writerow( out, ['Source:', net.getSource()] )
#writerow( out, ['Date:', net.getDate()] )
writerow( out, ['Date:', date.today()] )
writerow( out, ['Tool:', net.getTool()] )
writerow( out, ['Component Count:', len(components)] )
# Original version
#writerow( out, ['Ref', 'Value', 'Footprint', 'Datasheet', 'Manufacturer', 'Vendor'] )
# EBI version with URL, price, etc.
writerow( out, [ 'Ref', 'Quantity', 'Value', 'URL', 'Footprint', 'EBI Short Description', 'Distributor', 
    'Distributor Part Number', 'Manufacturer', 'Manufacturer Part Number', 'Description', 'Price at 1', 
        'Price at 100', 'Price / board', 'Price / 100 board'] )

'''
# Output all of the component information (One component per row)
for c in components:
    # Original version
    writerow( out, [c.getRef(), c.getValue(), c.getFootprint(), c.getDatasheet(),
        c.getField("Manufacturer"), c.getField("Vendor")])

    writerow( out, [c.getRef(), c.getValue(), c.getFootprint(), c.getField("EBI Short Description"), c.getField("Distributor"), 
        c.getField("Distributor Part Number"), c.getField("Manufacturer"), c.getField("Manufacturer Part Number"), c.getField("Description"),
            c.getField("Price at 1"), c.getField("Price at 100")])
'''

grouped = net.groupComponents()

# Output all of the component information
for group in grouped:
    refs = ""

    # Add the reference of every component in the group and keep a reference
    # to the component so that the other data can be filled in once per group
    for component in group:
        refs += component.getRef() + ", "
        c = component

    Price_1 = c.getField("Price at 1")
    Price_100 = c.getField("Price at 100")
    print( Price_1 )
    print( Price_100 )

    if Price_1 == "":
        Price_1_double = 0
    else:
        Price_1_double = float( Price_1 )

    if Price_100 == "":
        Price_100_double = 0
    else:
        Price_100_double = float( Price_100 )
    '''
    Price_1_double = float( Price_1 )
    Price_100_double = float( Price_100 )
    print( Price_1 )
    print( Price_100 )
    '''

    # Fill in the component groups common data
    out.writerow([refs, len(group), c.getValue(), c.getField("URL"), c.getFootprint(), c.getField("EBI Short Description"), c.getField("Distributor"), 
        c.getField("Distributor Part Number"), c.getField("Manufacturer"), c.getField("Manufacturer Part Number"), c.getField("Description"),
            Price_1, Price_100, len(group) * Price_1_double , len(group) * 100 * Price_100_double ] )
