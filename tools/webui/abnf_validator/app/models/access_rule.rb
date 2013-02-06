class AccessRule < ActiveRecord::Base
	belongs_to :user
	belongs_to :abnf_file
end
