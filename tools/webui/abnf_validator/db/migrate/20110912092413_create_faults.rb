class CreateFaults < ActiveRecord::Migration
  def self.up
    create_table :faults do |t|
	 
	  t.integer :number_of_value
	  t.integer :frame_id 
	  
	  t.string  :value
	  
      t.timestamps
    end
  end

  def self.down
    drop_table :faults
  end
end
