class AddLDistanceToFrames < ActiveRecord::Migration
  def self.up
	add_column :frames, :l_distance, :integer
  end

  def self.down
	remove_column :frames, :l_distance
  end
end
