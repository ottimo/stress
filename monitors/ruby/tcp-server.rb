require 'socket'
require 'stats-collector'

class ClientQuitError < RuntimeError; end

class Server

	attr :port
	attr :host
#port = ARGV.shift || 0 # default is to use the next available port
#host = ARGV.shift # default is to bind everything
def initialize
	@port = 60000
	@host = nil
end

	def run
		@server = @host ? TCPServer.open(@host, @port) : TCPServer.open(@port)
		@port = @server.addr[1]
		addrs = @server.addr[2..-1].uniq
	
		puts "*** listening on #{addrs.collect{|a|"#{a}:#{@port}"}.join(' ')}"
		loop do
		  @socket = @server.accept
		    s = @socket
		
		    port = s.peeraddr[1]
		    name = s.peeraddr[2]
		    addr = s.peeraddr[3]
		
		    puts "*** recieving from #{name}:#{port}"
		    	
		    begin
				s.puts "Ready"
		      while line = s.gets # read a line at a time
		        raise ClientQuitError if line =~ /^die\r?$/
		        puts "#{addr} [#{Time.now}]: #{line}"
				  if line.index("start")
					  start_test_case
					  send_ok
				  elsif line.index("stop")
					  stop_test_case
					  send_ok
				  elsif line.index("sent")
            packet_sent
            #send_ok
				  elsif line.index("read")
            packet_read
            #send_ok
				  elsif line.index("getreport")
					  getreport
				  else 
				  		begin 
				  			s.print "Error"
				  		rescue 
				  		 
				  		end
				  end
		      end
		
		    rescue ClientQuitError
		      puts "*** #{name}:#{port} disconnected"
			 rescue Errno::ECONNRESET
				puts "*** connection reset by peer"
		    ensure
		      s.close # close socket on error
		    end
		
		    puts "*** done with #{name}:#{port}"
			 stop_test_case		
		
		end
	end
	def set_monitor_thread(&block)
#		@monitor_thread = Thread.new { block.call }
		@block_call = block
	end
private
	def send_ok
		begin
			@socket.print "Ok"
		rescue

		end
	end

	def start_test_case
	 puts "start_test_case"
#	 StatsCollector.instance.reset_stats
	 StatsCollector.instance.start_test
#	 @monitor_thread = Thread.start { @block_call.call }
	end

	def stop_test_case
	 puts "stop_test_case"
	 StatsCollector.instance.stop_test
 #   @monitor_thread.exit! unless @monitor_thread == nil
#	 puts StatsCollector.instance.cpuresults.inspect
#	 puts StatsCollector.instance.memresults.inspect
	end
  def packet_sent
    puts "packet sent"
    StatsCollector.instance.packet_sent
  end
  def packet_read
    puts "packet read"
    StatsCollector.instance.packet_read
  end
	def getreport
	 StatsCollector.instance.get_report
	 res = StatsCollector.instance.results.inspect
	 #res = StatsCollector.instance.cpuresults.inspect
	 #res += StatsCollector.instance.memresults.inspect
	 begin
		 @socket.puts res
	 rescue
		 puts "data not sent"
	 end
	end
end
