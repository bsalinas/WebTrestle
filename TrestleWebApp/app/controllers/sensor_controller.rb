class SensorController < ApplicationController
	def add
		@station = Station.find_by_identifier(params[:station_identifier])
		if !@station.nil?
			@sensor = @station.sensor.find_by_identifier(params[:identifier])
			if @sensor.nil?
				@sensor = @station.sensor.create
			end
			@sensor.name = params[:name]
			@sensor.description = params[:description]
			@sensor.identifier = params[:identifier]
			@sensor.units = params[:units]
			@sensor.active = true
			if @sensor.save
				render :json => { :response => 'ok'}
				puts "A"
			else
				render :json => { :response => 'fail', :message => 'Couldn\'t create Sensor'}
				puts "B"
			end
		else
			render :json => {:response => 'fail', :message => 'Couldn\'t find Station'}
			puts "C"
		end
	end
end
