class WsdlsController < ApplicationController
	require 'lib/plugin.rb'
	require 'lib/easysoap.rb'
	
	def index
		@plugins = Array.new
		xml_file_list = Dir.glob("app/*.xml")
		xml_file_list.each do |f|
			@plugins << Plugin.new(f)
		end
		@libraries = Library.find(:all)
		respond_to {|format| format.html}
	end
	
	def get_wsdl_methods
		begin
			if params[:wsdl_uri]			
				session[:wsdl_uri] = params[:wsdl_uri]
			else
				session[:wsdl_uri] = "http://lillo.lilik.it/main.wsdl"
			end
			params[:add] ? session[:select_number] += 1 : session[:select_number] = 0
			methods = Wsdl_file.new(session[:wsdl_uri]).wsdl_methods
			session[:wsdl_methods] = Array.new
			session[:wsdl_methods] << "- Select a method -"
			session[:wsdl_methods] += methods			
		rescue Exceptions::MissingParameters
			session[:messages] = {:type => "err", :msg => "Missing wsdl uri!"}
		rescue
			session[:messages] = {:type => "err", :msg => "Generic error, please contact administrator"}
		end
		session[:messages] = {:type => "ok", :msg => "Wsdl found"}
		render :partial => "wsdl_methods"
	end
	
	def get_counter
		render :text => session[:select_number]
	end
	
	def render_method_selection
		params[:add] ? session[:select_number] += 1 : session[:select_number] = 0
		render :partial => "wsdl_methods"
	end
	
	# PRENDE COME PARAMETRO L'URI DEL WSDL, CREA L'OGGETTO E CHIAMA SHOW_METHODS_AND_ARGS
	def get_wsdl_content
		wsdl = Wsdl_file.new(session[:wsdl_uri])	
		show_methods_and_args(wsdl, params[:method])
	end
	
	# CREA UN OGGETTO WSDL_FILE CON UN URI DI DEFAULT PER I TEST
	def get_default_wsdl
		#@wsdl = Wsdl_file.new("http://www.webservicex.net/CurrencyConvertor.asmx?wsdl")
		#session[:wsdl_uri] = "http://harbormist.com/webservice/WebBackImage/PictureService.asmx?WSDL"
		session[:wsdl_uri] = "http://lillo.lilik.it/test.wsdl"
		wsdl = Wsdl_file.new(session[:wsdl_uri])
		params[:method] ? method = params[:method].strip : method  = nil
		session[:messages] = {:type => "ok", :msg => "Wsdl found"}
		show_methods_and_args(wsdl, method)
	end
	
	#SALVA UN FILE ABNF DA I FORM GENERATI CON WSDL
	def save_abnf_from_wsdl
		begin
			methods = Hash.new
			params.keys.each do |p|
				if p.include?("method_")
					methods[p.split("_")[1]] = Hash.new
				end
			end
			params.keys.each do |p|
				if p.include?("arg_")
					methods[p.split("_")[1]][p.split("_")[2]] = params[p]
				end
			end
			methods.keys.each do |method|
				file_name = params[:file_name] + "_" + method
				file = AbnfFile.create_new_file(file_name, params[:library_id], session[:user_id])
				abnf_content = "
root = packet-2 read read

read = <%TCP_read%> %d0

; packet-1 = <read or send>
packet-2 = <%TCP_send%> HypertextTransferProtocol-2 eXtensibleMarkupLanguage-2

HypertextTransferProtocol-2 =  HypertextTransferProtocolTextItem-2 ContentType-2 HypertextTransferProtocolTextItem1-2 Accept-2 UserAgent-2 CacheControl-2 HypertextTransferProtocolTextItem2-2 Host-2 Connection-2 ContentLength-2 HypertextTransferProtocolTextItem3-2

HypertextTransferProtocolTextItem-2 =   \"POST /StressWSService/StressWS HTTP/1.1\" %x0d0a ;POST /StressWSService/StressWS HTTP/1.1\r\n

ContentType-2 =   \"Content-type: text/xml;charset=utf-8\" %x0d0a ;Content-type: text/xml;charset=utf-8\r\n

