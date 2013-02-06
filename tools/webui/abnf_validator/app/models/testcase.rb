class Testcase < ActiveRecord::Base
	belongs_to :simulation
	has_many :frames, :order => :position, :dependent => :destroy
	has_many :monitorreports, :order => :position, :dependent => :destroy
	
	def printHTML
		out = "<b>"+self.position.to_s+"</b> -"
		out += " ID: "+self.id_anomaly.to_s
		out += " RUN: "+self.run.to_s
		return out
	end
end
