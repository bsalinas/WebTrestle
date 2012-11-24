# Migrations will run automatically. The DSL like wrapper syntax is courtesy 
# of sinatra-sequel
#
# For details on sequel's schema modifications, check out:
# http://sequel.rubyforge.org/rdoc/files/doc/schema_rdoc.html


Sequel.migration do
  up do
    create_table :stations do
      primary_key :id
      text      :name
      text        :description
      text      :identifier
      timestamp   :created_at
      timestamp   :updated_at
    end

    create_table :sensors do
      primary_key :id
      text      :name
      text        :description
      text      :identifier
      text      :units
      integer         :station_id
      timestamp   :created_at
      timestamp   :updated_at
    end

    create_table :measurements do
      primary_key   :id
      decimal         :value
      integer           :sensor_id
      timestamp     :created_at
      timestamp     :updated_at
    end


    create_table :hardware_actions do
      primary_key   :id
      text        :name
      text          :description
      boolean       :pending
      integer       :station_id
      text        :identifier
      integer       :hardware_id
      text        :message
      timestamp     :created_at
      timestamp     :updated_at
    end


    create_table :states do
      primary_key :id
      text :name
      text :description
      integer :station_id
      text :current_state
      text :identifier
      timestamp     :created_at
      timestamp     :updated_at
    end
  end
  down do
    drop_table(:stations, :actions, :accounts, :users, :cascade=>true)
  end
end

