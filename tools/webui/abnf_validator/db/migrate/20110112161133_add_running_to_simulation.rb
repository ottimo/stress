class AddRunningToSimulation < ActiveRecord::Migration
  def self.up
	  add_column :simulations, :running, :boolean
  end

  def self.down
	  remove_column :simulations, :running
  end
end
