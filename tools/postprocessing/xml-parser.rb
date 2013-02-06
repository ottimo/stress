#!/usr/bin/ruby

require '../../monitors/ruby/config.rb'
require 'nokogiri'
require 'progress-window.rb'

class DataCollector

	def initialize
		@outputfile = File.new("/tmp/output.txt","w")
		@indexfile = File.new("/tmp/index.txt","w")
		@file = File.new(ARGV[1])
		@last = nil
		@max = Hash.new(0.0)
		@min = Hash.new(10000000.0)
	 @line = 0

	#	@window = ProgressWindow.new
	#	@window.show_all
	#	@t = Thread.new{
	#		Gtk.main
	#	}
	#	@t.run

		@num_interval = 4
	end

	def at_exit
		@file.close
		@outputfile.close
		@indexfile.close
	end

	def give_name_array(name_array)
		id=0
		run=0
		count=0
		name_array.each do |ids|
			ids.each do |testcase|
				count+=1
			end
		end
	#	@window.set_num_file(count)
		

		name_array.each do |ids|
			print "testcases id: "+id.to_s
			ids.each do |testcase|
				load_data(testcase, id, run)
				run+=1
				print "#"
	#			@window.file_done
				print_output_svm(id, run)
			end
			puts " done."
			id+=1
			run=0
		end

#		puts_parameter
#		
#		id=0
#		run=0
#
#		name_array.each do |ids|
#		#	print "testcases id: "+id.to_s
#			ids.each do |testcase|
#				load_data(testcase, id, run)
#				mark_result
#				run+=1
#		#		print "#"
#			end
#		#	puts " done."
#			id+=1
#			run=0
#		end
	end

#	def mark(name, value)
#		res = 0
#		value = value.to_f
#		@parameters[name].each do|v|
#			res +=1 unless value < v
#		end
#		return res
#	end

#	def mark_result
#		marked_testcase = Array.new(0)
#		id=0
#		@result["Report"].each do |v|
#			marked_testcase[id] = mark("CPU"+id.to_s, v[0])
#			marked_testcase[id] += mark("VMSize"+id.to_s, v[1]) * 4
#			marked_testcase[id] += mark("VMData"+id.to_s, v[2]) * 16
#			id+=1
#		end
##		@result["Log"]
#		id=0
#		@result["RTT"].each do |v|
#			marked_testcase[id] += mark("RTT"+id.to_s ,	v.split(':')[-1].to_f) * 64
#			id+=1
#		end
#		print_data(marked_testcase[1..-1])
#	end

	def print_output_svm
		@result.each do |k,val|
			id=0
			unless k == "Log"
				val.each do |v|
					if k=="Report"
						@outputfile.print "CPU"+id.to_s+":"+v[0]+"\t"
						@outputfile.print "VMSize"+id.to_s+":"+v[1]+"\t"
						@outputfile.print "VMData"+id.to_s+":"+v[2]+"\t"
					else
						@outputfile.print k+id.to_s+":"+v.to_s+"\t"
					end
					id+=1
				end
			end
		end
		@outputfile.print "\n"
	end
	
	def print_output_svm(testid, testrun)
		lines = Array.new(@result["Report"][0].size, "")
		@result.each do |k,val|
			id=0
			unless k == "Log"
				val.each do |v|
#					puts v.inspect
					if k=="Report" && id > 0
						if v != val.last
							lines[id-1] += "CPU:"+v[0]+"\t"
							lines[id-1] += "VMSize:"+v[1]+"\t"
							lines[id-1] += "VMData:"+v[2]+"\t"
							lines[id-1] += "VMResident:"+v[3]+"\t"
							lines[id-1] += "VMText:"+v[4]+"\t"
						else
							 ind = 0
							 v.each do|l|
								lines[ind-1] += "IOWAIT:"+l+"\t" if ind >0
								ind+=1
							end
						end
					elsif k== "RTT"
						vv = v.split(':')[-1]
						lines[id] += k+":"+vv.to_s+"\t"
					elsif k!= "Report"
						lines[id] += k+":"+v.to_s+"\t"
					end
					id+=1
				end
			end
		end
		
#		puts lines.inspect
		
		tooutput = lines[0...-1]
		tooutput.each do |l|
			@line +=1
			unless l.size==0
				@outputfile.print l 
				@indexfile.print @line.to_s+":"+testid.to_s+","+testrun.to_s+"\n"
			end
		end
				@outputfile.print "\n" 
	end

#	def print_data(marked_testcase)
#		marked_testcase.each do|p|
#			if p != 0
#				@outputfile.print(p)
#			else
#				@outputfile.print(256)
#			end
#			@outputfile.print("\t")
#		end
#		(6-marked_testcase.size).times{@outputfile.print "0\t"}
#		@outputfile.print("\n")
##		puts marked_testcase.inspect
#	end

