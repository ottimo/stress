class AddPositionToMonitorreport < ActiveRecord::Migration
  def self.up
	add_column :monitorreports, :position, :integer
  end

  def self.down
	remove_column :monitorreports, :position
  end
end
