#Overview
The WebTrestle project is about building a simple bridge between the land of hardware and the internet. Currently, if you want to have a microcontroller upload data to the internet, you need to build a web application, define messaging prototocols, setup your hardware each time you start a new project.  In a few lines of code on an Arduino, you can generate a dynamic webpage for displaying data. Future versions will allow for control of hardware from the website as well.

#Example and Usage

## Arduino Library
###Library Description
At this point, there are two varieties of the Trestle library for Arduino.
* WiFlyHQTrestle: This library is designed around the [WiFlyHQ Library](https://github.com/harlequin-tech/WiFlyHQ) for the [Roving Networks WiFly RN-XV](http://www.rovingnetworks.com/products/RN_XV). 
* SerialTrestle: This library communicates data over Serial to a computer. An included python script must be run on the host computer to post the data to the internet. This allows to connect hardware to the internet without the need for a costly network shield or WiFi module.
###Installation
Minimally, you need the base `Arduino\Trestle` library as well as the library listed above that you would like to use. Installation instructions for Arduino libraries can be found on the [Arduino website](http://arduino.cc/it/Reference/Libraries).
###Usage
Each variation of the Trestle library uses the same common commands but has a different constructor.  
####Construction of the Trestle Object
For SerialTrestle, construct a SerialTrestle object as follows
```c++
SerialTrestle bridge("Station1", "192.168.1.18", 4567, &Serial);
```
In this case, Station1 is an identifier for the station, 192.168.1.18 is the web host, and 4567 is the port that it accepts communication over (Generally this will be 80, but the rails development configuration defaults to port 3000). `&Serial` provides a pointer to the Serial port that will be connected to the computer. This could be any of the standard hardware Serial ports or an insteance of a `SoftwareSerial` port.
For WiFlyHQTrestle, construct the Trestle object as follows
```c++
WiFlyHQTrestle bridge("Station1", "192.168.1.18", 4567, &wifly);
```
The host and the port are the same in this example. `&wifly` is a pointer to the WiFlyHQ object.
####Communication Setup
In the `void setup()` method, you must prepare the communication device. For `SerialTrestle`, you just need to use a
```c++
Serial.begin(57600);
```
SerialTrestle will work with any baud rate, so long as you run the python script at the same baud rate.
In the case of `WiFlyHQTrestle`, you need to setup the connection to your WiFi network. 
```c++
void setup(){
    //Initialize WiFly with the Serial port being  used to communicate with the module.
    if (!wifly.begin(&Serial, &debugSerial)) {
       Serial.println("WiFly Connection Failed");
       while(1){
       }
    }

    /* Join wifi network if not already associated */
    if (!wifly.isAssociated()) {
	/* Setup the WiFly to connect to a wifi network */
	wifly.setSSID(mySSID);
	wifly.setPassphrase(myPassword);
	wifly.enableDHCP();

	if (wifly.join()) {
	    debugSerial.println("Joined wifi network");
	} else {
	    debugSerial.println("Failed to join wifi network");
          while(1){
          }
	}
    } else {
        debugSerial.println("Already joined network");
    }

    wifly.setDeviceID("Wifly-WebClient");


    if (wifly.isConnected()) {
	wifly.close();
    }
```
####Register a Station
Registering a station (and the rest of the steps) are independent of what variety of Trestle you are using. In Trestle, every sensor is associated with a `Station`. To create a station call the registerStation function as follows:
```c++
//Register the station with a name, a description, and whether old data should be overriden.
int result = bridge.registerStation("My Station Name", "My Station Description", false);
//If result > 0, then the station was successfully registered
```
Once this command has been called, a website will be created for this station with the included name and description. The final argument should be `true` if any old data associated with this station (or any station with `Station1` as an identifier) should be destroyed.
####Sensors
Once a station has been registered, sensors can be added to it. A single station may have multiple sensors associated with it. Each sensor, at creation, is given an identifier, a name, a description, and units using the following command
```c++
//Add our two sensors by passing an identifier for the sensor, the name, description, and units.
bridge.addSensor("MyFirstSensor", "Time", "The time since startup", "Milliseconds");  
bridge.addSensor("MySecondSensor", "Room Temperature", "This sensor measures temperature", "Celsius");
```
Each has their own name, description, and units in addition to an identifier which will be used to associate measurements with the sensor.

Data can be submitted to the web service once the sensors have been created.

```c++
//Send sensor data to the web page by passing the sensor identifier, and int version of the value, and the number to divide by to return to a float.
bridge.sendSensorData("MySecondSensor", int(1000.0*readTemperature()), 1000);      
bridge.sendSensorData("MyFirstSensor", int(millis()), 1); 
```
The Station Identifier declared above and the Sensor Identifier are sent along with the data. To allow for simple handling of floats, a multiplier can be specified. In the first case, we are multiplying our temperature by 1000 before submitting it as an integer. The server will then divide by 1000 before displaying the data.
As data is submitted, it will appear on the website for the Station.

####States
A State is another way for the hardware to submit data to the website. Unlike with the Sensors, a State only has a single value at any given time (as opposed to a history of data values). To add a state, the following command is used
```c++
//Add a state to the website
bridge.addState("State1", "State Name", "State Description");
```
Then, to update the value of the state, use the following command:
```c++
//Set the state
bridge.setState("State1", "The Value to Set");
```

####Actions
An action allows the user to trigger a behavior on the hardware from the website. One an action is registered, a link to trigger that action appears on the station page. Then, whenever that action is requested by the user, the method associated with the action will be called on the hardware.

```c++
bridge.registerAction("ActionIdentifier", "My Action", "This action is cool", myAction);
...
void myAction(char* message){
	Serial.println("Cool!");
	Serial.println(message);
}
```
In this case, the Action has an Identifier, a name, a description and a method to execute when the action is performed.
Each time you want the hardware to check if there is a new action to perform, you must call the tick() method. This can be placed in the loop, however it is not recommended to call this more than every 10 seconds or so.



##Web App
Until there is a permanently hosted version of the web app, you must run your own server. To create the database and run the app, navigate to the TrestleWebApp folder and run `ruby app.rb`. It is possible you will need to install some gems first.

###Web Services
There are a few web services that are available so that other applications can interact with the app. Currently, only the following web services are available
####Perform Action
This will cause the hardware to perform an action.
```
POST perform_action 
{station_identifier: "Station1", identifier: "ActionIdentifier", message: "Hello World"}
```
Sample Usage with Curl
```
curl -d "station_identifier=Station1&identifier=ActionIdentifier&message=Hello World" localhost:4567/perform_action
```
Sample response
```
{"response":"ok"}
```
####Get State
This will get the value of a state.
```
GET get_state
{station_identifier: "Station1", identifier: "State1"}
```
An example using curl
```
curl "localhost:4567/get_state?station_identifier=Station1&identifier=State1"
```
The response will be
```
{"response":"ok","current_state":"StateValue","updated_at":"2012-08-28T17:59:58Z"}
```

