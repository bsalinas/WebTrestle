class CreateStations < ActiveRecord::Migration
  def change
    create_table :stations do |t|
      t.string :name
      t.string :description
      t.string :identifier
      t.boolean :override
      t.references :user

      t.timestamps
    end
    add_index :stations, :user_id
  end
end
