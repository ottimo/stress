#!/usr/bin/ruby

nomefile = ARGV[0]
output = ARGV[1]
if nomefile == nil || output == nil
	puts "Usage: txt2kde.rb nomefile output"
	exit(0)
end
puts "Nome del file: "+nomefile

file = File.new(nomefile)
numfile=0
id=0
number=0
lines_each_file=0
outputarray = nil
file.each_line do |l|
#	puts l
	l.chop!
	a = l.split("\t")

	####calcolo numero file e line per file
	if number < a.size
		number = a.size 
		a.each do |li|
			 numfile += 1 unless li.index("CPU")==nil
		end
		lines_each_file = (number/numfile).to_i
#		outputarray=Array.new(numfile, Array.new(lines_each_file, Array.new))
		outputarray=Array.new
		numfile.times{ outputarray << Array.new }
		outputarray.each do |p|
			lines_each_file.times{p << Array.new}
		end
#		puts outputarray.inspect
#		puts outputarray[0].inspect
#		puts outputarray[0][0].inspect
#		puts outputarray[1][0].class
	end
	#### fine calcolo numero file e line per file
	
	lineno = 0
	packetno= 0
	a.each do |v|
	#	puts id.to_s+" "+outputarray[packetno][lineno%lines_each_file].class.to_s+" "+packetno.to_s+" "+lineno.to_s
	#	puts packetno.to_s+" "+lineno.to_s+"->"+(lineno%lines_each_file).to_s+" V "+v.class.to_s+" "+v
		(outputarray[packetno][lineno%lines_each_file]) << v.to_s.split(":")[1].to_f
		lineno+=1
		if (lineno%lines_each_file) == 0
			packetno+=1
		end		 
	end
	if lineno%lines_each_file != 0
	#while lineno%lines_each_file != 0
	#while lineno < number
	#	puts id.to_s+" "+outputarray[packetno][lineno%lines_each_file].class.to_s+" "+packetno.to_s+" "+lineno.to_s
#scommentare per inserire un valore di default
#		(outputarray[packetno][lineno%lines_each_file]) << -1.0 
#		lineno+=1

	for i in 0...lineno%lines_each_file do 
		outputarray[packetno][i].delete_at(-1)
	end

#		if lineno % lines_each_file == 0
#			packetno+=1
#		end		 
	end

#	break if id == 2
	id+=1
end
file.close

num = 0
outputarray.each do |file|
	outputfile = File.new(output+"-"+num.to_s,"w")
	file.each do |la|
		la.each do |l|
		 outputfile.print l.to_s+" "
		end
		outputfile.print "\n"
	end
	outputfile.close
	num+=1
end


