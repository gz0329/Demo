import math
import pcbnew
board = pcbnew.GetBoard()

#execfile("PCB_parts_placement.py")

Parts_per_channel = 6
Channels = 22

Part_ids_rx = [ [ 0 for Part_idx in range( Parts_per_channel ) ] for Chan_idx in range( Channels ) ]
Part_ids_tx = [ [ 0 for Part_idx in range( Parts_per_channel ) ] for Chan_idx in range( Channels ) ]

#U201
#left
Part_ids_rx[0] = [ "CON_Rx_0", "U208", "C282", "C286", "R219", "R223"]
Part_ids_rx[1] = [ "CON_Rx_1", "U212", "C290", "C294 ", "R227", "R231"]
Part_ids_rx[2] = [ "CON_Rx_2", "U216", "C298", "C302", "R235", "R239"]
Part_ids_rx[3] = [ "CON_Rx_3", "U220", "C306", "C310", "R243", "R247"]
#top
Part_ids_rx[4] = [ "CON_Rx_4", "U224", "C314", "C318", "R251", "R255"]
Part_ids_rx[5] = [ "CON_Rx_5", "U228", "C322", "C326", "R259", "R263"]
Part_ids_rx[6] = [ "CON_Rx_6", "U232", "C330", "C334", "R267", "R271"]
Part_ids_rx[7] = [ "CON_Rx_7", "U236", "C338", "C342", "R275", "R279"]
#right
Part_ids_rx[8] = [ "CON_Rx_8", "U240", "C346", "C350", "R283", "R287"]
Part_ids_rx[9] = [ "CON_Rx_9", "U244", "C354", "C358", "R291", "R295"]
Part_ids_rx[10] = [ "CON_Rx_10", "U248", "C362", "C366", "R299", "R303"]
#U206
#top
Part_ids_rx[11] = [ "CON_Rx_11", "U209", "C283", "C287", "R220", "R224"]
Part_ids_rx[12] = [ "CON_Rx_12", "U213", "C291", "C295", "R228", "R232"]
Part_ids_rx[13] = [ "CON_Rx_13", "U217", "C299", "C303", "R236", "R240"]
Part_ids_rx[14] = [ "CON_Rx_14", "U221", "C307", "C311", "R244", "R248"]
#right
Part_ids_rx[15] = [ "CON_Rx_15", "U225", "C315", "C319", "R252", "R256"]
Part_ids_rx[16] = [ "CON_Rx_16", "U229", "C323", "C327", "R260", "R264"]
Part_ids_rx[17] = [ "CON_Rx_17", "U233", "C331", "C335", "R268", "R272"]
Part_ids_rx[18] = [ "CON_Rx_18", "U237", "C339", "C343", "R276", "R280"]
#bottom
Part_ids_rx[19] = [ "CON_Rx_19", "U241", "C347", "C351", "R284", "R288"]
Part_ids_rx[20] = [ "CON_Rx_20", "U245", "C355", "C359", "R292", "R296"]
Part_ids_rx[21] = [ "CON_Rx_21", "U249", "C363", "C367", "R300", "R304"]
#U204
Part_ids_tx[0] = [ "CON_Tx_0", "U210", "C284", "C288", "R221", "R225"]
Part_ids_tx[1] = [ "CON_Tx_1", "U214", "C292", "C296", "R229", "R233"]
Part_ids_tx[2] = [ "CON_Tx_2", "U218", "C300", "C304", "R237", "R241"]
Part_ids_tx[3] = [ "CON_Tx_3", "U222", "C308", "C312", "R245", "R249"]
Part_ids_tx[4] = [ "CON_Tx_4", "U226", "C316", "C320", "R253", "R257"]
Part_ids_tx[5] = [ "CON_Tx_5", "U230", "C324", "C328", "R261", "R265"]
Part_ids_tx[6] = [ "CON_Tx_6", "U234", "C332", "C336", "R269", "R273"]
Part_ids_tx[7] = [ "CON_Tx_7", "U238", "C340", "C344", "R277", "R281"]
Part_ids_tx[8] = [ "CON_Tx_8", "U242", "C348", "C352", "R285", "R289"]
Part_ids_tx[9] = [ "CON_Tx_9", "U246", "C356", "C360", "R293", "R297"]
Part_ids_tx[10] = [ "CON_Tx_10", "U250", "C364", "C368", "R301", "R305"]
#U207
Part_ids_tx[11] = [ "CON_Tx_11", "U211", "C285", "C289", "R222", "R226"]
Part_ids_tx[12] = [ "CON_Tx_12", "U215", "C293", "C297", "R230", "R234"]
Part_ids_tx[13] = [ "CON_Tx_13", "U219", "C301", "C305", "R238", "R242"]
Part_ids_tx[14] = [ "CON_Tx_14", "U223", "C309", "C313", "R246", "R250"]
Part_ids_tx[15] = [ "CON_Tx_15", "U227", "C317", "C321", "R254", "R258"]
Part_ids_tx[16] = [ "CON_Tx_16", "U231", "C325", "C329", "R262", "R266"]
Part_ids_tx[17] = [ "CON_Tx_17", "U235", "C333", "C337", "R270", "R274"]
Part_ids_tx[18] = [ "CON_Tx_18", "U239", "C341", "C345", "R278", "R282"]
Part_ids_tx[19] = [ "CON_Tx_19", "U243", "C349", "C353", "R286", "R290"]
Part_ids_tx[20] = [ "CON_Tx_20", "U247", "C357", "C361", "R294", "R298"]
Part_ids_tx[21] = [ "CON_Tx_21", "U251", "C365", "C369", "R302", "R306"]

