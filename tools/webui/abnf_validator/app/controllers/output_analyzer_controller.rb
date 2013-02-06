class OutputAnalyzerController < ApplicationController
	require 'lib/output_analyzer.rb'
	require 'som'
	require 'normalizer'
	
	#GET NEEDED INFORMATION FOR PLUGIN SYSTEM AND LIBRARIES
	def index
		@simulations = Simulation.find_all_by_username(session[:username])
		@plugins = Array.new
		xml_file_list = Dir.glob("app/*.xml")
		xml_file_list.each do |f|
			@plugins << Plugin.new(f)
		end
		@libraries = Library.find(:all)
		respond_to {|format| format.html}
	end
	
	def select_simulation
		@files = Dir.glob("results/#{params[:output]}/*")
		@files.sort!{|x,y| x.split("/").last <=> y.split("/").last }
		@simulation_selected=params[:output]
		sims = Simulation.find_all_by_output(@simulation_selected)
		@simulation = sims.first
		if @simulation != nil and @simulation.testcases.size == 0
			xmlparser = XMLOutputParser.new
			@files.each do |file|
				#simulation.testcases << Testcase.new(:filename => file)
				xmlparser.parse(@simulation, file)
			end
			@simulation.save
		end
		render :partial => "output_body"
		#render :update do |page|
         # page.replace_html "right_column", :partial => 'output_body'
        # end
	end
	
	def get_l_distance_graph
		@simulation_selected=params[:output]
		pos = params[:frames].to_i
		@simulation = Simulation.find_all_by_output(@simulation_selected).first
		build_l_distance(pos)
		render :partial => 'l_distance_graph'
	end
	
	def get_l_distance_menu
		@simulation_selected=params[:output]
		@simulation = Simulation.find_all_by_output(@simulation_selected).first
		build_list_read_frames
		render :partial => "l_distance"
	end
	
	def get_testcase_list
		@simulation_selected=params[:output]
		sims = Simulation.find_all_by_output(@simulation_selected)
		@simulation = sims.first
		@testcase = @simulation.testcases[params[:testcase_id].to_i] unless params[:testcase_id] == nil
		render :partial => "testcase_list"
	end
	
	def get_frames_list
		@testcase = Testcase.find(params[:testcase_id])
		render :partial => "frames_list" unless @testcase == nil
	end
	
	def get_frame_content
		@frame = Frame.find(params[:frame_id])
		render :partial => "frame_content" unless @frame == nil
	end
	
	def get_rtt_stats
		@simulation_selected=params[:output]
		sims = Simulation.find_all_by_output(@simulation_selected)
		@simulation = sims.first

		build_rtt_stats

		render :partial => "rtt_stats"
	end
	
	def show_loading
		render :partial => "loading"
	end
	
	def clean_cache
		@simulation_selected=params[:output]
		sims = Simulation.find_all_by_output(@simulation_selected)
		@simulation = sims.first
		@simulation.testcases.destroy_all
		session[:messages] = {:type => "ok", :msg => "Cache deleted!"}
		render :text => 'Cache for simulation '+@simulation_selected.to_s+' clean!'
	end
	
	def get_som_menu
		@simulation_selected=params[:output]
		@simulation = Simulation.find_all_by_output(@simulation_selected).first
		build_list_read_frames
		render :partial => "som_menu"
	end
	
	def get_som_predictor
		@simulation_selected=params[:output]
		pos = params[:frames].to_i
		sims = Simulation.find_all_by_output(@simulation_selected)
		@simulation = sims.first
		begin
		build_rtt_stats
		build_list_read_frames
		build_l_distance(pos)
		number_of_read_frame = @read_frames.index(@l_distance.first)
		
		@data = Array.new
		@data_indexes = Array.new
		
		if number_of_read_frame != nil
			@l_distance.each_index do |i|
				@data << [@l_distance[i].l_distance, @rtts[i][number_of_read_frame].value] unless @l_distance[i] == nil
				@data_indexes << i unless @l_distance[i] == nil
			end
		
			for n in 0...number_of_read_frame do
				build_l_distance(  @read_frames[n].position )
				@data_indexes.each do |i|
					@data[@data_indexes.index(i)] << @l_distance[i].l_distance unless @l_distance[i] == nil
					@data[@data_indexes.index(i)] << @rtts[i][n].value unless @l_distance[i] == nil
				end
			end
			puts @data.inspect
		
			min, max = Normalizer.find_min_and_max(@data)
			puts min.to_s+" "+max.to_s
			puts min.inspect
			puts max.inspect
			normalizer = Normalizer.new(:min => min, :max => max)
			@normalized_data = []
			@data.each do |n|
				@normalized_data << normalizer.normalize(n) unless n == nil
			end
			puts @normalized_data.inspect
			@normalized_data.each do |nd|
				nd.delete_if {|x| x.nan? }
			end
			@som = SOM.new(@normalized_data, :nodes => 3, :dimensions => 2+(number_of_read_frame*2))
			@som.train unless @normalized_data.size <= 1
		end
		rescue Exception
		
		end
		render :partial => "som_predictor"
	end
	
	def build_rtt_stats
		@number_of_rtt = @simulation.testcases.first.monitorreports.find_all_by_type("RTTReport").size
		@rtts = Array.new
		@simulation.testcases.each do |t|
			reports = t.monitorreports.find_all_by_type("RTTReport")
			@rtts[t.position] = reports unless reports == nil
		end
	end
	
	def build_l_distance(pos)
		@l_distance = Array.new
		first = @simulation.testcases.first
		@simulation.testcases.each do |testcase|
			@l_distance[testcase.position] = testcase.frames[pos] unless testcase.frames[pos] == nil or testcase.frames[pos].class == NoResponse
		end
	end
	
	def build_list_read_frames
		first = @simulation.testcases.first
		@read_frames = Array.new
		first.frames.each do |f|
			@read_frames << f if f.type == "ReadFrame" or f.type == "NoResponse"
		end
	end
	
	
	#SAVES AN ABNF FILE FROM FORM INFORMATION
#	def save_abnf_file
#		begin
#			file = AbnfFile.create_new_file(file_name, params[:library_id], session[:user_id])
#			abnf_content = "First arg: #{params[:first_arg]}, second_arg: #{params[:second_arg]}"
#			file.update_attributes(:content => abnf_content)
#			session[:messages] = {:type => "ok", :msg => "File Saved!"}
#		rescue Exceptions::MissingParameters
#			session[:messages] = {:type => "err", :msg => "Missing file name!"}
#		rescue Exceptions::InvalidLibraryId
#			session[:messages] = {:type => "err", :msg => "Invalid Library id!"}			
#		rescue
#			session[:messages] = {:type => "err", :msg => "Generic error, please contact administrator"}
#		end
#		render :text => "true"
#	end
	
end
