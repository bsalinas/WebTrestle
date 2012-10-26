require 'rubygems'
require 'sinatra'
require 'sinatra/flash'
require 'gon-sinatra'
require 'haml'
require_relative 'config/init.rb'
require "sinatra-authentication"
require 'json'
use Rack::Session::Cookie, :secret => "heyhihello"
set :server, :thin

connections = []
# Create our models
class Station < Sequel::Model
  one_to_many :sensor
  one_to_many :hardware_action
  one_to_many :state
end

class Sensor < Sequel::Model
  many_to_one :station
  one_to_many :measurement
end

class Measurement < Sequel::Model
  many_to_one :sensor
end

class HardwareAction <Sequel::Model
  many_to_one :station
end

class State <Sequel::Model
  many_to_one :station
end

#Add Timestamps
Station.plugin :timestamps
Sensor.plugin :timestamps
Measurement.plugin :timestamps
HardwareAction.plugin :timestamps
State.plugin :timestamps


# #Generate some test data.
# @station = Station.create({:name => "My Station", :identifier => "Station1", :description => "This is a station"})
# @sensor = Sensor.create({:name => "Sensor 1", :identifier=>"MySensor", :station_id => @station.id})
# Sensor.create({:name => "Sensor 2", :identifier=>"MySensor2", :station_id => @station.id})

# @station.sensor.each do |sensor|

#   puts sensor.name
# end

def find_station(identifier)
  station = Station.find(:identifier => identifier)
end

get '/' do
  render_login_logout
end

get("/stations/:identifier") do
  if params[:id]
    @station = Station.find(params[:id])
  else
    @station = find_station(params[:identifier])
    end
    haml :view_station
end

get("/stations") do
  @stations = Station.all
  haml :view_all_stations
end

post '/register_station/?' do
  content_type :json
  @station = find_station(params[:identifier])
  puts params
  if @station.nil?
    createdIt = true
    @station = Station.new
    @station.identifier =  params[:identifier]
    @station.name = params[:name]
    @station.description = params[:description]
    # @station.override = params[:override]
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

  return { :response => 'ok', :status => :ok}.to_json
end

post '/add_sensor/?' do 
  content_type :json
  @station = find_station(params[:station_identifier])
  if !@station.nil?
    @sensor = Sensor.find(:identifier => params[:identifier], :station_id => @station.id)
    if @sensor.nil?
      @sensor = Sensor.new
      @sensor.station_id = @station.id
    end
    @sensor.name = params[:name]
    @sensor.description = params[:description]
    @sensor.identifier = params[:identifier]
    @sensor.units = params[:units]
    if @sensor.save
      return { :response => 'ok'}.to_json
    else
      return { :response => 'fail', :message => 'Couldn\'t create Sensor'}.to_json
    end
  else
    return {:response => 'fail', :message => 'Couldn\'t find Station'}.to_json
  end
end

post '/add_measurement/?' do
  content_type :json
  puts params
  @station = find_station(params[:station_identifier])
  if !@station.nil?
    puts @station.id
    puts params[:sensor_identifier]
    @sensor = Sensor.find(:identifier => params[:sensor_identifier], :station_id => @station.id)
    if !@sensor.nil?
      value = Float(params[:value])/Float(params[:multiplier])
      @measurement = Measurement.new
      @measurement.value = value
      @measurement.sensor_id = @sensor.id
      if @measurement.save
        puts connections
        # Measurement.find(:sensor_id => @sensor.id)
        connections.each do |out|
         data = {:sensor_id => @measurement.sensor_id, :created_at => @measurement.created_at, :value => @measurement.value}.to_json
         puts out
         puts data
         out << "data: #{data}\nevent: measurement\n\n"
        end
        return { :response => 'ok'}.to_json
      else
        puts "No measurment"
        return { :response => 'fail', :message => 'Couldn\'t create Measurement'}.to_json
      end
    else
      puts "No Sensor"
      return {:response => 'fail', :message => 'Couldn\'t find Sensor'}.to_json
    end
  else
    return {:response => 'fail', :message => 'Couldn\'t find Station'}.to_json
  end
end

post '/add_state/?' do
  content_type :json
  @station = find_station(params[:station_identifier])
  if !@station.nil?
    @state = State.find(:identifier => params[:identifier], :station_id => @station.id)
    if @state.nil?
      @state = State.new
      @state.station_id = @station.id
    end
    @state.name = params[:name]
    @state.description = params[:description]
    @state.identifier = params[:identifier]
    @state.current_state = ""
    if @state.save
      return { :response => 'ok'}.to_json
    else
      return { :response => 'fail', :message => 'Couldn\'t create State'}.to_json
    end
  else
    return {:response => 'fail', :message => 'Couldn\'t find Station'}.to_json
  end
