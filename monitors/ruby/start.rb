#!/usr/bin/ruby

$LOAD_PATH.unshift File.expand_path(File.join(File.dirname(__FILE__)))
path = File.expand_path(File.join(File.dirname(__FILE__)))


require 'rubygems'
require 'daemons'

Daemons.run("#{path}/stressmonitor.rb")
