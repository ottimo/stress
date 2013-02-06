class AddTriggerToSetPid < ActiveRecord::Migration
  def self.up
	execute	"create trigger update_simulations_pid after update on bj_job for each row update simulations set pid = new.pid	where output = new.tag"
  end

  def self.down
	execute "drop trigger update_simulations_pid"
  end
end