rx_0_10_x = 170.0
rx_0_10_y = 95.0

rx_11_21_x = 188.4
rx_11_21_y = 98.0

for i in range( 4 ):
	mod_ref = board.FindModuleByReference( Part_ids_rx[i][0] )
	X_pos_nm, Y_pos_nm = mod_ref.GetPosition()
	mod_ref.SetOrientationDegrees( 0 )
	mod = board.FindModuleByReference( Part_ids_rx[i][1] )
	mod.SetPosition(pcbnew.wxPoint( X_pos_nm + 6.4  * 1000000, Y_pos_nm - 1 * 1000000))
	mod.SetOrientationDegrees( 180 )
	mod = board.FindModuleByReference( Part_ids_rx[i][2] )
	mod.SetPosition(pcbnew.wxPoint( X_pos_nm + 9.9  * 1000000, Y_pos_nm + 0.2 * 1000000))
	mod.SetOrientationDegrees( 0 )
	mod = board.FindModuleByReference( Part_ids_rx[i][3] )
	mod.SetPosition(pcbnew.wxPoint( X_pos_nm + 9.9  * 1000000, Y_pos_nm - 2.2 * 1000000))
	mod.SetOrientationDegrees( 0 )
	'''
	mod = board.FindModuleByReference( Part_ids_rx[i][4] )
	mod.SetPosition(pcbnew.wxPoint( X_pos_nm + 10.2  * 1000000, Y_pos_nm + 1.5 * 1000000))
	mod.SetOrientationDegrees( 90 )
	mod = board.FindModuleByReference( Part_ids_rx[i][5] )
	mod.SetPosition(pcbnew.wxPoint( X_pos_nm + 10.2  * 1000000, Y_pos_nm - 3.5 * 1000000))
	mod.SetOrientationDegrees( 270 )
	'''

sqrt_2 = math.sqrt(2)

