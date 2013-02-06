class AbnfFilesController < ApplicationController
	require 'open3'
	require 'lib/plugin.rb'

	#INDEX OF EDITOR/VALIDATOR, CALLED WITH CLICK ON FILE NAME
	def index
		@file = AbnfFile.find(params[:id])
		session[:editor_filename] = @file.name
		@permissions = @file.access_rules.find(:first, :conditions => {:user_id => session[:user_id]}).permissions
		@plugins = Array.new
		xml_file_list = Dir.glob("app/*.xml")
		xml_file_list.each do |f|
			@plugins << Plugin.new(f)
		end
		respond_to {|format| format.html}
	end

	#CALLED FROM HOMEPAGE, WITH CLICK ON "NEW FILE"
	def new_file
		@libraries = Library.find(:all)
		render :partial => "new_file"
	end

	#CALLED FROM HOMEPAGE, WITH CLICK ON "NEW FILE"
	def import_file
		@libraries = Library.find(:all)
		render :partial => "import_file"
	end

	#CALLED WHEN A NEW FILE IS IMPORTED, FROM HOMEPAGE --> IMPORT
	def import_new_file
    post = AbnfFile.import(params[:upload], params[:library_id], session[:user_id])
    redirect_to :controller => "users", :action => "index"
  end

	#CALLED WHEN A NEW FILE IS CREATED, FROM HOMEPAGE --> NEW FILE
	def create_file
		begin
			AbnfFile.create_new_file(params[:file_name], params[:library_id], session[:user_id])
			session[:messages] = {:type => "ok", :msg => "New file created!"}
		rescue Exceptions::MissingParameters
			session[:messages] = {:type => "err", :msg => "Missing file name!"}
		rescue Exceptions::InvalidLibraryId
			session[:messages] = {:type => "err", :msg => "Library id not valid!"}
		rescue
			session[:messages] = {:type => "err", :msg => "Generic error, please contact administrator"}
		end
		redirect_to :action => "new_file"
	end

	#CALLED FROM HOMEPAGE, WITH CLICK ON DELETE "X" BUTTON, AVAIABLE ONLY FOR FILE'S OWNER
	def delete_file
		begin
			AbnfFile.delete_file(params[:id], session[:user_id])
			session[:messages] = {:type => "ok", :msg => "File deleted!"}
		rescue Exceptions::MissingParameters
			session[:messages] = {:type => "err", :msg => "Missing file id"}
		rescue Exceptions::NotOwner
			session[:messages] = {:type => "err", :msg => "Only owner can delete files!"}
		rescue
			session[:messages] = {:type => "err", :msg => "Generic error, please contact administrator"}
		end
		redirect_to :controller => :users, :action => :get_files_list
	end
	
	#CALLED FROM HOMEPAGE, WITH CLICK ON UNSHARE "X" BUTTON, AVAIABLE ONLY FOR FILE'S SHARER
	def unshare_file
		begin
			AbnfFile.unshare_file(session[:user_id], params[:id])
			session[:messages] = {:type => "ok", :msg => "File unshared!"}
		rescue Exceptions::MissingParameters
			session[:messages] = {:type => "err", :msg => "Missing file id"}
		rescue Exceptions::NotSharer
			session[:messages] = {:type => "err", :msg => "You cannot unshare an owned file!"}
		rescue
			session[:messages] = {:type => "err", :msg => "Generic error, please contact administrator"}
		end			
		redirect_to :controller => :users, :action => :get_files_list
	end

	#############################################################################################
	# MODIFY PROPERTIES OF FILE PART, ALL DERIVED FROM CLICK ON MODIFY "M" BUTTON FROM HOMEPAGE #
	#############################################################################################

	#RENDER THE MODIFY_FILE_PROPERTIES PARTIAL ON RIGHT COLUMN
	def modify_file_properties
		#SETTING COMMON VARIABLES TO MANY ACTIONS IN THIS CONTROLLER
		settings_variables(params[:file_id])
		render :partial => "modify_file_properties"
	end

	#RENAME FILE, CALLED WITH CLICK ON "MODIFY_FILE_PROPERTIES" ---> "RENAME FILE"
	def rename_file
		begin
			AbnfFile.rename_file(params[:file_id], params[:file_name])
			session[:messages] = {:type => "ok", :msg => "File renamed!"}
		rescue Exceptions::MissingParameters
			session[:messages] = {:type => "err", :msg => "Missing file name or id"}
		rescue Exceptions::FileNotFound
			session[:messages] = {:type => "err", :msg => "File not found"}
		rescue
			session[:messages] = {:type => "err", :msg => "Generic error, please contact administrator"}
		end
		settings_variables(params[:file_id])
		render :partial => "modify_file_properties"
	end

	#SHARE FILE WITH OTHER USERS, CALLED WITH CLICK ON "MODIFY_FILE_PROPERTIES" ---> "SHARE NOW"
	def add_access_rule
		begin
			permissions = (params[:permissions])? params[:permissions] : "r"
			AbnfFile.share_file(params[:user], params[:file], permissions)
			session[:messages] = {:type => "ok", :msg => "File shared!"}
		rescue Exceptions::MissingParameters
			session[:messages] = {:type => "err", :msg => "Missing some parameters"}
		rescue Exceptions::FileAlreadyShared
			session[:messages] = {:type => "err", :msg => "Selected user already share this file"}
		rescue Exceptions::InvalidPermission
			session[:messages] = {:type => "err", :msg => "Invalid permission type"}
		rescue
			session[:messages] = {:type => "err", :msg => "Generic error, please contact administrator"}
		end
		settings_variables(params[:file])
		render :partial => "modify_file_properties"
	end

	#COPY FILE, CALLED WITH CLICK ON "MODIFY_FILE_PROPERTIES" ---> "COPY FILE"
	def copy_file
		begin
			file_copy = AbnfFile.copy_file(params[:file_id], params[:new_file_name], params[:library], session[:user_id])
			session[:messages] = {:type => "ok", :msg => "File copied!"}
		rescue Exceptions::MissingParameters
			session[:messages] = {:type => "err", :msg => "Missing some parameters!"}
		rescue Exceptions::InvalidLibraryId
			session[:messages] = {:type => "err", :msg => "Invalid library id!"}
		rescue Exceptions::FileNotFound
			session[:messages] = {:type => "err", :msg => "File not found!"}
		rescue
			session[:messages] = {:type => "err", :msg => "Generic error, please contact administrator"}
		end
		if file_copy
			settings_variables(file_copy.id)
		else
			settings_variables(params[:file_id])
		end
		render :partial => "modify_file_properties"
	end

	#MOVE FILE TO ANOTHER LIBRARY, CALLED WITH CLICK ON "MODIFY_FILE_PROPERTIES" ---> "MOVE FILE"
	def move_file
		begin
			AbnfFile.move_file(params[:file_id], params[:library])
			session[:messages] = {:type => "ok", :msg => "File moved!"}
		rescue Exceptions::MissingParameters
			session[:messages] = {:type => "err", :msg => "Missing some parameters!"}
		rescue Exceptions::InvalidLibraryId
			session[:messages] = {:type => "err", :msg => "Invalid library id!"}
		rescue Exceptions::FileNotFound
			session[:messages] = {:type => "err", :msg => "File not found!"}
		rescue
			session[:messages] = {:type => "err", :msg => "Generic error, please contact administrator"}
		end
		settings_variables(params[:file_id])
		render :partial => "modify_file_properties"
	end

	########################################################################################################
	# ALL THE ACTIONS THAT CONCERNS WITH "EDITOR" PART OF THE APPLICATION, DERIVED FROM CLICK ON FILE NAME #
	########################################################################################################

	#ACTION CALLED WITH CLICK ON "SAVE CHANGES" AND "ABANDON CHANGES", IT SAVES THE FILE OR RELOADS FILE CONTENT
	def modify_or_reload_content
		raise Exceptions::MissingParameters if params[:file_id].size < 1
		raise Exceptions::FileNotFound unless AbnfFile.exists?(params[:file_id])
		begin
			if params[:editor_textarea]
				file = AbnfFile.find(params[:file_id])
				file.update_attributes(:content => params[:editor_textarea])
				session[:messages] = {:type => "ok", :msg => "File saved!"}
			else
				session[:messages] = {:type => "ok", :msg => "File reloaded!"}
			end
		rescue Exceptions::MissingParameters
			session[:messages] = {:type => "err", :msg => "Missing file id!"}
		rescue Exceptions::FileNotFound
			session[:messages] = {:type => "err", :msg => "File not found!"}
		rescue
			session[:messages] = {:type => "err", :msg => "Generic error, please contact administrator"}
		end
		@file = AbnfFile.find(params[:file_id])
		@permissions = @file.access_rules.find(:first, :conditions => {:user_id => session[:user_id]}).permissions
		render :partial => "editor"
	end

	def validate
		File.open("script/temp", "w") {|f| f.write(params[:content])}
		stdin, stdout, stderr = Open3.popen3('script/parseabnf -i script/temp')
		@result = stderr.read
		render :partial => "results"
	end
	
	def export
		send_data(params[:content], :filename => params[:file_name], :type => "text")
	end

	private

		# FUNCTION THAT SETS COMMON VARIABLES FOR MODIFY_FILE_PROPERTIES RENDERING
		def settings_variables(file_id)
			@file = AbnfFile.find(file_id)
			@libraries = Library.find(:all)
			@access_rules = @file.access_rules.find(:first, :conditions => {:user_id => session[:user_id]})
			@users = User.find(:all, :conditions => ['id not in (select user_id from access_rules as a where a.abnf_file_id = ?)', file_id]);			
		end
		
	
end
