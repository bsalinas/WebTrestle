#include "Trestle.h"

int Trestle::registerStation(char* name, char* description, boolean override){
	strcpy_P(_buffer, (char*)register_string); 
	sprintf(_lastResponse, _buffer, name, description, override ? "true" : "false", _stationIdentifier);
	if(makePost("/stations/register.json", _lastResponse)){
		return evaluateResponse();
	}
	return STATUS_POST_ERROR;
}
int Trestle::addSensor(char* identifier, char* name, char* description, char* units){
	strcpy_P(_buffer, (char*)add_sensor_string); 
	sprintf(_lastResponse, _buffer, name, description, _stationIdentifier, identifier, units);

	if(makePost("/sensor/add.json", _lastResponse)){

		return evaluateResponse();
	}
	return STATUS_POST_ERROR;
}
int Trestle::sendSensorData(char* sensor_identifier, int value, int multiplier){
	strcpy_P(_buffer, (char*)send_sensor_string); 
	sprintf(_lastResponse, _buffer, _stationIdentifier, sensor_identifier,value, multiplier);
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
		strcpy_P(_buffer, (char*)register_action_string); 
		sprintf(_lastResponse, _buffer, _stationIdentifier, identifier,name, description, _biggestAction);
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

int Trestle::addState(char* identifier, char* name, char* description){
	strcpy_P(_buffer, (char*)add_state_string); 
	sprintf(_lastResponse, _buffer, _stationIdentifier, identifier, name, description);

	if(makePost("/state/add.json", _lastResponse)){

		return evaluateResponse();
	}
	return STATUS_POST_ERROR;
}

int Trestle::setStateValue(char* identifier, char* value){
	strcpy_P(_buffer, (char*)set_state_string); 
	sprintf(_lastResponse, _buffer, _stationIdentifier, identifier, value);

	if(makePost("/state/setState.json", _lastResponse)){

		return evaluateResponse();
	}
	return STATUS_POST_ERROR;
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