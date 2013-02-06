class CreateSimulations < ActiveRecord::Migration
  def self.up
    create_table :simulations do |t|
    	t.integer :pid
    	t.string :username
      t.string :filename
			t.string :output
      t.timestamps
    end
  end

  def self.down
    drop_table :simulations
  end
end
