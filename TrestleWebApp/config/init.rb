require 'sinatra/sequel'
#require 'sinatra'
require 'sequel'
require 'pg'

configure :development do
  #set :database, 'sqlite://tmp/development.sqlite'
# DB = Sequel.connect(ENV['HEROKU_POSTGRESQL_AMBER_URL'] || 'postgres://localhost/mydb')
DB = Sequel.connect('postgres://vetknwqblbajbl:-H5C4uo7Xz-8_v1TIb9UjSY1tJ@ec2-23-21-209-58.compute-1.amazonaws.com:5432/deia1866o0if4h')
#DB = database
end

configure :test do
 # set :database, 'sqlite3::memory:'
end
configure :production do 
  # puts ENV
DB = Sequel.connect('postgres://vetknwqblbajbl:-H5C4uo7Xz-8_v1TIb9UjSY1tJ@ec2-23-21-209-58.compute-1.amazonaws.com:5432/deia1866o0if4h')
end

# require_relative 'migrations.rb'

# Sequel::Model.strict_param_setting = false
# Dir["models/**/*.rb"].each{|model|
#   require_relative "../"+model
# }

helpers do
  include Rack::Utils
  alias_method :h, :escape_html
  
  # partial helper taken from Sam Elliot (aka lenary) at http://gist.github.com/119874 
  # which itself was based on Chris Schneider's implementation:
  # http://github.com/cschneid/irclogger/blob/master/lib/partials.rb
  def partial(template, *args)
    template_array = template.to_s.split('/')
    template = template_array[0..-2].join('/') + "/_#{template_array[-1]}"
    options = args.last.is_a?(Hash) ? args.pop : {}
    options.merge!(:layout => false)
    if collection = options.delete(:collection) then
      collection.inject([]) do |buffer, member|
        buffer << erb(:"#{template}", options.merge(:layout =>
        false, :locals => {template_array[-1].to_sym => member}))
      end.join("\n")
    else
      erb(:"#{template}", options)
    end
  end
end
