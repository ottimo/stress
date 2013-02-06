#!/usr/bin/ruby

require 'nokogiri'
require 'gtk2'

class TestCase

	attr :duration, true

	def initialize
		@rtt = Array.new
		@frames = Array.new
		@read_frames = Array.new
		@sent_frames = Array.new
		@report = Hash.new
		@report["CPU"] = Array.new
		@report["VMSize"] = Array.new
		@report["VMData"] = Array.new
		@report["VMResident"] = Array.new
		@report["VMText"] = Array.new
		@report["Timestamp"] = Array.new
		@report["IOWait"] = Array.new
		@pxl = 5
	end

	def reset
		@rtt.clear
		@frames.clear
		@report.each do|k,v|
			v.clear or puts "No clear methods for "+k.to_s 
		end
	end

	def add_sent_frame(data)
		@frames << data
		@sent_frames << data
	end

	def add_read_frame(data)
		@frames << data
		@read_frames << data
	end

	def add_rtt(data)
		@rtt << data
	end

	def add_report(data)
		adata = data.split("],")
		adata.each do|d|
			unless d == adata.last
				d.tr!('[] ','')
				result = d.split(',')
				@report["CPU"] << result[0]
				@report["VMSize"] << result[1]
				@report["VMData"] << result[2]
				@report["VMResident"] << result[3]
				@report["VMText"] << result[4]
				@report["Timestamp"] << result[5]
			else
				d.tr!('[] ','')
				result = d.split(',')
				@report["IOWait"] = result
			end
		end
		@report.each do |k,v|
			v.delete_at 0
			v.delete_at -1 
		end
	end

	def add_lost_frame
		@read_frames << "LOST-IT!!!"
		@frames << "LOST-IT!!!"
	end

	def draw(area)
		alloc = area.allocation

		xstart = 10+45
		xend = alloc.width-55
		ymin = 15+60+30
		ystep = (alloc.height - ymin)/@frames.size
		y=ymin
		playout = area.create_pango_layout
		#		playout.alignment= Pango::ALIGN_CENTER 
		@frames.each do|p|
			if @read_frames.index(p) != nil
				draw_read(area, xstart, ymin, xend)
			else
				draw_sent(area, xstart, ymin, xend)
			end
			playout.text=p.to_s
			area.window.draw_layout(area.style.fg_gc(area.state), 
											(alloc.width/2) - (playout.pixel_size[0]/2) , ymin,
											playout)
			ymin+=ystep
		end
	end

	private
	def draw_read(area, xstart, ymin, xend)
		area.window.draw_line(area.style.fg_gc(area.state), 
									 xstart, ymin+@pxl, xend, ymin-@pxl )
	end
	def draw_sent(area, xstart, ymin, xend)
		area.window.draw_line(area.style.fg_gc(area.state), 
									 xstart, ymin-@pxl, xend, ymin+@pxl )
	end

end

