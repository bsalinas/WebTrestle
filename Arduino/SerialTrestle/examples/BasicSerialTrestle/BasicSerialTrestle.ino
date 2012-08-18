

#include <Trestle.h>
#include <SerialTrestle.h>

SerialTrestle bridge("192.168.1.18", 3000, &Serial);

long lastTime = 0;

void setup()
{
    Serial.begin(9600);
    initializeStationAndSensor();

}

void initializeStationAndSensor(){
    //Register the station with an identifier, a name, a description, and whether old data should be overriden.
   //This will create a web page for this station.
   bridge.registerStation("Station1", "My Station Name", "My Station Description", false);
   //Add our two sensors by passing the station identifier, an identifier for the sensor, the name, description, and units.
   bridge.addSensor("Station1", "MyFirstSensor", "Time", "The time since startup", "Milliseconds");  
   bridge.addSensor("Station1", "MySecondSensor", "Room Temperature", "This sensor measures temperature", "Celsius");

}

void loop()
{
  if((millis()-lastTime) > 30000){
    lastTime = millis();
    //Send sensor data to the web page by passing the Station identifier, the sensor identifier, and int version of the value, and the number to divide by to return to a float.
    bridge.sendSensorData("Station1", "MySecondSensor", int(1000.0*readTemperature()), 1000);      
    bridge.sendSensorData("Station", "MyFirstSensor", int(millis()), 1); 
  }

}

float readTemperature(){
 int value = analogRead(A6);
 float R2 = 10000*(1024.0 - value)/(value);
 float R1 = 10000.0;
 float T1 = 298;
 float B = 3892;
  return float( T1*B/log(R1/R2)  /  ( B/log(R1/R2) - T1 ))-273;
}


