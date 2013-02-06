class Monitorreport < ActiveRecord::Base
	belongs_to :testcase
end

class RTTReport < Monitorreport
end

class SanityCheck < Monitorreport
end

class LoadMonitor < Monitorreport
end