#	def puts_parameter
#		num_interval = @num_interval
#		@parameters = Hash.new
#		@max.each do |k,v|
#			puts "MAX: "+k
#			a = Array.new
#			ampiezza = v - @min[k].to_f
#			sezione = ampiezza / num_interval
#		#	a << @min[k].to_f
#			res = @min[k].to_f
#			num_interval.times{
#				res += sezione
#				a << res
#			}
#			#a << v
#			@parameters[k] = a
#		end
#		puts @parameters.inspect
#	end

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
	
	def log_data(interval)
		file = @file
		tstart = Time.at(interval[0].to_f)
		tend = Time.at(interval[-1].to_f)
#		puts interval[0].to_s+" = "+tstart.to_s
#		puts interval[-1].to_s+" = "+tend.to_s
		res=Array.new

		if @last!=nil
#			puts "CHECKING LAST: "+@last
			time = get_time(@last)
			if time < tstart
				return res				
			end
         if tstart <= time && time <= tend
            good = false
            Configuration.instance.processes.each do |p|
               good = true unless @last.index(p) == nil
            end
            res << @last if good == true
				@last = nil
         end
			if time>tend
				return res
			end
		end

		file.each_line do|l|
			time = get_time(l)
#			puts "CHECKING:     "+l
#			puts time
#			puts "Interval: "+tstart.to_s+" -> "+tend.to_s
			if tstart <= time && time <= tend
				good = false
				Configuration.instance.processes.each do |p|
					good = true unless l.index(p) == nil
				end
				res << l if good == true
			end
			if time>tend
				@last = l
				break
			end
		end
		return res
	end

	def get_time(l)
		al = l.split(' ')
		data = al[0].split('T')[0]
		data = data.split('-')
		ora = al[0].split('T')[1].split('+')[0]
		microsec = ora.split('.')[1]
		ora = ora.split('.')[0]
		#time = Time.local(2010, al[0], al[1], al[2].split(':')[0] ,al[2].split(':')[1] , al[2].split(':')[2] ) 
		time = Time.local( data[0], data[1], data[2],ora.split(':')[0],ora.split(':')[1], ora.split(':')[2], microsec ) 
		return time
	end

	def load_data(data, id, run)
		#puts data+" "+id.to_s+" "+run.to_s
		file = File.new(data)
		interval = read_data(file)
		file.close
		logs = log_data(interval)
		@result["Log"] = logs
		#puts @result.inspect
		elaborate
		#puts "############END#############"
	end

	def elaborate
		#puts "<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>"
		@result.each do |k,v|
			case k
				when "Report"
					id = 0
					v.each do |val|
						@max["CPU"+id.to_s] 	= val[0].to_f if val[0].to_f >  @max["CPU"+id.to_s] 
						@max["VMSize"+id.to_s] = val[1].to_f if val[1].to_f >@max["VMSize"+id.to_s] 
						@max["VMData"+id.to_s] = val[2].to_f if val[2].to_f >@max["VMData"+id.to_s] 
						@min["CPU"+id.to_s] 	= val[0].to_f if val[0].to_f <  @min["CPU"+id.to_s] 
						@min["VMSize"+id.to_s] = val[1].to_f if val[1].to_f <@min["VMSize"+id.to_s] 
						@min["VMData"+id.to_s] = val[2].to_f if val[2].to_f <@min["VMData"+id.to_s] 
						id+=1
					end
				when "RTT"
					id = 0
					v.each do |val|
						val = val.split(':')[-1]
						@max["RTT"+id.to_s] = val.to_f if val.to_f > @max["RTT"+id.to_s] 
						@min["RTT"+id.to_s] = val.to_f if val.to_f < @min["RTT"+id.to_s] 
						id+=1
					end
			end
#			puts k
#			puts v
		end
		#puts "<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>"
	end

end


def id2string(id)
	string = id.to_s
	mancano = 12 -	string.size
	string.reverse!
	mancano.times { string << '0' }
	string.reverse!
	return string
end

if ARGV[0] == nil || ARGV[1] == nil
	puts "Usage: #{$0} directory_test_output syslog_file"
	exit(-1)
end
id = 0
run = 0
dirname = ARGV[0]
xmls = Array.new
directory = Dir.new(dirname)
directory.each do |filename|
	if filename =~ /state-([0-9]{12})-([0-9]{12})\.xml/
		xmls[$1.to_i]=Array.new if xmls[$1.to_i]== nil
		xmls[$1.to_i][$2.to_i] = "#{dirname}#{filename}"
		#IO.readlines("#{dirname}#{filename}")
	end
end

engine = DataCollector.new
engine.give_name_array(xmls)


#IO.readlines("/tmp/state-#{id}-#{run}.xml")
