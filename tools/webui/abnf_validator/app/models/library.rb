class Library < ActiveRecord::Base
	has_many :abnf_files
	
	def self.create_new_library(library_name, user_id)
		raise Exceptions::MissingParameters if library_name.size < 1
		raise Exceptions::LibraryAlreadyPresent if Library.exists?(:name => library_name)
		library = Library.create!(:name => library_name, :creator => user_id)
		return library
	end

	def self.delete_library(library_id)
		raise Exceptions::LibraryNotEmpty if Library.find(library_id).abnf_files.length() > 0		
		library = Library.find(library_id)
		Library.delete(library)
		return true			
	end
	
end
