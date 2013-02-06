#!/usr/bin/ruby

require 'nokogiri'
require 'rubygems'
require 'text'



class FaultDiscover < Nokogiri::XML::SAX::Document
	def initialize(anomalies,frames = nil)
		super()
		@anomalies = anomalies
		@frames = Array.new
		@frames = frames unless frames == nil
		reset
	end

	def reset
		@faults = Array.new
		@number_of_packet = -1 
		@evaluating_frame = false
		@search_read_frame = false
		@next_rtt = 0
		@rtt_lost_frame= 1.0
	end

	def end_document
		#puts "the document has ended"
		#controllo RTT se manca inserisco 5.0
		
		@faults.each do |id|
			@anomalies[id][@testcase]  << @rtt_lost_frame if (@anomalies[id][@testcase].size < 7) && @anomalies[id][@testcase].size != 0
			if @anomalies[id][@testcase].size == 0
				puts "something wrong in "+@testcase.to_s
				puts "faults: "+@faults.inspect
				puts "anomalies: "+@anomalies[id].inspect
				puts "frames: "+@frames[id].inspect
				a = STDIN.getc
				@anomalies[id].delete(@testcase)
			end
		end	

		reset
	end

	def start_element name, attributes = []
		#puts "#{name} started "+attributes.inspect
		if name == "testcase"
			@testcase = attributes[attributes.index("id")+1 ]
			@testcase += "-"
			@testcase += attributes[attributes.index("run")+1 ]
		end

		if name == "state"
			if( attributes[attributes.index("name")+1 ] =="Data sent" )
				@evaluating_frame = true
				@number_of_packet += 1
			elsif @evaluating_frame == true  
				name = attributes[attributes.index("name")+1 ] 
				if name.index("Anomaly") != nil && name.split("-")[1].to_i > 0
					@faults << @number_of_packet if @faults.index(@number_of_packet) == nil
					@anomalies[@number_of_packet] = Hash.new if @anomalies[@number_of_packet] == nil
					@anomalies[@number_of_packet][@testcase] = Array.new if @anomalies[@number_of_packet][@testcase] == nil
					@frames[@number_of_packet] = Hash.new() if @frames[@number_of_packet] == nil
					@frames[@number_of_packet][@testcase] = "" if @frames[@number_of_packet][@testcase] == nil
					@frames[@number_of_packet][@testcase] << attributes[attributes.index("data")+1]
					@search_read_frame = true
				end
				#@evaluating_frame = false
			end
			if( attributes[attributes.index("name")+1 ] =="Data received" && 
					@search_read_frame == true )
			#	@frames[@number_of_packet] = Hash.new if @frames[@number_of_packet] == nil
			#	@frames[@number_of_packet][@testcase] = "" if @frames[@number_of_packet][@testcase] == nil
				@frames[@number_of_packet][@testcase] << " -> " 
				@frames[@number_of_packet][@testcase] << attributes[attributes.index("data")+1 ]+" "+@testcase+" "+@number_of_packet.to_s
				@evaluating_frame = false
				@search_read_frame = false
			elsif attributes[attributes.index("name")+1 ] =="Report"
				data = attributes[attributes.index("data")+1 ]
				adata = data.split("],")
				@faults.each do |i|
					d = adata[i+1] #il primo risultato è riferito al primo pacchetto spontaneo
					d.tr!('[] ','')
					result = d.split(',')
					#puts "insert value for frame "+i.to_s+" in testcase "+@testcase
					@anomalies[i][@testcase]<< result[0].to_f
					@anomalies[i][@testcase]<< result[1].to_f
					@anomalies[i][@testcase]<< result[2].to_f
					@anomalies[i][@testcase]<< result[3].to_f
					@anomalies[i][@testcase]<< result[4].to_f
					
					d = adata.last
					d.tr!('[] ','')
					result = d.split(',')
					@anomalies[i][@testcase]<< result[i].to_f
				end

			elsif attributes[attributes.index("name")+1 ] =="RTT Report"
				if @faults.index(@next_rtt) != nil
					@anomalies[@next_rtt][@testcase] << ( attributes[attributes.index("data")+1 ]).split(":")[-1].to_f
				end
				@next_rtt += 1
			end
		end
	end

