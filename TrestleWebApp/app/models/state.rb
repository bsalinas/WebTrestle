class State < ActiveRecord::Base
  belongs_to :station
  attr_accessible :current_state, :description, :identifier, :name
end
