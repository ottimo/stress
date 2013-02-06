class Wsdl_file
require 'rubygems'
require 'savon'

attr_accessor :wsdl_content, :wsdl_methods, :wsdl_endpoint, :targetnamespace
	
	def initialize(wsdl_uri)
		raise Exceptions::MissingParameters if wsdl_uri.empty?
		client = Savon::Client.new do
			wsdl.document = wsdl_uri
		end
		@targetnamespace = client.wsdl.namespace
		@wsdl_endpoint = client.wsdl.endpoint.to_s
		#@wsdl_content = client.dump_method_signatures
		@wsdl_methods = client.wsdl.operations.keys
	end
			
end

