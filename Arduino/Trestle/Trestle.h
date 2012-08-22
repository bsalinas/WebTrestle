#ifndef TRESTLE_H
#define TRESTLE_H
#include <Arduino.h>

typedef void (* ActionFunctionPtr) ();
#define NActions 3
#define MAX_LENGTH_STATION_IDENTIFIER 32
#define STATUS_SUCCESS 1
#define STATUS_POST_ERROR -1
#define STATUS_SERVER_KNOWN_ERROR 2
#define STATUS_SERVER_UNKNOWN_ERROR -2
#define STATUS_OUT_OF_BOUNDS_ERROR -3

class Trestle{
public:
	int registerStation(char* name, char* description, boolean override);
	int addSensor(char* identifier, char* name, char* description, char* units);
	int sendSensorData(char* sensor_identifier, int value, int multiplier);
	int registerAction(char* identifier, char* name, char* description, ActionFunctionPtr func);
	int tick();
protected:
	virtual boolean makePost(char* request, char* data);
	virtual boolean waitForResponse();
	int evaluateResponse();
	char _lastResponse[255];
	int _biggestAction;
	char _stationIdentifier[MAX_LENGTH_STATION_IDENTIFIER + 1];
	ActionFunctionPtr _functions[NActions];


};

#endif