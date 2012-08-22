#include "Trestle.h"

int Trestle::registerStation(char* name, char* description, boolean override){
	// char data[255+strlen(identifier)+strlen(name)+strlen(description)];
	sprintf(_lastResponse, "name=%s&description=%s&override=%s&identifier=%s", name, description, override ? "true" : "false", _stationIdentifier);
	if(makePost("/stations/register.json", _lastResponse)){
		return evaluateResponse();
	}
	return STATUS_POST_ERROR;
}
int Trestle::addSensor(char* identifier, char* name, char* description, char* units){
	// char data[255+strlen(station_identifier) + strlen(identifier)+strlen(name)+strlen(description)+strlen(units)];
	sprintf(_lastResponse, "name=%s&description=%s&station_identifier=%s&identifier=%s&units=%s", name, description, _stationIdentifier, identifier, units);
	// sprintf(_lastResponse, "{\"name\":\"%s\",\"description\":\"%s\",\"station_identifier\":\"%s\",\"identifier\":\"%s\",\"units\":\"%s\"}", name, description, _stationIdentifier, identifier, units);

	if(makePost("/sensor/add.json", _lastResponse)){

		return evaluateResponse();
	}
	return STATUS_POST_ERROR;
}
int Trestle::sendSensorData(char* sensor_identifier, int value, int multiplier){
	// char data[255+strlen(sensor_identifier)];
	sprintf(_lastResponse, "station_identifier=%s&sensor_identifier=%s&value=%i&multiplier=%i", _stationIdentifier, sensor_identifier,value, multiplier);
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

int Trestle::registerAction(char* identifier, char* name, char* description, ActionFunctionPtr func){
	_biggestAction++;
	if(_biggestAction < NActions){
		sprintf(_lastResponse, "station_identifier=%s&identifier=%s&name=%s&description=%s&hardware_id=%i", _stationIdentifier, identifier,name, description, _biggestAction);
		if(makePost("/hardware_action/add.json", _lastResponse)){
			int resp =  evaluateResponse();
			if(resp == STATUS_SUCCESS){
				_functions[_biggestAction] = func;
			}
			return _biggestAction;
		}
		return STATUS_POST_ERROR;
	}
	return STATUS_OUT_OF_BOUNDS_ERROR;
}

int Trestle::tick(){
	sprintf(_lastResponse, "station_identifier=%s", _stationIdentifier);
	if(makePost("/hardware_action/getPending.json", _lastResponse)){
		int resp = evaluateResponse();
		if(resp == STATUS_SUCCESS){
			char* pch = strstr (_lastResponse,"\"action\":\"");	
			if(pch != NULL){
				int len = strcspn(pch+10, "\"");
				strncpy(_lastResponse, pch+10, len);
				_lastResponse[len]='\0';
				if(atoi(_lastResponse) < NActions && atoi(_lastResponse)>=0 && atoi(_lastResponse) <= _biggestAction){
					_functions[atoi(_lastResponse)]();
				}
				else{
					return STATUS_OUT_OF_BOUNDS_ERROR;
				}
			}
			else{
				return STATUS_SERVER_UNKNOWN_ERROR;
			}
		}
		else{
			return resp;
		}
	}
	return STATUS_POST_ERROR;
}