#ifndef TRESTLE_H
#define TRESTLE_H
#include <Arduino.h>
#include <avr/pgmspace.h>

	const char register_string[] PROGMEM = "name=%s&description=%s&identifier=%s";   // "String 0" etc are strings to store - change to suit.
	const char add_sensor_string[] PROGMEM = "name=%s&description=%s&station_identifier=%s&identifier=%s&units=%s";
	const char send_sensor_string[] PROGMEM = "station_identifier=%s&sensor_identifier=%s&value=%i&multiplier=%i";
	const char register_action_string[] PROGMEM = "station_identifier=%s&identifier=%s&name=%s&description=%s&hardware_id=%i";
	const char add_state_string[] PROGMEM = "station_identifier=%s&identifier=%s&name=%s&description=%s";
	const char set_state_string[] PROGMEM = "station_identifier=%s&identifier=%s&current_state=%s";

typedef void (* ActionFunctionPtr) (char*);
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
	int addState(char* identifier, char* name, char* description);
	int setStateValue(char* identifier, char* value);
	int tick();
protected:
	virtual boolean makePost(char* request, char* data);
	virtual boolean waitForResponse();
	int evaluateResponse();
	char _lastResponse[255];
	int _biggestAction;
	char _buffer[100];
	char _stationIdentifier[MAX_LENGTH_STATION_IDENTIFIER + 1];
	ActionFunctionPtr _functions[NActions];




};

#endif