for i in range( 4 , 8 ):
	mod_ref = board.FindModuleByReference( Part_ids_rx[i][0] )
	X_pos_nm, Y_pos_nm = mod_ref.GetPosition()
	mod_ref.SetOrientationDegrees( 45 )
	mod = board.FindModuleByReference( Part_ids_rx[i][1] )
	mod.SetPosition(pcbnew.wxPoint( X_pos_nm + 7.4 / sqrt_2 * 1000000, Y_pos_nm + 5.4 / sqrt_2 * 1000000))
	mod.SetOrientationDegrees( 135 )
	mod = board.FindModuleByReference( Part_ids_rx[i][2] )
	mod.SetPosition(pcbnew.wxPoint( X_pos_nm + 9.7 / sqrt_2  * 1000000, Y_pos_nm + 10.1 / sqrt_2 * 1000000))
	mod.SetOrientationDegrees( 315 )
	mod = board.FindModuleByReference( Part_ids_rx[i][3] )
	mod.SetPosition(pcbnew.wxPoint( X_pos_nm + 12.1 / sqrt_2  * 1000000, Y_pos_nm + 7.7 / sqrt_2 * 1000000))
	mod.SetOrientationDegrees( 315 )
	'''
	mod = board.FindModuleByReference( Part_ids_rx[i][4] )
	mod.SetPosition(pcbnew.wxPoint( X_pos_nm + 8.7 / sqrt_2  * 1000000, Y_pos_nm + 11.7 / sqrt_2 * 1000000))
	mod.SetOrientationDegrees( 45 )
	mod = board.FindModuleByReference( Part_ids_rx[i][5] )
	mod.SetPosition(pcbnew.wxPoint( X_pos_nm + 13.7 / sqrt_2  * 1000000, Y_pos_nm + 6.7 / sqrt_2 * 1000000))
	mod.SetOrientationDegrees( 225 )
	'''

for i in range( 8, 15 ):
	mod_ref = board.FindModuleByReference( Part_ids_rx[i][0] )
	X_pos_nm, Y_pos_nm = mod_ref.GetPosition()
	mod_ref.SetOrientationDegrees( 90 )
	mod = board.FindModuleByReference( Part_ids_rx[i][1] )
	mod.SetPosition(pcbnew.wxPoint( X_pos_nm + 1  * 1000000, Y_pos_nm + 6.4 * 1000000))
	mod.SetOrientationDegrees( 90 )
	mod = board.FindModuleByReference( Part_ids_rx[i][2] )
	mod.SetPosition(pcbnew.wxPoint( X_pos_nm - 0.2  * 1000000, Y_pos_nm + 9.9 * 1000000))
	mod.SetOrientationDegrees( 270 )
	mod = board.FindModuleByReference( Part_ids_rx[i][3] )
	mod.SetPosition(pcbnew.wxPoint( X_pos_nm + 2.2  * 1000000, Y_pos_nm + 9.9 * 1000000))
	mod.SetOrientationDegrees( 270 )
	'''
	mod = board.FindModuleByReference( Part_ids_rx[i][4] )
	mod.SetPosition(pcbnew.wxPoint( X_pos_nm - 1.5  * 1000000, Y_pos_nm + 10.2 * 1000000))
	mod.SetOrientationDegrees( 0 )
	mod = board.FindModuleByReference( Part_ids_rx[i][5] )
	mod.SetPosition(pcbnew.wxPoint( X_pos_nm + 3.5  * 1000000, Y_pos_nm + 10.2 * 1000000))
	mod.SetOrientationDegrees( 180 )
	'''

