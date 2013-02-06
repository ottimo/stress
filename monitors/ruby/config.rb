require 'singleton'

class Configuration
	include Singleton

	attr :pid, true
	attr :array_processes, true
	attr :interval, true
	attr_reader :cpu
	attr_reader :mem
	attr_reader :processes
	attr_reader :results
	attr_reader :cpuresults
	attr_reader :memresults

	def initialize
		@interval = 0.1
		@pid = 1024
		@processes = Array.new
		@processes << "dovecot"
		@processes << "pop3-login"
		@processes << "dovecot-auth"
		@processes << "pop3"
		@processes << "pop"
		@processes << "authdaemond"
		@processes << "courierlogger"
		@processes << "couriertcpd"
		@processes << "courierpop3d"
		@processes << "courier"
		@processes << "popa3d"
	end

end
