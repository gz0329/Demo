# Execute by opening the KiCad scripting tool and entering:
# execfile( "copy_clean.py" )

# George Zhang 06/23/2017
# Clean all the overlapped traces

#execfile("copy_clean.py")
import pcbnew

Board = pcbnew.GetBoard()

Tracks = Board.GetTracks()

# Keep track of the number of tracks and copied ones
Track_num = 0
Copied_num = 0

# Loop through the tracklist
for Track in Tracks:
	x1_s, y1_s = Track.GetStart()
	x1_e, y1_e = Track.GetEnd()
	Track_num += 1

	# Flag to not pass the end of the list
	if Track.Next() == None:
		break
	Track_n = Track.Next()

	# Loop through the rest items in the list
	while 1:
		x2_s, y2_s = Track_n.GetStart()
		x2_e, y2_e = Track_n.GetEnd()

		if x1_s == x2_s and x1_e == x2_e and y1_s == y2_s and y1_e == y2_e:
			Copied_num += 1
			# Move that trace to the bottom left
			Track_n.SetPosition( pcbnew.wxPoint( 25, 360000000 ) )

		# Another flag to make sure not to pass the end of the list
		if Track_n.Next() == None:
			break
		Track_n = Track_n.Next()

print Track_num
print Copied_num

pcbnew.Refresh()