class XmlViewer < Gtk::Window

	def initialize
		super(Gtk::Window::TOPLEVEL)
		signal_connect("delete_event") {
			Gtk.main_quit
			false
		}


		mainbox = Gtk::VBox.new(false,5)
		add(mainbox)
		menubox = Gtk::HBox.new
		mainbox.pack_start(menubox,false,false,0)

		menubox.pack_start(get_file_chooser, true, true,0)

		contentbox = Gtk::VBox.new(false,5)
		mainbox.add(contentbox)
		buttonbox = Gtk::HButtonBox.new
		mainbox.pack_end(buttonbox,false,false,0)

		cancelbutton = Gtk::Button.new("Cancel")
		cancelbutton.signal_connect("clicked") {
			Gtk.main_quit
			#exit
			false
		}
		buttonbox.pack_end(cancelbutton, true, false, 0)
		nb = Gtk::Notebook.new
		contentbox.pack_start(nb, true, true, 0)



		viewer = Gtk::ScrolledWindow.new
		@viewer = Gtk::TextView.new
		@viewer.editable= false
		@viewer.cursor_visible= false
		@viewer.buffer= Gtk::TextBuffer.new
		viewer.add(@viewer)
		#	@title = Gtk::Label.new("Processing XML file...")
		#	contentbox.add(@title)
		#	@label = Gtk::Label.new("0 %")
		#	contentbox.add(@label)
		#	@num_file = 0
		#	@file_done = 0

		@darea = Gtk::DrawingArea.new
		@darea.can_focus=true
		@darea.signal_connect "expose-event" do  
			on_expose
		end

		nb.append_page(@darea, Gtk::Label.new("Draw"))
		nb.append_page(viewer, Gtk::Label.new("Txt"))

	end
	def set_num_file(int)
		@num_file = int
	end

	def file_done
		@file_done+=1
		update_label if @file_done % 100 == 0
	end

	def update_label
		done =@file_done.to_f / @num_file.to_f
		done *= 100
		@label.text = done.to_i.to_s+" %"
	end
	def read_data(filename)
		@result = Hash.new
		rtt = Array.new
		result = Array.new
		puts "File: "+filename
		puts filename.inspect
		file = File.new(filename)
		@viewer.buffer.text=file.read
		file.close
		file = File.new(filename)
		xmlreader = Nokogiri::XML::Reader.from_io(file)
		@tc = TestCase.new
		begin
			xmlreader.each do |l|
				# puts l.attributes
				#	 unless l.attribute("name") == nil
				case l.attribute("name") 
				when "Data received"
					@tc.add_read_frame(l.attribute("data"))
				when "Data sent"
					@tc.add_sent_frame(l.attribute("data"))
				when "No Packet Received"
					@tc.add_lost_frame
				when "Test Case Duration"
					@tc.duration= l.attribute("data")
				when "Report"
					@tc.add_report(l.attribute("data"))
				when "RTT Report"
					@tc.add_rtt(l.attribute("data"))
				else
					#do nothing
				end
			end
			#end
		rescue Nokogiri::SyntaxError 
			puts $!.inspect		
			puts $!.to_s+" at line:"+$!.line.to_s	
			exit -1
		rescue
			puts $!.inspect		
			#puts $!.methods
		end

		@result["Report"] = result
		@result["RTT"] = rtt
		#		a = Array.new
		#		a << result[0][-1]
		#		a << result[-2][-1]	
		file.close
	end

	private
	def get_file_chooser
		chooser_btt  = Gtk::FileChooserButton.new(
			"Choose your File", Gtk::FileChooser::ACTION_OPEN)
			chooser_btt.current_folder  = GLib.home_dir

			#chooser_btt.signal_connect('file_activated') do |w|
			chooser_btt.signal_connect('selection_changed') do |w|
				#			  file_changed(w, file_name_entry, rename_button)
				#			  chiamare il metodo per il caricamento del fil xml
				read_data(w.filename) unless w.filename == nil
				puts @tc.inspect unless @tc == nil
			end

			filter1 = Gtk::FileFilter.new
			filter1.name="XML Files"
			filter1.add_pattern('*.xml')
			chooser_btt.add_filter(filter1)

			return chooser_btt
	end

	def on_expose
		cr = @darea.window.create_cairo_context  
		alloc = @darea.allocation


		cr.set_source_rgb 0.2, 0.23, 0.9
		cr.rectangle 10, 15, 90, 60 # (x, y, width, height)
		cr.fill
		@darea.window.draw_line(@darea.style.fg_gc(@darea.state), 
										10+45, 15+60, 10+45, alloc.height )

		#        cr.set_source_rgb 0.9, 0.1, 0.1
		#        cr.rectangle 130, 15, 90, 60
		#        cr.fill

		cr.set_source_rgb 0.4, 0.9, 0.4
		cr.rectangle alloc.width-100, 15, 90, 60
		cr.fill
		@darea.window.draw_line(@darea.style.fg_gc(@darea.state), 
										alloc.width-55, 15+60, alloc.width-55, alloc.height )

		@tc.draw(@darea) unless @tc == nil

	end

end
window = XmlViewer.new
window.title="S.T.R.E.S.S. XML viewer"
window.set_size_request(800,600)
window.border_width= 10
#window.padding= 10
window.show_all
t = Thread.new{
	Gtk.main
}
t.join
