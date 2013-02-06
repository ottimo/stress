class Fault < ActiveRecord::Base
	belongs_to :frame, :foreign_key => "frame_id"
end
