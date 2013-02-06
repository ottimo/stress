#Module STRESSMonitor

class Monitor
public
  def initialize
    @results = Array.new
  end

  def print_log(string)
    puts string
  end

	def get_value

	end
	def get_label

	end

 	def start_test
    @results.clear
		get_value
    @results.clear
	end

  def packet_sent
    
  end

  def packet_read

  end

	def stop_test
		get_value
	end

	def get_report
		return @results
	end

end
