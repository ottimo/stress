require 'static-monitor'
require 'dynamic-monitor'
require 'config'

class MyThread < Thread
  def at_exit
    super
    @pipe.close unless @pipe.closed?
  end
end

class LogMonitor < DynamicMonitor
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
    @@file2parse = "tail -F -n 0 /var/log/syslog"
	end

	def at_exit

	end

  def start_test
    @results.clear
    @queue = Array.new
    @thread = MyThread.new do
      IO.popen(@@file2parse) do |pipe|
        # Saves child pid
        puts pipe.inspect
        @pipe = pipe
        pidfile = File.new('/var/run/log-monitor.pid', 'w')
        pidfile.puts(pipe.pid)
        pidfile.close
        pipe.sync = true
        while str = pipe.gets
          @queue << str unless good == false
        end
      end
    end
    @thread.run
	end

  def stop_test
    @thread.exit! unless @thread == nil
    get_usage
  end

	def get_value
		get_usage
	end

	def get_label
		return "Log"
	end

	private

  include Sys

  def get_usage
    get_info_from_queue
    reset_data
	end

  def get_info_from_queue
    @queue.each do|l|
      good=false
      @processes.each{|p| good = true unless l.index(p) == nil }
      @results << l unless good == false
    end
  end

  def reset_data
    @queue.clear
  end
  
end


