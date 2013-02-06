class CreateAccessRules < ActiveRecord::Migration
  def self.up
		create_table :access_rules do |t|
 		 	t.integer :user_id
 		 	t.integer :abnf_file_id
  			t.string :permissions
  	end
  end

  def self.down
  end
end
