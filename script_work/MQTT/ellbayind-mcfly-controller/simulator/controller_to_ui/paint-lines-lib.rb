require 'pp'

Math::TAU = Math::PI*2

$debug_paint_lines = $debug

def angle_to_point_on_square(degrees)
  rads = ((degrees%45)/360.0) * Math::TAU

  #the adjacent side is always 1
  opposite_len = Math.tan(rads)
  case degrees
  when (315...360)
    #left edge, down
    return [-1, -1+opposite_len]
  when (0...45)
    #left edge, up
    return [-1, +opposite_len]

  when (45...90)
    #top edge, left
    return [-1+opposite_len, +1]
  when (90...135)
    #top edge, right
    return [+opposite_len, +1]

  when (135...180)
    #right edge, up
    return [+1, +1-opposite_len]
  when (180...225)
    #right edge, down
    return [+1, -opposite_len]

  when (225...270)
    #bottom edge, right
    return [+1-opposite_len, -1]
  when (270...315)
    #bottom edge, left
    return [-opposite_len, -1]
  end
end

def degree_range_covers?(from, to, test_deg)
  from %= 360     #= #these comments are a hack 
  to %= 360       #= #to fix broken syntax 
  test_deg %= 360 #= #highlighting in emacs
  if from > to
    return (from..360).include?(test_deg) || (0..to).include?(test_deg)
  else
    return (from..to).include?(test_deg)
  end
end

#assumes 0 <= degrees_{from,to} <= 360
def angle_range_to_path_along_square(degrees_from, degrees_to)
  points = []
  points << [0,0]
  points << angle_to_point_on_square(degrees_from)

  corners = []

  #top left
  corners << [45, [-1,+1]] if degree_range_covers?(degrees_from, degrees_to, 45)

  #top right
  corners << [135,[+1,+1]] if degree_range_covers?(degrees_from, degrees_to, 135)

  #bottom left
  corners << [225,[+1,-1]] if degree_range_covers?(degrees_from, degrees_to, 225)

  #bottom right
  corners << [315,[-1,-1]] if degree_range_covers?(degrees_from, degrees_to, 315)

  if not corners.empty?
    while corners.first[0] < degrees_from
      stuff = corners.shift
      corners << [stuff[0] + 360, stuff[1]]
    end
    corners.each do |angle, coord|
      points << coord
    end
  end

  points << angle_to_point_on_square(degrees_to)

  puts "mathmatical (topleft -,+)" if $debug_paint_lines
  pp points if $debug_paint_lines

  return points
end

def angle_range_to_image_path(degrees_from, degrees_to, image_size)
  orig_points = angle_range_to_path_along_square(degrees_from, degrees_to)
  puts "image size is #{image_size}" if $debug_paint_lines
  return orig_points.map! do |x, y|
    y = -y #mathmatical coordinates increase up, images increase down
    x = (x+1) * (image_size / 2.0)
    y = (y+1) * (image_size / 2.0)
    next [x, y]
  end
end

def paint_angle_range_on_image( img, line_size, color, angle_range_begin, angle_range_end, x = 0, y = 0 )
  angle_range_begin %= 360 #=
  angle_range_end   %= 360 #=
  puts "painting angle range from #{angle_range_begin} to #{angle_range_end}" if $debug_paint_lines
  points = angle_range_to_image_path(
    angle_range_begin,
    angle_range_end,
    line_size
  )

  pp points if $debug_paint_lines

  x_pos = x * line_size
  y_pos = y * line_size

  points.map! do |x,y|
    x += x_pos
    y += y_pos
    next [x.to_i, y.to_i]
  end

  puts "sending these points to ChunkyPNG::Canvas#polygon" if $debug_paint_lines
  pp points if $debug_paint_lines

  img.polygon( points, ChunkyPNG::Color::TRANSPARENT, color )
  return img
end

@memos = {}

def antialiased_angle_range_for( line_size, angle_range_begin, angle_range_end )
  key = [line_size, angle_range_begin, angle_range_end]
  if @memos.has_key? key
    return @memos[key]
  end
  antialias_amt = 8
  big_line = line_size * antialias_amt
  big_canvas = ChunkyPNG::Canvas.new( big_line, big_line )
  paint_angle_range_on_image( big_canvas, big_line, ChunkyPNG::Color::BLACK, angle_range_begin, angle_range_end )
  small_canvas = ChunkyPNG::Canvas.new( line_size, line_size )
  line_size.times do |small_y|
    line_size.times do |small_x|
      big_y = small_y * antialias_amt
      big_x = small_x * antialias_amt

      sum = 0
      antialias_amt.times do |sub_y|
        antialias_amt.times do |sub_x|
          if big_canvas[big_x + sub_x, big_y + sub_y] > 0
            sum += 1
          end
        end
      end
      #TODO: sRGB colors work on a roughly logarithmic, not linear scale
      teint = sum / (antialias_amt ** 2.0) * 255
      small_canvas[small_x, small_y] = ChunkyPNG::Color.grayscale_alpha( 0, teint.to_i )
    end
  end
  return @memos[key] = small_canvas
end

def paint_antialiased_angle_range_on_image( img, line_size, alpha_val, angle_range_begin, angle_range_end, x = 0, y = 0 )
  canvas = antialiased_angle_range_for( line_size, angle_range_begin, angle_range_end )
  if $debug_paint_lines
    canvas.to_image.save("canvas-debugout.png")
  end
  alpha_val = alpha_val.to_i
  raise "invalid alpha_val" unless (0...256).include? alpha_val
  x_offset = x * line_size
  y_offset = y * line_size

  norm_alpha_val = alpha_val / 256.0
  
  canvas.height.times do |y|
    canvas.width.times do |x|
      c_alpha = canvas[x, y] & 0xFF
      norm_c_alpha = c_alpha / 255.0
      #puts "norm_alpha_val #{norm_alpha_val}, norm_c_alpha #{norm_c_alpha}"
      img[x + x_offset, y + y_offset] = ( norm_alpha_val * norm_c_alpha * 256 ).to_i
    end
  end
end