HypertextTransferProtocolTextItem1-2 =   \"Soapaction: \" %x22 \"#{session[:wsdl_endpoint]}/#{method}Request\" %x220d0a ;Soapaction: \"#{session[:wsdl_endpoint]}/#{method}Request\"\r\n

Accept-2 =   \"Accept: text/xml, multipart/related\" %x0d0a ;Accept: text/xml, multipart/related\r\n

UserAgent-2 =   \"User-Agent: JAX-WS RI 2.2-hudson-752-\" %x0d0a ;User-Agent: JAX-WS RI 2.2-hudson-752-\r\n

CacheControl-2 =   \"Cache-Control: no-cache\" %x0d0a ;Cache-Control: no-cache\r\n

HypertextTransferProtocolTextItem2-2 =   \"Pragma: no-cache\" %x0d0a ;Pragma: no-cache\r\n

Host-2 =   \"Host: localhost:8080\" %x0d0a ;Host: localhost:8080\r\n

Connection-2 =   \"Connection: keep-alive\" %x0d0a ;Connection: keep-alive\r\n

ContentLength-2 =   \"Content-Length: 219\" %x0d0a ;Content-Length: 219\r\n

HypertextTransferProtocolTextItem3-2 =  %x0d0a ;\r\n

eXtensibleMarkupLanguage-2 =  xmlXmlpiXml-2 Tag-2

xmlXmlpiXml-2 =   \"<?xml version=\'1.0\' encoding=\'UTF-8\'?>\"  ;<?xml

Tag-2 =   \"<S:Envelope xmlns:S=\" %x22 \"http://schemas.xmlsoap.org/soap/envelope/\" %x22 \"><S:Body><ns2:#{method} xmlns:ns2=\" %x22 \"#{session[:wsdl_targetnamespace]}\" %x22 \"><#{methods[method].keys}>#{methods[method].values}</#{methods[method].keys}></ns2:#{method}></S:Body></S:Envelope>\"  ;<S:Envelope>\r\n"
				file.update_attributes(:content => abnf_content)
			end
			if methods.keys.length > 1
				session[:messages] = {:type => "ok", :msg => "Files Saved!"}
			else
				session[:messages] = {:type => "ok", :msg => "File Saved!"}
			end
		rescue Exceptions::MissingParameters
			session[:messages] = {:type => "err", :msg => "Missing file name!"}
		rescue Exceptions::InvalidLibraryId
			session[:messages] = {:type => "err", :msg => "Invalid Library id!"}			
		rescue
			session[:messages] = {:type => "err", :msg => "Generic error, please contact administrator"}
		end
		render :text => "true"
	end
		
	private
	
	# FA IL PARSING DEGLI ARGOMENTI DEI VARI METODI, PRENDE COME PARAMETRI UN OGGETTO DI TIPO Wsdl_file E  
	# IL METODO CON CUI FILTRARE
	def show_methods_and_args(wsdl, method_selected = nil)
		session[:wsdl_endpoint] = wsdl.wsdl_endpoint
		session[:wsdl_targetnamespace] = wsdl.targetnamespace
		@wsdl_methods = wsdl.wsdl_methods
		@wsdl_method_selected = method_selected
#		@wsdl_content[method] = @wsdl_methods
#				#@wsdl_content[method] << "<h4>Method: </h4><i>Name: </i>" + method
#				next_is_method = false
#			elsif next_is_arg
#				l.index("}") ? @wsdl_content[method] <<  l[l.index("}")+1, l.length] : @wsdl_content[method] <<  l
#				next_is_arg = false
#			end
#			next_is_method = true if l =~ /.SYNOPSIS/
#			next_is_arg = true if l =~ /.ARGS/
#		end
#		if method_selected
#			@wsdl_content_filtred[method_selected.strip] = @wsdl_content[method_selected.strip]
#			#render :text => @wsdl_content.inspect + "pippoooooooo" + method_selected and return
#		else
#			@wsdl_content_filtred = @wsdl_content
#		end
		render :partial => "wsdl_content"
		#render :text => @wsdl_content_filtred.inspect
	end
	
end
