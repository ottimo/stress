# This file is auto-generated from the current state of the database. Instead of editing this file, 
# please use the migrations feature of Active Record to incrementally modify your database, and
# then regenerate this schema definition.
#
# Note that this schema.rb definition is the authoritative source for your database schema. If you need
# to create the application database on another system, you should be using db:schema:load, not running
# all the migrations from scratch. The latter is a flawed and unsustainable approach (the more migrations
# you'll amass, the slower it'll run and the greater likelihood for issues).
#
# It's strongly recommended to check this file into your version control system.

ActiveRecord::Schema.define(:version => 20111125094704) do

  create_table "abnf_files", :force => true do |t|
    t.string   "name"
    t.text     "content",    :limit => 2147483647
    t.integer  "creator"
    t.integer  "library_id"
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  create_table "access_rules", :force => true do |t|
    t.integer "user_id"
    t.integer "abnf_file_id"
    t.string  "permissions"
  end

  create_table "bj_config", :primary_key => "bj_config_id", :force => true do |t|
    t.text "hostname"
    t.text "key"
    t.text "value"
    t.text "cast"
  end

  create_table "bj_job", :primary_key => "bj_job_id", :force => true do |t|
    t.text     "command"
    t.text     "state"
    t.integer  "priority"
    t.text     "tag"
    t.integer  "is_restartable"
    t.text     "submitter"
    t.text     "runner"
    t.integer  "pid"
    t.datetime "submitted_at"
    t.datetime "started_at"
    t.datetime "finished_at"
    t.text     "env"
    t.text     "stdin"
    t.text     "stdout"
    t.text     "stderr"
    t.integer  "exit_status"
  end

  create_table "bj_job_archive", :primary_key => "bj_job_archive_id", :force => true do |t|
    t.text     "command"
    t.text     "state"
    t.integer  "priority"
    t.text     "tag"
    t.integer  "is_restartable"
    t.text     "submitter"
    t.text     "runner"
    t.integer  "pid"
    t.datetime "submitted_at"
    t.datetime "started_at"
    t.datetime "finished_at"
    t.datetime "archived_at"
    t.text     "env"
    t.text     "stdin"
    t.text     "stdout"
    t.text     "stderr"
    t.integer  "exit_status"
  end

  create_table "faults", :force => true do |t|
    t.integer  "number_of_value"
    t.integer  "frame_id"
    t.string   "value"
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  create_table "frames", :force => true do |t|
    t.string   "type"
    t.integer  "testcase_id"
    t.integer  "position"
    t.text     "data"
    t.datetime "created_at"
    t.datetime "updated_at"
    t.integer  "l_distance"
  end

  create_table "libraries", :force => true do |t|
    t.string   "name"
    t.integer  "creator"
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  create_table "monitorreports", :force => true do |t|
    t.string   "type"
    t.integer  "testcase_id"
    t.datetime "created_at"
    t.datetime "updated_at"
    t.integer  "position"
    t.float    "value"
  end

  create_table "simulations", :force => true do |t|
    t.integer  "pid"
    t.string   "username"
    t.string   "filename"
    t.string   "output"
    t.datetime "created_at"
    t.datetime "updated_at"
    t.boolean  "running"
  end

  create_table "testcases", :force => true do |t|
    t.integer  "simulation_id"
    t.string   "filename"
    t.integer  "id_anomaly"
    t.integer  "run"
    t.datetime "created_at"
    t.datetime "updated_at"
    t.integer  "position"
  end

  create_table "users", :force => true do |t|
    t.string   "username"
    t.string   "password"
    t.boolean  "is_admin"
    t.datetime "created_at"
    t.datetime "updated_at"
  end

end
