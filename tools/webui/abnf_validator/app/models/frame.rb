class Frame < ActiveRecord::Base
	belongs_to :testcase
	#acts_as_list :scope => :testcase
	has_many :faults, :dependent => :destroy, :foreign_key => "frame_id"
	def printHTML
		out = self.position.to_s+": "
		out+= "<b>"+self.type+"</b>: "
		
		return out
	end
end

class ReadFrame < Frame

end

class SentFrame < Frame
	

end

class NoResponse < ReadFrame

end
