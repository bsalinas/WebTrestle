class Station < ActiveRecord::Base
  belongs_to :user
  has_many :sensor
  has_many :hardware_action
  has_many :state
  attr_accessible :description, :identifier, :name, :override
end
