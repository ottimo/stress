require 'static-monitor'
require 'config'

class PAcctStats < Monitor
	public
	def initialize
		@pid = Configuration.instance.processes #array di stringhe contenenti i nomi dei processi da monitorare
		@hertz = 100 #default in kernel 2.6
		@cpu=0.0
		@cpu_usage = 0.0
		@results = Array.new
	end

	def at_exit

	end
	def get_value
		@results << get_usage
	end
	def get_label
		return "PAcct Cpu Usage"
	end
	def get_report
		result = Array.new(@results)
		@results.clear
		return result
	end

	private
	def get_usage
		results = Hash.new
		get_info_from_sa
		calculate_cpu_usage
		results['Pacct CPU Usage'] = @cpu_usage
		return results
	end

	def get_info_from_sa
		begin
			p = `sa`
		rescue
			puts "sa must be called with root permission"
		end
		a = p.split("\n")
		@r = Array.new
		a.each do|l| 
			n = l.split(" ")
			@r << n unless @pid.index(n[-1]) == nil
		end
	end
	
	def calculate_cpu_usage
		@oldcpu = @cpu
		@cpu = 0.0
		@r.each do|l|
			@cpu += l[1].to_f
		end
		@cpu_usage = @cpu - @oldcpu
	end

end


