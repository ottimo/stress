#Module STRESSMonitor
require 'static-monitor'

class DynamicMonitor < Monitor

	def start_test
    get_value
    @results.clear
    sleep @interval
		@thread = Thread.new{
			loop do
        get_value
				sleep @interval
			end
		}
    @thread.run
	end

	def stop_test
		@thread.exit! unless @thread == nil
	end
end

