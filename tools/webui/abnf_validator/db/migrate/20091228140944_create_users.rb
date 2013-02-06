class CreateUsers < ActiveRecord::Migration
  def self.up
    create_table :users do |t|
      t.string :username
      t.string :password
      t.boolean :is_admin
      t.timestamps
    end
    User.create :username => "admin", :password => "admin", :is_admin => true
  end

  def self.down
    drop_table :users
  end
end
