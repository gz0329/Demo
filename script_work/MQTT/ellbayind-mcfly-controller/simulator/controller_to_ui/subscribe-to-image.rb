require "mqtt"
begin
  require "oily_png"
rescue LoadError
  require "chunky_png"
end
require_relative "paint-lines-lib"

## MONKEYPATCHING
class MQTT::Client
  def handle_packet(packet)
      @last_ping_response = Time.now
    if packet.class == MQTT::Packet::Publish
      # Add to queue
      @read_queue.push(packet)
    elsif packet.class == MQTT::Packet::Pingresp
      @last_ping_response = Time.now
    elsif packet.class == MQTT::Packet::Puback
      @pubacks_semaphore.synchronize do
        @pubacks[packet.id] = packet
      end
    end
    # Ignore all other packets
    # FIXME: implement responses for QoS  2
  end
end

unless (2..3).include? ARGV.size
  STDERR.puts(
    "Must provide number of message to read, "+
    "number of cells, and "+
    "MQTT server addr (optional, defaults to 127.0.0.1)"
  )
  exit 1
end

line_size = 15

messages_to_read = ARGV[0].to_i
num_cells = ARGV[1].to_i

img = ChunkyPNG::Image.new(
  messages_to_read*line_size,
  num_cells*line_size,
  ChunkyPNG::Color::TRANSPARENT
)

msgs_received = 0

messages = []
MQTT::Client.connect( ARGV[2] || "127.0.0.1" ) do |c|
  c.get( "mcfly_samples" ) do |_, message|
    m = message.force_encoding( "BINARY" )
    if m[0] != "\x01"
      raise "invalid message"
    end
    messages << m
    msgs_received += 1
    puts msgs_received
    if msgs_received == messages_to_read
      break
    end
  end
end
if true
  messages.each_with_index do |m, msgs_received|
    m_num_cells, num_samples = m[1..2].unpack( "CC" )
    if m_num_cells != num_cells
      raise "cell count should be #{m_num_cells}"+
      " not #{num_cells}"
    end
    data = m[3..-1].unpack( "n*" )
    data.each_slice( num_samples ).with_index do |samples, i|
      #avg = samples.inject( :+ ) / samples.size.to_f
      #puts "avg: #{avg}"
      #teint = avg / ( 2**16 / 256 ).to_f
      #puts "teint: #{teint}"

      #img[msgs_received, i] =
      #  ChunkyPNG::Color.grayscale( teint.to_i )
      samples.each_with_index do |sample, si|
        angle_range_from = ( si   /num_samples.to_f)*360
        angle_range_to   = ((si+1)/num_samples.to_f)*360
        teint = sample / ( 2**16 / 256 ).to_f
        #color = ChunkyPNG::Color.rgba( 0,0,0, teint.to_i )
        x = msgs_received
        y = i

        paint_antialiased_angle_range_on_image(
          img,
          line_size,
          teint,
          angle_range_from,
          angle_range_to,
          x, y
        )
      end
    end
    puts msgs_received
  end
end

out_fn = "output2.png"

img.save( out_fn )

=begin
system(
  "convert",
  out_fn,
  "-scale",
  "400%",
  "output-scaled.png"
)
=end
