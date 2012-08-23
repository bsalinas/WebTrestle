class StateController < ApplicationController
  def add
  	@station = Station.find_by_identifier(params[:station_identifier])
	if !@station.nil?
		@state = @station.state.find_by_identifier(params[:identifier])
		if @state.nil?
			@state = @station.state.create
		end
		@state.name = params[:name]
		@state.description = params[:description]
		@state.identifier = params[:identifier]
		@state.current_state = ""
		if @state.save
			render :json => { :response => 'ok'}
		else
			render :json => { :response => 'fail', :message => 'Couldn\'t create State'}
		end
	else
		render :json => {:response => 'fail', :message => 'Couldn\'t find Station'}
	end
  end

  def setState
  	@station = Station.find_by_identifier(params[:station_identifier])
	if !@station.nil?
		@state = @station.state.find_by_identifier(params[:identifier])
		if !@state.nil?
			@state.current_state = params[:current_state]
			if @state.save
				render :json => { :response => 'ok'}
			else
				render :json => { :response => 'fail', :message => 'Couldn\'t update State'}
			end
		else
			render :json => { :response => 'fail', :message => 'Couldn\'t find State'}
		end
	else
		render :json => {:response => 'fail', :message => 'Couldn\'t find Station'}
	end
  end

   def getState
  	@station = Station.find_by_identifier(params[:station_identifier])
	if !@station.nil?
		@state = @station.state.find_by_identifier(params[:identifier])
		if !@state.nil?
			render :json => { :response => 'ok', :current_state => @state.current_state}
		else
			render :json => { :response => 'fail', :message => 'Couldn\'t find State'}
		end
	else
		render :json => {:response => 'fail', :message => 'Couldn\'t find Station'}
	end
  end
end
