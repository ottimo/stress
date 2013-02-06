#Here you can insert extra classes or what you need, then you have to include this file in your controller/model

require 'nokogiri'
require 'text'

class LDistance
	attr_reader :name
	
	def initialize(name =nil, first=nil, second=nil)
		@first = first
		@second = second
		@name = name
	end
	def l_distance (first = @first, second = @second)
		first = "" if first == nil
		second = "" if second == nil
		#return rand(101).to_i
		return Text::Levenshtein.distance(first, second).to_i
	end
end

class XMLTestcase < Nokogiri::XML::SAX::Document

	def initialize(simulation,filename)
		@filename =filename
		@simulation =simulation
	end

	def start_document
		puts "Start parsing XML!"
	end
	
	def end_document
		puts "Parsing END!"
	end

	def start_element name, attrs = []
		testcase_position = 0
		testcase_position = @simulation.testcases.last.position+1 unless @simulation.testcases.last == nil
		position = 0
		data = nil
		state_name = nil
		case name
		when "testcase"
			id=0
			run=0
			attrs.each do |p|
				id = p.second if p.first =="id"
				run = p.second if p.first =="run"
			end
			@testcase = Testcase.new(:filename => @filename, :id_anomaly => id, :run => run, :position => testcase_position)
			@simulation.testcases << @testcase
			@simulation.save
		when "send"
			position = @testcase.frames.last.position.to_i+1 unless @testcase.frames.last == nil
			attrs.each do |p|
				data = p.second if p.first =="data"
			end
			
			@testcase.frames << SentFrame.new(:data => data, :position => position)
			@testcase.save
		when "state"
			attrs.each do |p|
				data = p.second if p.first =="data"
				state_name = p.second if p.first =="name"
			end
			if state_name =~ /Anomaly-([1-9]\d*)/
				if($1) != @anomaly_number_of_value
					write_fault
				end
				@anomaly_number_of_value = $1
				@anomaly = "" if @anomaly == nil
				@anomaly += data
			elsif @anomaly != nil
				write_fault
			end
		when "read"
			if @anomaly != nil
				write_fault
			end
			position = @testcase.frames.last.position.to_i+1 unless @testcase.frames.last == nil
			read = nil
			attrs.each do |p|
				read = p.second if p.first =="name"
				data = p.second if p.first =="data"
			end
			if read == "Data received" 
				ld = LDistance.new
				l_dist = 0
				first_read = @simulation.testcases.first.frames[position]
				l_dist = ld.l_distance(first_read.data, data) unless first_read == nil
				@testcase.frames << ReadFrame.new(:data => data, :position => position, :l_distance => l_dist) 
				@testcase.save
			else
				l_dist = 0
				#l_dist = @simulation.testcases.first.frames[position].data.size unless @simulation.testcases.first.frames[position] == nil
				@testcase.frames << NoResponse.new(:data => data, :position => position, :l_distance => l_dist)
				@testcase.save
			end
		when "monitor"
			monitor_name = ""
			attrs.each do |p|
				monitor_name = p.second if p.first =="name"
				data = p.second if p.first =="data"
			end
			if monitor_name == "RTT Report"
				data = data.split(":")[-1]
				position = @testcase.monitorreports.last.position.to_i+1 unless @testcase.monitorreports.last == nil
				@testcase.monitorreports << RTTReport.new( :value => data.to_f, :position => position )
			end
		else
		
		end
	end

	def write_fault
		@testcase.frames.last.faults << Fault.new( :value => @anomaly , :number_of_value => @anomaly_number_of_value) unless @anomaly == nil
		@testcase.frames.last.save
		@anomaly_number_of_value = nil
		@anomaly = nil
	end
	

#	def end_element name
#		puts "#{name} ended"
#	end

end

class XMLOutputParser

	def parse(simulation, filename)
		puts "Read: "+filename
		
		sax_parse(simulation, filename)
		
		puts "Done."
	end
	
	def sax_parse(simulation, filename)
		# Create a new parser
		parser = Nokogiri::XML::SAX::Parser.new(XMLTestcase.new(simulation,filename))

		# Feed the parser some XML
		parser.parse_file(filename)
	end
	
	def xml_parse(simulation, filename)
		xmlreader = Nokogiri::XML::Reader.from_io(filename)
		begin
		
		
		rescue Nokogiri::SyntaxError 
			puts $!.inspect		
			puts $!.to_s+" at line:"+$!.line.to_s	
		end
		
		#TODO: chiudere il file nel xml reader
	end
	
	def read_data(file)
		@result = Hash.new
		rtt = Array.new
		result = Array.new
      xmlreader = Nokogiri::XML::Reader.from_io(file)
		begin
      	xmlreader.each do |l|
      	   case l.attribute("name") 
					when "No Packet Received"
						
					when "Report"
      	      	data = l.attribute("data")
      	      	adata = data.split("],")
      	      	adata.each do|d|
      	      	   d.tr!('[] ','')
      	      	   result << d.split(',')
      	      	end
					when "RTT Report"
						rtt << l.attribute("data")
					else
						#do nothing
      	   end
      	end
		rescue Nokogiri::SyntaxError 
			puts $!.inspect		
			puts $!.to_s+" at line:"+$!.line.to_s	
			exit -1
		end

     	@result["Report"] = result
		@result["RTT"] = rtt
		a = Array.new
		a << result[0][-1]
		a << result[-2][-1]	
		return a
	end
end
