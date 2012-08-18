#ifndef TRESTLE_H
#define TRESTLE_H
#include <Arduino.h>


#define STATUS_SUCCESS 1
#define STATUS_POST_ERROR -2
#define STATUS_SERVER_KNOWN_ERROR 0
#define STATUS_SERVER_UNKNOWN_ERROR -1

class Trestle{
public:
	int registerStation(char* identifier, char* name, char* description, boolean override);
	int addSensor(char* station_identifier, char* identifier, char* name, char* description, char* units);
	int sendSensorData(char* station_identifier, char* sensor_identifier, int value, int multiplier);
protected:
	virtual boolean makePost(char* request, char* data);
	virtual boolean waitForResponse();
	int evaluateResponse();
	char _lastResponse[255];


};

#endif