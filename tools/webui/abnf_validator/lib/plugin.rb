class Plugin
require 'rubygems'
require 'rexml/document'

attr_accessor :plugin, :xml_file

	def initialize(xml)
		@xml_file = xml
		@plugin = Hash.new
		xmlfile = File.new(xml)
		doc = REXML::Document.new(xmlfile)
		doc.root.each_element do |field|
			@plugin[field.name] = field.text
		end
	end
	
	def delete_plugin
		xmlfile = File.new(xml_file)
		doc = REXML::Document.new(xmlfile)
		doc.root.each_element do |field|
			if field.name == "extra_file"
				File.delete(field.text)
			end
		end
		File.delete(self.xml_file)
		File.delete("app/controllers/#{self.plugin['suffix']}_controller.rb")
		File.delete("app/models/#{self.plugin['suffix']}.rb")
		FileUtils.remove_dir("app/views/#{self.plugin['suffix']}/")
		File.delete("app/views/layouts/#{self.plugin['suffix']}.html.erb")
	end

end
