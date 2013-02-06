class CreateAbnfFiles < ActiveRecord::Migration
  def self.up
    create_table :abnf_files do |t|
    	t.string :name
    	t.text :content
    	t.integer :creator
    	t.integer :library_id
      t.timestamps
    end
  end

  def self.down
    drop_table :abnf_files
  end
end
