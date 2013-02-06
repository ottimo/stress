require 'static-monitor'
require 'dynamic-monitor'
require 'config'

class WaitTimeMonitor < DynamicMonitor
	public
	def initialize
    super
#		@pid = Configuration.instance.pid
#		@hertz = 100 #default in kernel 2.6
#		@seconds = 0
#		@total_time = 0
#		@total_global_time = 0
		@oldwtime = 0
	end

	def at_exit

	end

	def start_test
    get_value
    @results.clear
    get_value
	end

  def packet_sent
    # do nothing
    #get_value
  end

  def packet_read
    get_value
  end

	def get_value
		get_usage
 	   @results << @wait_time
	end
	def get_label
		return "Cpu Usage"
	end

	private
	def get_usage
		get_info_from_files
		calculate_cpu_usage
	end

	def get_memory_size
		return @vmsize
	end

	def get_process_data
		return @vmdata
	end

	def get_info_from_files
		begin
			#stat = IO.read("/proc/#{@pid}/stat")
			tot_stat = IO.read("/proc/stat")
			#statm = IO.read("/proc/#{@pid}/statm")
			#uptime = IO.read("/proc/uptime")
		rescue StandardError, Errno::EACCES
			puts "Exit with errors."
			exit(-1)
		end
		#mi servono il 14° utime ed il 15° stime 22° istante di avvio in "sec dal boot"
#		a = Array.new
#		psinfo.each(" "){|v|	a << v }
	#	psinfo = stat.split(" ")
		tot = tot_stat.split(" ")
	#	mem = statm.split(" ")
		@totutime = 	tot[1].to_i
		@totltime = 	tot[2].to_i
		@totstime = 	tot[3].to_i
		@totitime = 	tot[4].to_i
		@totwtime =		tot[5].to_i
	#	@utime = 		psinfo[13].to_f
	#	@stime = 		psinfo[14].to_f
	#	@cutime = 		psinfo[15].to_f
	#	@cstime = 		psinfo[16].to_f
	#	@start_time = 	psinfo[21].to_f
		
	#	@seconds_uptime = uptime.split(" ")[0].to_f

	#	@vmsize =	mem[0].to_i
	#	@vmdata =	mem[5].to_i
	end
	
	def calculate_cpu_usage
#		@oldtotal_time = @total_time
#		@oldtotal_global_time = @total_global_time
#		#@oldseconds = @seconds
#		@total_global_time = @totutime + @totltime + @totstime + @totitime
#		#@seconds = @seconds_uptime - @start_time / @hertz
#		@total_time =  @utime + @stime
#		@total_time += @cutime + @cstime #wait for children 
#		#pcup = (total_time *100 / @hertz ) / (@seconds - @oldseconds)
#		#frame_scale = 100.0 / (@hertz * (@seconds - @oldseconds) * 1)
#		#pcup = (@total_time *100 / @hertz ) / @seconds 
#		pcup = (@total_time - @oldtotal_time) 
#      # * 100 / (@total_global_time - @oldtotal_global_time)
#		@cpu_usage = pcup.to_f  
		@wait_time = @totwtime - @oldwtime
	   @oldwtime = @totwtime	

	end

end


