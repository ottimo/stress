class CreateMonitorreports < ActiveRecord::Migration
  def self.up
    create_table :monitorreports do |t|
	  t.string :type
	  
	  t.integer :testcase_id
	  
      t.timestamps
    end
  end

  def self.down
    drop_table :monitorreports
  end
end
