require 'cpu-usage'
require 'process-monitor'
require 'framesync-process-monitor'
require 'wait-time-monitor'
require 'log-monitor'
require 'pacct'
require 'mem-usage'
require 'singleton'

class StatsCollector
	include Singleton

	attr :pid, true
	attr :array_processes, true
	attr :interval, true
	attr_reader :cpu
	attr_reader :mem
	attr_reader :results

	def initialize
		@interval = 0.1
		@results = Array.new
		@mon = Array.new
	end
	
	def enable_cpu_stats
		cpu = CpuStats.new
		puts cpu.get_value.inspect
		@mon << cpu
	end

	def enable_process_stats
		puts "Enabling ProcessMonitor"
		#process = ProcessMonitor.new
    process = FrameSyncProcessMonitor.new
		puts process.get_value.inspect
		@mon << process
		@mon << WaitTimeMonitor.new
	end

	def enable_pacct_stats
		pacct = PAcctStats.new
  		puts pacct.get_value.inspect
		@mon << pacct
	end
	
	def enable_mem_stats
		mem = MemStats.new
		puts mem.get_value.inspect
		@mon << mem
	end

	def start_test
		@results.clear
		@mon.each do|m|
			m.start_test
		end
	end

	def stop_test
		@mon.each do|m|
			m.stop_test
		end
	end

  def packet_sent
		@mon.each do|m|
			m.packet_sent
		end
	end

  def packet_read
		@mon.each do|m|
			m.packet_read
		end
	end

	def get_report
		@mon.each do|m|
			@results << m.get_report
		end
	end

	def read_stat
#		unless @cpu == nil
#			@cpu.get_value.each do |k,v| 
#				print k.to_s+" % "+(v.to_f/@interval).to_s+" " 
#				@cpuresults << v.to_i
#			end
#		end
#		unless @mem == nil
#			@mem.get_value.each do |k,v| 
#				print k.to_s+" "+(v.to_f/@interval).to_s+" " 
#				@memresults << v.to_f/@interval
#			end
#		end
		@mon.each do|m|
			m.get_value.each do |k,v| 
				print k.to_s+" % "+(v.to_f/@interval).to_s+" " 
				@results << v.to_f
			end
		end
		print "\n"
	end
	
	def reset_stats
		@results.clear
	end

end
