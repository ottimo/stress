class CreateLibraries < ActiveRecord::Migration
  def self.up
    create_table :libraries do |t|
    	t.string :name
    	t.integer :creator
      t.timestamps
    end
    Library.create :name => "No library", :creator => 1
  end

  def self.down
    drop_table :libraries
  end
end
