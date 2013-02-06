class UsersController < ApplicationController
	before_filter :is_admin, :only => [ :admin_index, :create, :delete_user ]
	require "lib/unzip.rb"
	require 'lib/plugin.rb'
	
	def admin_index
		@users = User.find(:all)
		@user = User.new
		@plugins = Array.new
		xml_file_list = Dir.glob("app/*.xml")
		xml_file_list.each do |f|
			@plugins << Plugin.new(f)
		end			
	end

	def create
		@user = User.new
		@user.update_attributes(params[:user])
		redirect_to :action => :index
	end

	def delete_user
		User.delete(params[:id])
		redirect_to :action => :index
	end
	
	def upload_plugin
		begin
			uploaded_file = params[:upload]
			#File.open("tmp/upload.zip", "wb") { |f| f.write(uploaded_file.read) }
			file = UnZipThemAll.new(uploaded_file, RAILS_ROOT)
			session[:migration] = file.unzip
			@current_dir = system("touch #{RAILS_ROOT}/tmp/restart.txt")
			flash[:notice] = "Plugin installed"
		rescue Exceptions::MissingParameters
			flash[:notice] = "You have to select a file"
		rescue Exceptions::NotZipFile
			flash[:notice] = "Not a zip file"
		rescue Exceptions::MalformedPlugin
			flash[:notice] = "Malformed plugin"
		#rescue
		#	flash[:notice] = "Generic error, please contact administrator"
		end		
		redirect_to :action => :index			
	end
	
	def run_migration
		system("cd #{RAILS_ROOT}/db/migrate && rake db:migrate")
		redirect_to :action => :index
	end
	
	def delete_plugin
		begin
			plugin = Plugin.new(params[:plugin_xml_file_name])
			plugin.delete_plugin
			flash[:notice] = "Plugin deleted, now you have to cancel database tables"
		rescue
			flash[:notice] = "Generic error, please contact administrator"
		end
		redirect_to :action => :index
	end
	
	def user_index
		@user = User.find(session[:user_id])
		@files = @user.abnf_files.find(:all, :order => "library_id")
		@libraries = Library.find(:all)
		@plugins = Array.new
		xml_file_list = Dir.glob("app/*.xml")
		xml_file_list.each do |f|
			@plugins << Plugin.new(f)
		end
	end
		
	def show_file
		@file_name = "#{session[:user_dir]}/#{params[:file_name]}"
		@file = User.show_file(@file_name)
		render :partial => "editor"
	end

	def get_files_list
		if params[:library]
			session[:library] = params[:library]
		end
		@user = User.find(session[:user_id])
		if session[:library] != "all"
			@files = @user.abnf_files.find(:all, :conditions => {:library_id => session[:library]})
		else
			@files = @user.abnf_files.find(:all, :order => "library_id")
		end
		@libraries = Library.find(:all)
		render :partial => "files_list"
	end
	
	def get_libraries_list
		@libraries = Library.find(:all)
		render :partial => "libraries_list"
	end
	##svnsangiorgipasswd
	#sangiorgi

end
