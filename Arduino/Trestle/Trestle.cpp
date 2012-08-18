#include "Trestle.h"

int Trestle::registerStation(char* identifier, char* name, char* description, boolean override){
	// char data[255+strlen(identifier)+strlen(name)+strlen(description)];
	sprintf(_lastResponse, "name=%s&description=%s&override=%s&identifier=%s", name, description, override ? "true" : "false", identifier);
	if(makePost("/stations/register.json", _lastResponse)){
		return evaluateResponse();
	}
	return STATUS_POST_ERROR;
}
int Trestle::addSensor(char* station_identifier, char* identifier, char* name, char* description, char* units){
	// char data[255+strlen(station_identifier) + strlen(identifier)+strlen(name)+strlen(description)+strlen(units)];
	sprintf(_lastResponse, "name=%s&description=%s&station_identifier=%s&identifier=%s&units=%s", name, description, station_identifier, identifier, units);
	if(makePost("/sensor/add.json", _lastResponse)){
		return evaluateResponse();
	}
	return STATUS_POST_ERROR;
}
int Trestle::sendSensorData(char* station_identifier, char* sensor_identifier, int value, int multiplier){
	// char data[255+strlen(sensor_identifier)];
	sprintf(_lastResponse, "station_identifier=%s&sensor_identifier=%s&value=%i&multiplier=%i", station_identifier, sensor_identifier,value, multiplier);
	if(makePost("/measurement/add.json", _lastResponse)){
		return evaluateResponse();
	}
	return STATUS_POST_ERROR;
}

int Trestle::evaluateResponse(){
	char* pch = strstr (_lastResponse,"\"response\":\"ok\"");
	if(pch == NULL){
		pch = strstr (_lastResponse,"\"response\":\"fail\"");
		if(pch == NULL){
			return STATUS_SERVER_UNKNOWN_ERROR;
		}
		return STATUS_SERVER_KNOWN_ERROR;
	}
	return STATUS_SUCCESS;

}