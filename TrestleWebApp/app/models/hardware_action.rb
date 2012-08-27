class HardwareAction < ActiveRecord::Base
  belongs_to :station
  attr_accessible :description, :identifier, :name, :pending, :message
end
