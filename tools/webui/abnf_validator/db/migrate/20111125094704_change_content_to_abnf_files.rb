class ChangeContentToAbnfFiles < ActiveRecord::Migration
  def self.up
	change_column :abnf_files, :content, :text, :limit => 4294967295
  end

  def self.down
	change_column :abnf_files, :content, :text
  end
end