end

post '/set_state/?' do 
  content_type :json
  @station = find_station(params[:station_identifier])
  if !@station.nil?
    @state = State.find(:identifier => params[:identifier], :station_id => @station.id)
    if !@state.nil?
      @state.current_state = params[:current_state]
      if @state.save
        puts connections
        connections.each do |out|
         data = {:state_id => @state.id, :time => @state.updated_at, :value => @state.current_state}.to_json
         out << "data: #{data}\nevent: state\n\n"
        end
        return { :response => 'ok'}.to_json
      else
        return { :response => 'fail', :message => 'Couldn\'t update State'}.to_json
      end
    else
      return { :response => 'fail', :message => 'Couldn\'t find State'}.to_json
    end
  else
    return {:response => 'fail', :message => 'Couldn\'t find Station'}.to_json
  end
end

get '/get_state' do   
  content_type :json
  @station = find_station(params[:station_identifier])
  #need to handle if the station returns more than one... the station_identifier should be unique.
  puts @station
  if !@station.nil?
    @state = State.find(:identifier=>params[:identifier], :station_id => @station.id)
    puts @state
    if !@state.nil?
      return { :response => 'ok', :current_state => @state.current_state, :updated_at => @state.updated_at}.to_json
    else
      return { :response => 'fail', :message => 'Couldn\'t find State'}.to_json
    end
  else
    return {:response => 'fail', :message => 'Couldn\'t find Station'}.to_json
  end
end 

post '/add_action/?' do 
  content_type :json
  @station = find_station(params[:station_identifier])
  if !@station.nil?
    @action = HardwareAction.find(:identifier=> params[:identifier], :station_id => @station.id)
    if @action.nil?
      #handle the case where this already exists gracefully?
      @action = HardwareAction.new
      @action.station_id = @station.id;
    end
    @action.name = params[:name]
    @action.description = params[:description]
    @action.identifier = params[:identifier]
    @action.hardware_id = params[:hardware_id]
    @action.pending = false
    if @action.save
      return { :response => 'ok', :status => :ok}.to_json
    else
      return { :response => 'fail', :message => 'Couldn\'t create Action'}.to_json
    end
  else
    return {:response => 'fail', :message => 'Couldn\'t find Station'}.to_json
  end
end 

post '/get_pending_actions/?' do
  content_type :json
  puts params
  @station = find_station(params[:station_identifier])
  if !@station.nil?
    @action = HardwareAction.find(:pending => true, :station_id => @station.id)
    if !@action.nil?
      @action.pending = false
      message = @action.message
      @action.message = ""
      @action.save
      connections.each do |out|
        data = {:action_id => @action.id, :action_identifier => @action.identifier}.to_json
        out << "data: #{data}\nevent: action_performed\n\n"
      end
      return { :response => 'ok', :action => @action.hardware_id.to_s, :message => message}.to_json
    else
      puts "No Action"
      return { :response => 'ok', :action => "-1"}.to_json
    end
  else
    puts "NoStation"
    return {:response => 'fail', :message => 'Couldn\'t find Station'}.to_json
  end
end

post '/perform_action/?' do
  content_type :json
  @station = find_station(params[:station_identifier])
    if !@station.nil?
      @action = HardwareAction.find(:identifier=> params[:identifier], :station_id => @station.id)
      if !@action.nil?
        @action.pending = true
        @action.message = params[:message]
        if @action.save
          return { :response => 'ok', :action => @action}.to_json
        else
          return {:response => 'fail', :message => 'Couldn\'t save Action'}.to_json
        end
      else
      return {:response => 'fail', :message => 'Couldn\'t find Action'}.to_json
    end
    else
      return {:response => 'fail', :message => 'Couldn\'t find Station'}.to_json
    end
end

get '/get_measurements' do
  content_type :json
  @sensor = Sensor.find(:id=>params[:sensor_id])
  if !@sensor.nil?
    # @measurements = DB[:measurements].filter(:sensor_id => params[:sensor_id])
    @measurements = []
     Measurement.where(:sensor_id => params[:sensor_id]).each{|measure| @measurements<<measure.to_hash}
    return { :response => 'ok', :measurements => @measurements}.to_json
  else
    return { :response => 'fail', :message => 'Couldn\'t find Sensor'}.to_json
  end
end 

get '/stream', provides: 'text/event-stream' do
  stream :keep_open do |out|
    connections << out
    out.callback { connections.delete out }

  end
end
