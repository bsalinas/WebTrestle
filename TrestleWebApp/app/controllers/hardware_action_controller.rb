class HardwareActionController < ApplicationController
  def getPending
	@station = Station.find_by_identifier(params[:station_identifier])
		if !@station.nil?
			@action = @station.hardware_action.find_by_pending(true, :limit=>1)
			if !@action.nil?
				@action.pending = false
				message = @action.message
				@action.message = ""
				@action.save
				render :json => { :response => 'ok', :action => @action.hardware_id.to_s, :message => message}
			else
				render :json => { :response => 'ok', :action => "-1"}
			end
		else
			render :json => {:response => 'fail', :message => 'Couldn\'t find Station'}
		end
  end

  #/hardware_action/performAction.json {station_identifier: STATION1, identifier: ACTION1, message : MESSAGE}
  def performAction
  	@station = Station.find_by_identifier(params[:station_identifier])
  	if !@station.nil?
  		@action = @station.hardware_action.find_by_identifier(params[:identifier], :limit => 1)
  		if !@action.nil?
  			@action.pending = true
  			@action.message = params[:message]
  			if @action.save
  				render :json => { :response => 'ok', :action => @action}
  			else
  				render :json => {:response => 'fail', :message => 'Couldn\'t save Action'}
  			end
  		else
			render :json => {:response => 'fail', :message => 'Couldn\'t find Action'}
		end
  	else
		render :json => {:response => 'fail', :message => 'Couldn\'t find Station'}
  	end
  			

  end


  def forcePending
  	@hardware_action = HardwareAction.find(params[:hardware_action])
  	@hardware_action.pending = true
  	@hardware_action.message = params[:message]
  	if(@hardware_action.save)
  		redirect_to @hardware_action.station
  	end
  end


  def add
  	@station = Station.find_by_identifier(params[:station_identifier])
		if !@station.nil?
			@action = @station.hardware_action.find_by_identifier(params[:identifier])
			if @action.nil?
				#handle the case where this already exists gracefully?
				@action = @station.hardware_action.create
			end
			@action.name = params[:name]
			@action.description = params[:description]
			@action.identifier = params[:identifier]
			@action.hardware_id = params[:hardware_id]
			@action.pending = false
			if @action.save
				render :json => { :response => 'ok', :status => :ok}
			else
				render :json => { :response => 'fail', :message => 'Couldn\'t create Action'}
			end
		else
			render :json => {:response => 'fail', :message => 'Couldn\'t find Station'}
		end
  end
end
