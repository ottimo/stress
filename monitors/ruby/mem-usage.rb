require 'static-monitor'
require 'config'

class MemStats < Monitor
	public
	def initialize
    super
		@pid = Configuration.instance.pid
	end

	def at_exit

	end
	def get_value
		get_usage
    a = Array.new
    a << @vmsize
    a << @vmdata
    @results << a
	end
	def get_label
		return "Mem Usage"
	end

	private
	def get_usage
		results = Hash.new
		get_info_from_files
		results['VMSize'] = get_memory_size
		results['VMData'] = get_process_data
		return results
	end

	def get_memory_size
		return @vmsize
	end

	def get_process_data
		return @vmdata
	end

	def get_info_from_files
		begin
			statm = IO.read("/proc/#{@pid}/statm")
		rescue StandardError, Errno::EACCES
			puts "Exit with errors."
			exit(-1)
		end
		mem = statm.split(" ")
		@vmsize =	mem[0].to_i
		@vmdata =	mem[5].to_i
	end
end

