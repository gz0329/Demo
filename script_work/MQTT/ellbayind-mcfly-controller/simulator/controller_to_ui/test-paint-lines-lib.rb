require "chunky_png"
require_relative "paint-lines-lib"
$debug_paint_lines = false
if false

png = ChunkyPNG::Image.new( 300, 300, ChunkyPNG::Color::TRANSPARENT )

ang = 99

paint_angle_range_on_image( png, 150, ChunkyPNG::Color::BLACK, 250, 185 )
puts
puts
paint_angle_range_on_image( png, 150, ChunkyPNG::Color::BLACK, 255, 195 )
puts
puts
paint_angle_range_on_image( png, 150, ChunkyPNG::Color::BLACK, 260, 205 )
exit
puts "----"

#paint_antialiased_angle_range_on_image( png, 150, 255, 0, ang, 1, 1 )
#puts
#puts
paint_antialiased_angle_range_on_image( png, 150, 255, 180, 180+ang, 1, 1 )

else

line_size = 15
img = png = ChunkyPNG::Image.new(
  15 * line_size,
  15 * line_size,
  ChunkyPNG::Color::TRANSPARENT
)

angle_a = 0
angle_b = 45

15.times do |x|
  15.times do |y|
    puts "#{x}, #{y}: painting #{angle_a} to #{angle_b}"
    paint_antialiased_angle_range_on_image(
      img,
      line_size,
      255,
      angle_a,
      angle_b,
      x, y
    )
    angle_a += 7
    angle_b += 13
    angle_a %= 360
    angle_b %= 360
  end
end

end

png.save 'paint-lines-test-output.png'
