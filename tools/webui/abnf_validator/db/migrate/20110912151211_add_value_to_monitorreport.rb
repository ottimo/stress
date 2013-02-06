class AddValueToMonitorreport < ActiveRecord::Migration
  def self.up
	add_column :monitorreports, :value, :double
  end

  def self.down
	remove_column :monitorreports, :value
  end
end
