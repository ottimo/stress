class CreateTestcases < ActiveRecord::Migration
  def self.up
    create_table :testcases do |t|
	  t.integer :simulation_id
	  
	  # validare la presenza di questi attributi
	  t.string :filename
	  t.integer :id_anomaly
	  t.integer :run
	  
      t.timestamps
    end
  end

  def self.down
    drop_table :testcases
  end
end
