require 'static-monitor'
require 'dynamic-monitor'
require 'process-monitor'
require 'config'
require 'rubygems'
require 'sys/proctable'

class FrameSyncProcessMonitor < ProcessMonitor
	public
	def initialize
    super

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

end


