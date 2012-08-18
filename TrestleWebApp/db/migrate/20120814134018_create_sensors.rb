class CreateSensors < ActiveRecord::Migration
  def change
    create_table :sensors do |t|
      t.string :name
      t.string :description
      t.string :identifier
      t.string :units
      t.references :station
      t.boolean :active

      t.timestamps
    end
    add_index :sensors, :station_id
  end
end
