class StationsController < ApplicationController
  def index
  	@stations = Station.all
  end

  def new
  	@station = Station.new
  	# respond_to do |format|
   #    format.html # new.html.erb
   #    format.json { render json: @station }
   #  end
  end
  def create
  	@station = Station.create(params[:station])
  	@station.user = current_user
  	if(@station.save)
  		redirect_to @station
	 else
	 	render :action => "new"
	 end

  end
  def show
    if params[:id]
  	 @station = Station.find(params[:id])
    else
      @station = Station.find_by_identifier(params[:identifier])
    end
  end
  def edit

  end

  def update

  end

  def destroy

  end

  def register
  	@station = Station.find_by_identifier(params[:identifier])
    puts params
  	if @station.nil?
  		createdIt = true
  		@station = Station.create({:identifier => params[:identifier], :name => params[:name], :description=>params[:description], :override => params[:override]})
  		@station.save
  		#need error handling
  	else
  		createdIt = false
  		if params[:override] == 'true'
  			@station.sensor.destroy_all
  			@station.name = params[:name]
  			@station.description = params[:description]
  			@station.save
  			#need error handling
  		end

  	end

  	respond_to do |format|
  		format.html
  		format.json do
  			render :json => { :response => 'ok', :status => :ok}
  		end
  	end

  end
end
