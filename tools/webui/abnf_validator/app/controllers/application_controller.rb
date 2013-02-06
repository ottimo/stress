# Filters added to this controller apply to all controllers in the application.
# Likewise, all the methods added will be available for all controllers.

class ApplicationController < ActionController::Base
  helper :all # include all helpers, all the time
  protect_from_forgery # See ActionController::RequestForgeryProtection for details

	before_filter :authorize, :except => :login
			
	def is_admin
		user = User.find(session[:user_id])
		if !user.is_admin
			redirect_to :action => :access_denied
		end
	end
	
	def access_denied
	end
		
	def login
		user = User.find(:first, :conditions => [ "username = ?", params[:username] ])
		if user
			if user.password == params[:password]
				session[:autenticato] = true
				session[:user_id] = user.id
				session[:username] = user.username
				session[:library] = "all"
				session[:messages] = {}
				session[:is_admin] = user.is_admin
				redirect_to :action => :index
			else
				flash.now[:notice] = 'Password errata!'
		  end
		else
			if params[:username]
				flash.now[:notice] = 'Username inesistente!'
			end
		end
  end
  
  def index
  	user = User.find(session[:user_id])
		if user.is_admin
			redirect_to :action => :admin_index
		else
			redirect_to :action => :user_index
		end
	end
	  
  def logout
  	reset_session
  	redirect_to :controller => :users, :action => :index
	end

	def notify
		render :partial => "users/notify"
	end
	
	private
		def authorize
			unless session[:autenticato] == true
				redirect_to :action => :login
			end
		end

end
