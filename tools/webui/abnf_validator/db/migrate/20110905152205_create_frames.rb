class CreateFrames < ActiveRecord::Migration
  def self.up
    create_table :frames do |t|
	  t.string :type
	  
	  t.integer :testcase_id
	  t.integer :position
	  
	  t.text	:data
	  
      t.timestamps
    end
  end

  def self.down
    drop_table :frames
  end
end
