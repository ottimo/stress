require 'static-monitor'
require 'dynamic-monitor'
require 'log-monitor'
require 'config'
require 'rubygems'
require 'sys/proctable'

class FrameSyncLogMonitor < LogMonitor
	public
	def initialize
    super

	end

	def at_exit

	end



  def packet_sent
    # do nothing
    #get_value
  end

  def packet_read
    get_value
  end

end


