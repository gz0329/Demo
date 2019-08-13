import math
import pcbnew
Board = pcbnew.GetBoard()

#execfile("TX_parts_placement.py")

Parts_per_channel = 10
Channels = 4

Part_ids = [ [ 0 for Part_idx in range( Parts_per_channel ) ] for Chan_idx in range( Channels ) ]

Part_ids[0] = [ "R201", "R213", "R205", "U209", "U213", "U217", "C203", "C212", "U224", "CON_Tx_0" ]
Part_ids[1] = [ "R202", "R214", "R206", "U210", "U214", "U218", "C204", "C213", "U225", "CON_Tx_1" ]
Part_ids[2] = [ "R203", "R215", "R207", "U211", "U215", "U219", "C205", "C214", "U226", "CON_Tx_2" ]
Part_ids[3] = [ "R204", "R216", "R208", "U212", "U216", "U220", "C206", "C215", "U227", "CON_Tx_3" ]

Channel_offset_mm = [ 0, 21, 21 + 28, 21 + 28 + 21 ]
Channel_offset_nm = [ 0 for Chan_idx in range( Channels ) ]
# Units are in nano-meters for some reason.
for Channel in range( Channels ):
	Channel_offset_nm[ Channel ] = Channel_offset_mm[ Channel ] * 1000000

def print_bounding_box( Box ):
	# Surprise! GetTop() is not properly implemented, so it gets an error.
	print "Lft={}, Rgt={}, Top={}, Bot={}".format( #Box.GetTop(),
		Box.GetLeft() / 1000000.0,
			Box.GetRight() / 1000000.0,
				( Box.GetBottom() - Box.GetHeight() ) / 1000000.0,
					Box.GetBottom() / 1000000.0 )
