require "mqtt"

if ARGV.count < 2
  STDERR.puts(
    "You must provide two arguments, "+
    "num_cells and num_samples"
  )
  exit 1
end

def get_range_overlap( *ranges )
  begin_item = ranges.map( &:begin ).max
  end_item   = ranges.map( &:end   ).min
  return (begin_item..end_item)
end

def get_range_overlap_or_gap( *ranges )
  r = get_range_overlap( *ranges )
  if r.begin > r.end
    return ( r.end )..( r.begin )
  else
    return r
  end
end

num_cells = ARGV[0].to_i
num_samples = ARGV[1].to_i

range_low = 0
range_mid = 2**15
range_high= ( 2**16 - 1 )

density_variation = 200
knot_addl_density = 10_000

#how much the grain affects the readout value
grain_scale = 10_000

wood_density_range = ( 1000 .. 10_000 )
noise_range = ( -1 .. 1 )

densities = []
densities[0] = rand( wood_density_range )
( num_cells - 1 ).times do |i|
  densities[i+1] = densities[i] + rand( noise_range )
end

past_densities = densities

past_knots = []
future_knots = []

grain_direction = -1

MQTT::Client.connect( ARGV[2] || "127.0.0.1" ) do |c|
  loop do
    densities = num_cells.times.map do |i|
      ranges = [-1,0,1].map do |add|
        next nil if ( i+add ) < 0
        past_densities[i+add]
      end.compact.map do |v|
        ( v-density_variation )..( v+density_variation )
      end
      range = get_range_overlap_or_gap( *ranges )
      #( rand( range ) + rand( range ) )/2
      rand( range )
    end
    puts "densities b4 knotting"
    p densities
    current_knots = future_knots
    future_knots = []
    while rand( 5 ) == 0
      future_knots << rand()
    end
    densities_with_knots = densities.dup
    current_knots.each do |knot_position|
      puts "looking for #{knot_position}"
      res = []
      num_cells.times do |i|
        divided = 1.0 / num_cells
        r = ( ( i * divided )..( ( i + 1 ) * divided ) )
        puts "knot pos range"
        p r
        if r.include? knot_position
          densities_with_knots[i] +=
            ( knot_addl_density + rand( noise_range ) )
          puts "adding super density for knot to #{i}"
        end
      end
    end

    puts "densities after knotting"
    p densities_with_knots

    #-1 <= grain_direction <= +1
    grain_directions = num_cells.times.map do
      grain_direction
    end

    cells = densities_with_knots
      .zip( grain_directions )
      .map do |density, grain|

      #1/(sqrt((1/5)*2pi)) * e^(-(x^2)/2*40)
      num_samples.times.map do |i|
        #zero_point = grain_direction * num_samples
        x = (((i+0.5)/num_samples.to_f)*2)-1 + grain
        #enforce -1 <= x <= +1 with wraparound
        while x < -1
          x += 2
        end
        while x > +1
          x -= 2
        end
        grain_addl = (
          ( 0.892062 ) *
          ( Math::E**( -( x**2.0 ) / 2.0*40.0 ) )
        )
        v = density + rand( noise_range ) + ( grain_addl * grain_scale )
        if v > range_high
          v = range_high
        end
        if v < range_low
          v = range_low
        end
        v
      end
    end
    cells.each do |cell_set|
      puts cell_set.map{|c| c.to_i.to_s.rjust(5," ")}.join(", ")
    end
    #cells = num_cells.times.map{
    #  num_samples.times.map{ rand( 2**16 ) }
    #}
    #puts cells.map{ |s| s.join( "," ) }
    #puts "-----"
    msg = (
      "\x01" +
      [num_cells, num_samples].pack( "CC" ) +
      cells.flatten.pack( "n*" )
    )
    m = msg.force_encoding( "BINARY" )
    puts "now slep"
    #sleep( 0.5 )
    c.publish( "mcfly_samples", m )
    sleep( 0.020 )
    past_densities = densities
    past_knots = current_knots
    grain_direction += 0.1
    if grain_direction >= 1
      grain_direction -= 2
    end
  end
end
