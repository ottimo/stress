class AddPositionToTestcase < ActiveRecord::Migration
  def self.up
	add_column :testcases, :position, :integer
  end

  def self.down
	remove_column :testcases, :position
  end
end
