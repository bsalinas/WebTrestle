#include <Trestle.h>
#include <WiFlyHQTrestle.h>
#include <WiFlyHQ.h>
#include <SoftwareSerial.h>


SoftwareSerial debugSerial(9,8);
WiFly wifly;
WiFlyHQTrestle bridge("Station1", "192.168.1.18", 4567, &wifly);

/* Change these to match your WiFi network */
const char mySSID[] = "MySSID";
const char myPassword[] = "MyWPAPassword";

long lastTime = 0;
void setup()
{

    debugSerial.begin(9600);
    Serial.begin(9600);
    
    setupWiFi();

   int response = bridge.registerStation("My Station Name", "My Station Description", false);
   if(response < 0){
    Serial.println("Error Creating Station");
   }
   //Add our two sensors by passing the station identifier, an identifier for the sensor, the name, description, and units.
   response = bridge.addSensor("MyFirstSensor", "Time", "The time since startup", "Milliseconds");  
   if(response < 0){
    Serial.println("Error adding sensor");
   }
   response = bridge.addSensor("MySecondSensor", "Room Temperature", "This sensor measures temperature", "Celsius");
   if(response < 0){
      Serial.println("Error adding sensor");
   }
   response = bridge.registerAction("ActionIdentifier","My Action", "This action is cool", myAction);
   if(response < 0){
      Serial.println("Error registering Action");
   }

}

void myAction(char* message){
  //This is called whenever the user clicks on the button on the website.
  Serial.println("Cool!");
  Serial.println(message);
}


void loop()
{
  if((millis()-lastTime) > 30000){
    bridge.tick();
    //Only post every 30 seconds.
    lastTime = millis();
    //Send sensor data to the web page by passing the Station identifier, the sensor identifier, and int version of the value, and the number to divide by to return to a float.
    bridge.sendSensorData("MySecondSensor", int(1000.0*readTemperature()), 1000);      
    bridge.sendSensorData("MyFirstSensor", int(millis()), 1); 
  }
}

//Reads the temperature from a 10K thermistor on A6
float readTemperature(){
 int value = analogRead(A6);
 float R2 = 10000*(1024.0 - value)/(value);
 float R1 = 10000.0;
 float T1 = 298;
 float B = 3892;
  return float( T1*B/log(R1/R2)  /  ( B/log(R1/R2) - T1 ))-273;
}

//Setup the wifi connection.
void setupWiFi(){
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
}

