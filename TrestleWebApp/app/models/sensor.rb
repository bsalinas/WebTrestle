class Sensor < ActiveRecord::Base
  belongs_to :station
  has_many :measurement
  attr_accessible :description, :identifier, :name, :units, :active
end
