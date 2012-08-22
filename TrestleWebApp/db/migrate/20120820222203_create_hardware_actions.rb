class CreateHardwareActions < ActiveRecord::Migration
  def change
    create_table :hardware_actions do |t|
      t.string :name
      t.string :description
      t.boolean :pending
      t.references :station
      t.string :identifier
      t.integer :hardware_id

      t.timestamps
    end
    add_index :hardware_actions, :station_id
  end
end