for i in range( 15, 19 ):
	mod_ref = board.FindModuleByReference( Part_ids_rx[i][0] )
	X_pos_nm, Y_pos_nm = mod_ref.GetPosition()
	mod_ref.SetOrientationDegrees( 45 )
	mod = board.FindModuleByReference( Part_ids_rx[i][1] )
	mod.SetPosition(pcbnew.wxPoint( X_pos_nm - 5.4 / sqrt_2 * 1000000, Y_pos_nm + 7.4 / sqrt_2 * 1000000))
	mod.SetOrientationDegrees( 45 )
	mod = board.FindModuleByReference( Part_ids_rx[i][2] )
	mod.SetPosition(pcbnew.wxPoint( X_pos_nm - 10.1 / sqrt_2  * 1000000, Y_pos_nm + 9.7 / sqrt_2 * 1000000))
	mod.SetOrientationDegrees( 225 )
	mod = board.FindModuleByReference( Part_ids_rx[i][3] )
	mod.SetPosition(pcbnew.wxPoint( X_pos_nm - 7.7 / sqrt_2  * 1000000, Y_pos_nm + 12.1 / sqrt_2 * 1000000))
	mod.SetOrientationDegrees( 225 )
	'''
	mod = board.FindModuleByReference( Part_ids_rx[i][4] )
	mod.SetPosition(pcbnew.wxPoint( X_pos_nm - 11.7 / sqrt_2  * 1000000, Y_pos_nm + 8.7 / sqrt_2 * 1000000))
	mod.SetOrientationDegrees( -45 )
	mod = board.FindModuleByReference( Part_ids_rx[i][5] )
	mod.SetPosition(pcbnew.wxPoint( X_pos_nm - 6.7 / sqrt_2  * 1000000, Y_pos_nm + 13.7 / sqrt_2 * 1000000))
	mod.SetOrientationDegrees( 135 )
	'''

for i in range( 19, 22 ):
	mod_ref = board.FindModuleByReference( Part_ids_rx[i][0] )
	X_pos_nm, Y_pos_nm = mod_ref.GetPosition()
	mod_ref.SetOrientationDegrees( 0 )
	mod = board.FindModuleByReference( Part_ids_rx[i][1] )
	mod.SetPosition(pcbnew.wxPoint( X_pos_nm - 6.4  * 1000000, Y_pos_nm + 1 * 1000000))
	mod.SetOrientationDegrees( 0 )
	mod = board.FindModuleByReference( Part_ids_rx[i][2] )
	mod.SetPosition(pcbnew.wxPoint( X_pos_nm - 9.9  * 1000000, Y_pos_nm - 0.2 * 1000000))
	mod.SetOrientationDegrees( 180 )
	mod = board.FindModuleByReference( Part_ids_rx[i][3] )
	mod.SetPosition(pcbnew.wxPoint( X_pos_nm - 9.9  * 1000000, Y_pos_nm + 2.2 * 1000000))
	mod.SetOrientationDegrees( 180 )
	'''
	mod = board.FindModuleByReference( Part_ids_rx[i][4] )
	mod.SetPosition(pcbnew.wxPoint( X_pos_nm - 10.2  * 1000000, Y_pos_nm - 1.5 * 1000000))
	mod.SetOrientationDegrees( 270 )
	mod = board.FindModuleByReference( Part_ids_rx[i][5] )
	mod.SetPosition(pcbnew.wxPoint( X_pos_nm - 10.2  * 1000000, Y_pos_nm + 3.5 * 1000000))
	mod.SetOrientationDegrees( 90 )
	'''

x_center = 170 * 1000000
y_center = 110 * 1000000
for i in range( Channels ):
	for j in range( Parts_per_channel ):
		mod_ref = board.FindModuleByReference( Part_ids_rx[i][j] )
		X_pos_nm, Y_pos_nm = mod_ref.GetPosition()
		mod = board.FindModuleByReference( Part_ids_tx[i][j] )
		x = x_center - ( X_pos_nm - x_center )
		y = y_center - ( Y_pos_nm - y_center )
		mod.SetPosition(pcbnew.wxPoint(x, y))
		degree = mod_ref.GetOrientationDegrees() - 180
		mod.SetOrientationDegrees( degree )




