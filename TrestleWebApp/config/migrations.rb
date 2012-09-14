# Migrations will run automatically. The DSL like wrapper syntax is courtesy 
# of sinatra-sequel
#
# For details on sequel's schema modifications, check out:
# http://sequel.rubyforge.org/rdoc/files/doc/schema_rdoc.html

migration "create the people table" do
  database.create_table :people do
    primary_key :id 
    string      :name
  end
end
migration "create stations" do
  database.create_table :stations do
    primary_key :id
    string      :name
    text        :description
    string      :identifier
    timestamp   :created_at
    timestamp   :updated_at
  end
end

migration "create sensors" do
  database.create_table :sensors do
    primary_key :id
    string      :name
    text        :description
    string      :identifier
    string      :units
    int         :station_id
    timestamp   :created_at
    timestamp   :updated_at
  end
end

migration "create_measurements" do
  database.create_table :measurements do
    primary_key   :id
    float         :value
    int           :sensor_id
    timestamp     :created_at
    timestamp     :updated_at
  end
end

migration "create_actions" do 
  database.create_table :hardware_actions do
    primary_key   :id
    string        :name
    text          :description
    boolean       :pending
    integer       :station_id
    string        :identifier
    integer       :hardware_id
    string        :message
    timestamp     :created_at
    timestamp     :updated_at
  end
end

migration "create_states" do
  database.create_table :states do
    primary_key :id
    string :name
    text :description
    int :station_id
    string :current_state
    string :identifier
    timestamp     :created_at
    timestamp     :updated_at
  end
end
