class CreateMeasurements < ActiveRecord::Migration
  def change
    create_table :measurements do |t|
      t.float :value
      t.references :sensor

      t.timestamps
    end
    add_index :measurements, :sensor_id
  end
end
