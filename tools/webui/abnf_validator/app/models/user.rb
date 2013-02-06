class User < ActiveRecord::Base  
	has_many :access_rules
	has_many :abnf_files, :through => :access_rules

	def self.new_library(library_name)
		Dir.mkdir("#{library_name}")
	end

	def self.new_file(file_name)
		File.new(file_name, 'w')
	end	

	def self.upload_file(directory, upload)
		name =  upload['datafile'].original_filename
		path = File.join(directory, name)
		File.open(path, "wb") { |f| f.write(upload['datafile'].read) }
end

	def self.delete_file(file)
		File.delete(file)
	end

	def self.delete_library(library)
		Dir.rmdir(library)
	end

	def self.show_file(file_name)
		file = File.new("#{file_name}", "r")
		@file = file.read()
	end
	
	def self.update_file(file_name, value)
		File.open(file_name, "w") { |f| f.write(value) }
	end		

end
