require 'tcp-server'
require 'stats-collector'


#t = Thread.new {
#	pid = 2244
#	cpu = CpuStats.new(pid)
#	mem = MemStats.new(pid)
#	cpuresults = Array.new
#	memresults = Array.new
#	interval=0.1
#	cpu.get_value
#	mem.get_value
#	sleep interval 
#	loop do
#		cpu.get_value.each do |k,v| 
#			print k.to_s+" % "+(v/interval).to_s+" " 
#			cpuresults << v
#		end
#		mem.get_value.each do |k,v| 
#			print k.to_s+" "+(v/interval).to_s+" " 
#			memresults << v
#		end
#		print "\n"
#		sleep interval
#	end
#}


#tcpserver = Server.new
#tcpserver.set_monitor_thread {
#	processes = Array.new
#	processes << "dovecot"
#	processes << "dovecot-auth"
#	processes << "pop3-login"
#	pid = 1031
#	interval=0.1
#	puts "Getting StatsCollector"
#	stats = StatsCollector.instance
#
#	puts "Setting array_processes"
#	stats.array_processes= processes
#	stats.pid= pid
#	print "PID: "
#	puts stats.pid
#	stats.interval= interval
#
#	puts "StatsCollector ready!"
#
##	stats.enable_cpu_stats
##	stats.enable_pacct_stats
#	stats.enable_process_stats
#	puts "Cpu monitors enabled"
#
##	stats.enable_mem_stats
#	puts "Mem monitors enabled"
#	stats.read_stat
#	sleep interval
#	puts "Start monitors"
#	loop do
#		stats.read_stat
#		sleep interval
#	end
#}
tcpserver = Server.new
puts "Daemon ready"

StatsCollector.instance.enable_process_stats
tcpserver.run

