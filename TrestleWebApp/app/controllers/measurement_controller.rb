class MeasurementController < ApplicationController
	def add
		@station = Station.find_by_identifier(params[:station_identifier]);
		if !@station.nil?
			@sensor = @station.sensor.find_by_identifier_and_active(params[:sensor_identifier], true)
			if !@sensor.nil?
				value = Integer(params[:value])/Integer(params[:multiplier])
				@measurement = @sensor.measurement.create(:value => value)
				if @measurement.save
					render :json => { :response => 'ok'}
				else
					render :json => { :response => 'fail', :message => 'Couldn\'t create Measurement'}
				end
			else
				render :json => {:response => 'fail', :message => 'Couldn\'t find Sensor'}
			end
		else
			render :json => {:response => 'fail', :message => 'Couldn\'t find Station'}
		end
	end
end
