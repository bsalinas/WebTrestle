class CreateStates < ActiveRecord::Migration
  def change
    create_table :states do |t|
      t.string :name
      t.string :description
      t.references :station
      t.string :current_state
      t.string :identifier

      t.timestamps
    end
    add_index :states, :station_id
  end
end
