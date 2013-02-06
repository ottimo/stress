class LibrariesController < ApplicationController
	
	def new_library
		render :partial => "new_library"
	end
	
	def delete
		@libraries = Library.all.select { |lib| lib.abnf_files.empty? }
		render :partial => "delete_library"
	end

	def create_library
		begin
			Library.create_new_library(params[:library], session[:user_id])
			session[:messages] = {:type => "ok", :msg => "New library created!"}
		rescue Exceptions::MissingParameters
			session[:messages] = {:type => "err", :msg => "Missing library name!"}	
		rescue Exceptions::LibraryAlreadyPresent
			session[:messages] = {:type => "err", :msg => "Library already present!"}
		rescue
			session[:messages] = {:type => "err", :msg => "Generic error, please contact administrator"}
		end
		render :partial => "new_library"
	end
		
	def delete_library
		begin
			Library.delete_library(params[:library_select])
			session[:messages] = {:type => "ok", :msg => "Library deleted!"}
		rescue Exceptions::LibraryNotEmpty
			session[:messages] = {:type => "err", :msg => "Library not empty"}
		rescue
			session[:messages] = {:type => "err", :msg => "Generic error, please contact administrator"}
		end
		redirect_to :action => :delete
	end
	
end
