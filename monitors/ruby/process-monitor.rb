require 'static-monitor'
require 'dynamic-monitor'
require 'config'
require 'rubygems'
require 'sys/proctable'
require 'cproctable/cproctable'

class ProcessMonitor < DynamicMonitor
	public
  include CProc
	def initialize
    super
		@processes = Configuration.instance.processes
		@interval = Configuration.instance.interval
		@hertz = 100 #default in kernel 2.6
		@seconds = 0
		@total_time = 0
		@total_global_time = 0
    @proctable = CProcTable.new
	end

	def at_exit

	end
	def get_value
		get_usage
    a = Array.new
    a << @cpu_usage
    a << @vmsize
    a << @vmdata
    a << @vmresident
    a << @vmtext

    a << Time.now.to_f
    @results << a
	end
	def get_label
		return "Cpu Usage"
	end

	private

  include Sys

  def get_usage
		results = Hash.new
		get_info_from_files
		calculate_cpu_usage
		results['Cpu Usage'] = @cpu_usage
		return results
	end

  def get_info_from_files
    #dai nomi dei processi preleva il pid
    reset_data
    pids = @proctable.get_list_processes
#    begin
#      ProcTable.ps do|p|
#        pids << p.pid unless @processes.index(p.comm) == nil
#     end
#    rescue
#      print_log("An Error is occur: "+$!+"\nException captured!!!")
#    end
    pids.each { |p| get_info_from_file(p[0]) unless @processes.index(p[1].tr!('() ','')) == nil }
  end

  def reset_data
		@state = ""
    @utime  = 0
		@stime  = 0
		@cutime = 0
		@cstime = 0
		@vmsize = 0
		@vmdata = 0
		@nswap = 0
		@delayacct_blkio_ticks = 0
  end

	def get_info_from_file(pid)
		begin
			stat = IO.read("/proc/#{pid}/stat")
			statm = IO.read("/proc/#{pid}/statm")
	      psinfo = stat.split(" ")
	      mem = statm.split(" ")
			@state = psinfo[2].to_s
	  		@utime  += psinfo[13].to_f
	    	@stime  += psinfo[14].to_f
	      @cutime += psinfo[15].to_f
	  		@cstime += psinfo[16].to_f
	  		@nswap += psinfo[35].to_f
	  		@delayacct_blkio_ticks += psinfo[41].to_f
	    	@vmsize =	mem[0].to_i
	    	@vmresident =	mem[1].to_i
	      @vmtext =	mem[3].to_i
	      @vmdata =	mem[5].to_i
		rescue StandardError, Errno::EACCES
			puts "Process with pid #{pid} not found"
		end
	end
	
	def calculate_cpu_usage
		@oldtotal_time = @total_time
		@total_time =  @utime + @stime
		@total_time += @cutime + @cstime #wait for children 
		pcup = (@total_time - @oldtotal_time) 
		@cpu_usage = pcup.to_i
	end

end