def main():
	# Validate the data.
	if ( len( Part_ids ) != Channels ):
		print( "Bad data (1)" )

	for Channel in range( 0, Channels ):
		if ( len( Part_ids[ Channel ] ) != Parts_per_channel ):
			print( "Bad data (2)" )

	if ( len( Channel_offset_nm ) != Channels ):
		print( "Bad data (3)" )

	Reference_channel = 0

	Bounding_box_enlarge_nm = 1000000
	# Start with any valid bounding box.
	Bounding_box = Board.FindModuleByReference( Part_ids[ Reference_channel ][ 0 ] ).GetBoundingBox()
	# Then find the bounding box for the reference channel.
	for Col in range( Parts_per_channel ):
		Ref_module = Board.FindModuleByReference( Part_ids[ Reference_channel ][ Col ] )
		Ref_module_box = Ref_module.GetBoundingBox()
		Bounding_box.Merge( Ref_module_box )
	for Ref in Part_ids[0]:
		Ref_module = Board.FindModuleByReference( Ref )
		Ref_module_box = Ref_module.GetBoundingBox()
		Bounding_box.Merge( Ref_module_box )
	print "Reference channel has a bounding box of (mm):"
	print_bounding_box( Bounding_box )
	# Enlarge the bounding box to pick up more traces.
	Bounding_box.Merge( pcbnew.EDA_RECT( 
		pcbnew.wxPoint( Bounding_box.GetLeft() - Bounding_box_enlarge_nm,
			Bounding_box.GetBottom() + Bounding_box_enlarge_nm ),
				pcbnew.wxSize( Bounding_box.GetWidth() + ( Bounding_box_enlarge_nm * 2 ),
					- ( Bounding_box.GetHeight() + ( Bounding_box_enlarge_nm * 2 ) ) ) ) )
	print "Enlarging to a bounding box of (mm):"
	print_bounding_box( Bounding_box )

	# Loop through the target channels and move parts and traces into place.
	Target_channel = 1
	#for Channel in range( Target_channel, Target_channel + 1 ):
	Channel = Target_channel
	for Channel in range( Channels ):
		if ( Channel != Reference_channel ):
			First_part = 1
			for Col in range( Parts_per_channel ):
				# Move each part into its relative position in this channel.
				Ref_module = Board.FindModuleByReference( Part_ids[ Reference_channel ][ Col ] )
				X_pos_nm, Y_pos_nm = Ref_module.GetPosition()
				Degree = Ref_module.GetOrientationDegrees()

				Ref_des = Ref_module.Reference()
				Ref_des_X_pos_nm, Ref_des_Y_pos_nm = Ref_des.GetPosition()
				Degree_des = Ref_des.GetDrawRotation()

				Ref_val = Ref_module.Value()
				Ref_val_X_pos_nm, Ref_val_Y_pos_nm = Ref_val.GetPosition()
				Degree_val = Ref_val.GetDrawRotation()

				#print( "Val: " + str(Degree_val) + " Des: " + str(Degree_des) )

				Tgt_module = Board.FindModuleByReference( Part_ids[ Channel ][ Col ] )
				Tgt_val = Tgt_module.Value()
				Tgt_des = Tgt_module.Reference()

				Tgt_val.SetPosition( pcbnew.wxPoint( ( Ref_val_X_pos_nm + Channel_offset_nm[ Channel ] ), Ref_val_Y_pos_nm ) )
				#Tgt_val.SetTextAngle( Degree_val )

				Tgt_des.SetPosition( pcbnew.wxPoint( ( Ref_des_X_pos_nm + Channel_offset_nm[ Channel ] ), Ref_des_Y_pos_nm ) )
				#Tgt_des.SetTextAngle( Degree_des )

				Tgt_module.SetPosition( pcbnew.wxPoint( ( X_pos_nm + Channel_offset_nm[ Channel ] ), Y_pos_nm ) )
				Tgt_module.SetOrientationDegrees( Degree )

				# Track the overall bounding box for this channel.
				if ( First_part == 1 ):
					First_part = 0
					Channel_bounding_box = Tgt_module.GetBoundingBox()
				else:
					Channel_bounding_box.Merge( Tgt_module.GetBoundingBox() )
	      
			#print_bounding_box( Channel_bounding_box )
			Channel_bounding_box.Merge( pcbnew.EDA_RECT( 
				pcbnew.wxPoint( Channel_bounding_box.GetLeft() - Bounding_box_enlarge_nm,
					Channel_bounding_box.GetBottom() + Bounding_box_enlarge_nm ),
						pcbnew.wxSize( Channel_bounding_box.GetWidth() + ( Bounding_box_enlarge_nm * 2 ),
							- ( Channel_bounding_box.GetHeight() + ( Bounding_box_enlarge_nm * 2 ) ) ) ) )
	    #print_bounding_box( Channel_bounding_box )
	    
	    #print "module {} is at {}".format( Ref_module.GetReference(), Ref_module.GetPosition() )
	    #for Pad in Ref_module.Pads():
	    #  print "Pad {} at {} -> {}".format( Pad.GetPadName(), Pad.GetCenter(), Pad.GetNet().GetNetname() )
	      
	    # If any zones are inside the bounding box, duplicate them into the target area.
	    #print 'Iterating through all the pads for each cloned zone, might take a few seconds...'
	    # For all the zones in the board...

			for Area_idx in range( 0, Board.GetAreaCount() ):
				Zone = Board.GetArea( Area_idx )

				# If the zone is inside the area to be cloned (the reference circuit) and it
				# is not the comment zone (layer 41)
				# Also constrain it to the outside (F.Cu and B.Cu) layers.
				if ( Bounding_box.Contains( Zone.GetPosition() ) and Zone.GetLayer() is not 41
					and ( ( Zone.GetLayerName() == "F.Cu" ) or ( Zone.GetLayerName() == "B.Cu" ) ) ):
						# Make copy of the zone and move it into position.
						Zone_clone = Zone.Duplicate()
						Zone_clone.Move( pcbnew.wxPoint( Channel_offset_nm[ Channel ], 0 ) )
						print "Duplicated layer {}, zone {} to channel {}".format( Zone_clone.GetLayerName(),
							Zone_clone.GetNetname(), Channel )

						# Iterate through all the pads in the board, finding the ones we just cloned.
						for Module in Board.GetModules():
							for Pad in Module.Pads():
								# Find the (last) one inside the cloned zone. pad.GetZoneConnection() could also be used
								if ( Zone_clone.HitTestInsideZone( Pad.GetPosition() )
									and Pad.IsOnLayer( Zone_clone.GetLayer() ) ):
										# Set the (maybe) correct net for the zone.
										Zone_clone.SetNetCode( Pad.GetNet().GetNet() )
										Board.Add( Zone_clone )

			# If any traces are inside the bounding box, duplicate the traces for that net
			# into the target area, for the target's nets.
			Tracks = Board.GetTracks()
			Clone_tracks = []
			for Track in Tracks:
				if Track.HitTest( Bounding_box ):
					# Copy and move the track into place.
					Clone_track = Track.Duplicate()
					Clone_track.Move( pcbnew.wxPoint( Channel_offset_nm[ Channel ], 0 ) )
					Clone_tracks.append( Clone_track )
			# Append the temporary list to the board.
			for Track in Clone_tracks:
				Tracks.Append( Track )

	  # Need a 2017-03-20+ build to use Refresh()
		pcbnew.Refresh()

main()