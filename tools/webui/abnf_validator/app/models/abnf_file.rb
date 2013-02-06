class AbnfFile < ActiveRecord::Base
	has_many :access_rules
	has_many :users, :through => :access_rules
	belongs_to :library

	def self.create_new_file(file_name, library_id, user_id)
		raise Exceptions::MissingParameters if file_name.size < 1
		raise Exceptions::InvalidLibraryId unless Library.exists?(library_id)
		file = AbnfFile.new(:name => file_name, :library_id => library_id, :creator => user_id, :created_at => Time.now)
		file.save!
		AccessRule.create!(:user_id => user_id, :abnf_file_id => file.id, :permissions => "w")
		return file
	end
	
	def self.import(upload, library, user_id)
		name =  upload['datafile'].original_filename
		content = upload['datafile'].read
		file = AbnfFile.new(:name => name, :library_id => library, :content => content, :creator => user_id, :created_at => Time.now)
		file.save!
		AccessRule.create!(:user_id => user_id, :abnf_file_id => file.id, :permissions => "w")
		return file
	end

	def self.delete_file(file_id, user_id)
		raise Exceptions::MissingParameters if file_id.size < 1
		raise Exceptions::NotOwner	unless AbnfFile.find(file_id).creator == user_id
		file = AbnfFile.find(file_id)
		AbnfFile.delete(file)
		file.access_rules.destroy_all
		return true
	end

	def self.unshare_file(user_id, file_id)
		raise Exceptions::MissingParameters if file_id.size < 1 or user_id.size < 1
		raise Exceptions::NotSharer unless AbnfFile.find(file_id).creator != user_id
		access_rule = AccessRule.find(:all, :conditions => {:user_id => user_id, :abnf_file_id => file_id})
		AccessRule.delete(access_rule)
		return true
	end

	def self.rename_file(file_id, file_name)
		raise Exceptions::MissingParameters if file_id.size < 1 or file_name.size < 1
		raise Exceptions::FileNotFound unless AbnfFile.exists?(file_id)
		file = AbnfFile.find(file_id)
		file.update_attributes(:name => file_name)
		return true
	end

	def self.share_file(user_id, file_id, permissions)
		raise Exceptions::MissingParameters if user_id.size < 1 or file_id.size < 1 or permissions.size < 1
		raise Exceptions::FileAlreadyShared if AccessRule.exists?(:user_id => user_id, :abnf_file_id => file_id)
		raise Exceptions::InvalidPermission if permissions != "w" or permissions != "r"
		AccessRule.create!(:user_id => user_id, :abnf_file_id => file_id, :permissions => permissions)
		return true
	end

	def self.copy_file(file_id, new_file_name, library_id, user_id)
		raise Exceptions::MissingParameters if new_file_name.size < 1 or file_id.size < 1 or library_id.size < 1 or user_id.size < 1
		raise Exceptions::FileNotFound unless AbnfFile.exists?(file_id)
		raise Exceptions::InvalidLibraryId unless Library.exists?(library_id)
		file = AbnfFile.find(file_id)
		file_copy = AbnfFile.create!(file.attributes.merge({:name => new_file_name, :library_id => library_id, :creator => user_id, :created_at => Time.now, :updated_at => Time.now}))
		AccessRule.create!(:user_id => user_id, :abnf_file_id => file_copy.id, :permissions => "w")
		return file_copy
	end

	def self.move_file(file_id, library_id)
		raise Exceptions::MissingParameters if file_id.size < 1 or library_id.size < 1
		raise Exceptions::FileNotFound unless AbnfFile.exists?(file_id)
		raise Exceptions::InvalidLibraryId unless Library.exists?(library_id)
		file = AbnfFile.find(file_id)
		file.update_attributes(:library_id => library_id)
		return true
	end		
	
end
