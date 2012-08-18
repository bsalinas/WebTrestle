class Station < ActiveRecord::Base
  belongs_to :user
  has_many :sensor
  attr_accessible :description, :identifier, :name, :override
end