'''
Rx_Balum = ["U208", "U212", "U216", "U220", "U224", "U228", "U232", "U236", "U240", "U244", "U248", "U209", "U213", "U217", "U221", "U225", "U229", "U233", "U237", "U241", "U245", "U249"]
Tx_Balum = ["U210", "U214", "U218", "U222", "U226", "U230", "U234", "U238", "U242", "U246", "U250", "U211", "U215", "U219", "U223", "U227", "U231", "U235", "U239", "U243", "U247", "U251"]

Rx_Sma = ["CON_Rx_0", "CON_Rx_1", "CON_Rx_2", "CON_Rx_3", "CON_Rx_4", "CON_Rx_5", "CON_Rx_6", "CON_Rx_7", "CON_Rx_8", "CON_Rx_9", "CON_Rx_10", "CON_Rx_11", "CON_Rx_12", "CON_Rx_13", "CON_Rx_14", "CON_Rx_15", "CON_Rx_16", "CON_Rx_17", "CON_Rx_18", "CON_Rx_19", "CON_Rx_20", "CON_Rx_21"]
Tx_Sma = ["CON_Tx_0", "CON_Tx_1", "CON_Tx_2", "CON_Tx_3", "CON_Tx_4", "CON_Tx_5", "CON_Tx_6", "CON_Tx_7", "CON_Tx_8", "CON_Tx_9", "CON_Tx_10", "CON_Tx_11", "CON_Tx_12", "CON_Tx_13", "CON_Tx_14", "CON_Tx_15", "CON_Tx_16", "CON_Tx_17", "CON_Tx_18", "CON_Tx_19", "CON_Tx_20", "CON_Tx_21"]

x_center = 170
y_center = 110

radius_bal = input("Radius for balum(mm)?")
radius_sma = input("Radius for sma(mm)?")
degree_start = input("Start from(degree)?")
degree_dif = input("Difference(degree)?")

for i in range( len( Rx_Balum ) ):
	degree = degree_start + i * degree_dif
	x_dif_bal = math.cos( degree / 180.0 * math.pi ) * radius_bal
	y_dif_bal = math.sin( degree / 180.0 * math.pi ) * radius_bal
	x_dif_sma = math.cos( degree / 180.0 * math.pi ) * radius_sma
	y_dif_sma = math.sin( degree / 180.0 * math.pi ) * radius_sma
	#print( "Degree: " + str(degree) + " x diff: " + str(x_dif) + " y diff: " + str(y_dif))
	print( "x_dif_bal: " + str(x_dif_bal) + " y_dif_bal: " + str(y_dif_bal) + " x_dif_sma: " + str(x_dif_sma)  + " y_dif_sma: " + str(y_dif_sma))
	x_Rx_Balum = x_center - x_dif_bal
	y_Rx_Balum = y_center - y_dif_bal

	x_Tx_Balum = x_center - x_dif_bal
	y_Tx_Balum = y_center + y_dif_bal

	x_Rx_Sma = x_center - x_dif_sma
	y_Rx_Sma = y_center - y_dif_sma

	x_Tx_Sma = x_center - x_dif_sma
	y_Tx_Sma = y_center + y_dif_sma

	mod = board.FindModuleByReference( Rx_Balum[i] )
	mod.SetPosition(pcbnew.wxPoint(x_Rx_Balum  * 1000000, y_Rx_Balum  * 1000000))
	mod.SetOrientationDegrees( 180 - degree )

	mod = board.FindModuleByReference( Tx_Balum[i] )
	mod.SetPosition(pcbnew.wxPoint(x_Tx_Balum  * 1000000, y_Tx_Balum  * 1000000))
	mod.SetOrientationDegrees( degree + 180 )

	mod = board.FindModuleByReference( Rx_Sma[i] )
	mod.SetPosition(pcbnew.wxPoint(x_Rx_Sma  * 1000000, y_Rx_Sma  * 1000000))
	mod.SetOrientationDegrees( 180 - degree )

	mod = board.FindModuleByReference( Tx_Sma[i] )
	mod.SetPosition(pcbnew.wxPoint(x_Tx_Sma * 1000000, y_Tx_Sma  * 1000000))
	mod.SetOrientationDegrees( degree + 180 )
'''