end

#def normalize(anomalies)
#	anomalies.each do |file|
#		file.each do|riga|
#			max_val = 0
#			riga.each do|val|
#				max_val = val if val > max_val
#			end
#			#puts max_val
#			for n in 0...riga.size
#				riga[n] = riga[n]/max_val
#			end
#		end
#	end
#end
def print_file_for_kde(output, anomalies)
	num=0
	anomalies.each do |file|
		outputfile = File.new(output+"-"+num.to_s,"w")

		for j in 0...7
			for i in 0...file.size
				outputfile.print file[i][j].to_s+" "
			end
			outputfile.print "\n"
		end

		#	file.each do |la|
		#		la.each do |l|
		#		 outputfile.print l.to_s+" "
		#		end
		#		outputfile.print "\n"
		#	end
		outputfile.close
#		puts num.to_s+": "+file.inspect
		num+=1
	end
end
def print_file_for_1class_svm(output,anomalies)
	num=0
	label = 1.0
	anomalies.each do |file|
		outputfile = File.new(output+"-"+num.to_s,"w")

		file.each do |la|
			index=1
			outputfile.print label.to_s+" "
			la.each do |l|
				outputfile.print index.to_s+":"+l.to_s+" "
				index+=1
			end
			outputfile.print "\n"
		end
		
		outputfile.close
#		puts num.to_s+": "+file.inspect
		num+=1
	end
end

def som_engine(output, anomalies, frames)
	require 'benchmark'
	require 'rubygems'
	require 'normalizer'
	require 'som'

	i=0 #primo pacchetto

	numero_classi = 2

	outputfile = File.new(output, "w")

	anomalies.each do |anomalie|
	##	puts anomalie.inspect
		min, max = Normalizer.find_min_and_max(anomalie.values)

		normalizer = Normalizer.new(:min => min, :max => max)

		data = []

		anomalie.each do |k,n|
			anomalie[k] = normalizer.normalize(n)
		end
		anomalie.each_value do |l|
#			l.delete_at(-2)
			l.delete_if{|v| v.nan? }
		end
		#puts data.inspect

		a = SOM.new(anomalie.values, :nodes => numero_classi)

		#	puts a.global_distance_error

		#	times = Benchmark.measure do
		a.train
		#	end

		classes = Array.new(numero_classi,0)
	#	data.each do |j|
		id0 = []
		id1 = []
#		for j in 0...data.size
		anomalie.each_key do |j|
			res = a.classify(anomalie[j])
			classes[res[0]] += 1 
			if res[0]==0
				id0 << j
			else
				id1 << j
			end
		end

		outputfile.puts "----------------------------------------------------------------------------------------------"
		outputfile.print i.to_s+": Classi sono? "
		outputfile.print classes.inspect
		outputfile.print " su "
		outputfile.puts data.size

		if classes[0] < classes[1]
			id0.each do|j| 
				outputfile.print j+": "
				outputfile.puts frames[i][j]
			end
		else
			id1.each do|j| 
				outputfile.print j+": "
				outputfile.puts frames[i][j]
			end
		end

#		outputfile.puts frames[i].inspect

		#	puts a.classify(data[2]).inspect

		#	puts a.global_distance_error

		#	puts a.nodes.inspect

		#	puts times

		i+=1
	end
	outputfile.close
end

if ARGV.size!=2
	puts "fault-parser.rb xml_directory radice_file_di_output"
	exit 0
end

anomalies = Array.new
frames = Array.new

parser = Nokogiri::XML::SAX::Parser.new(FaultDiscover.new(anomalies,frames))
puts ARGV[0]
output = ARGV[1]
listfile = Array.new
xmls = Dir.new(ARGV[0])
xmls.each do |f|
	listfile << f unless  f.index(".xml") == nil
end
listfile.each do |nomefile|
	file = File.new(xmls.path+nomefile,'r')
	parser.parse(file)
	file.close
end

## buggato va usato dopo aver invertito la matrice
#normalize(anomalies)

##### stampa su file #####
#print_file_for_kde(output, anomalies)

#print_file_for_1class_svm(output,anomalies)


som_engine(output,anomalies,frames)


#Distanze di Levenshtein
#Text::Levenshtein('pippo','pluto')
