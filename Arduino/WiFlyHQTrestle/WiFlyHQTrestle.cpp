#include "WiFlyHQTrestle.h"

WiFlyHQTrestle::WiFlyHQTrestle(char* station_identifier, char* site, int port, WiFly* wifly){
	_site = site;
	_wifly = wifly;
	_port = port;
	_biggestAction = -1;
	strncpy(_stationIdentifier, station_identifier, MAX_LENGTH_STATION_IDENTIFIER);
}

boolean WiFlyHQTrestle::makePost(char* request, char* data){
	if(_wifly-> isConnected()){
		_wifly->close();
	}
	if (_wifly->open(_site, 3000)) {
		_wifly->print("POST ");
		_wifly->print(request);
		_wifly->println(" HTTP/1.1");
		_wifly->println("Content-Type: application/x-www-form-urlencoded");
		_wifly->println("Connection: close");
		_wifly->print("Content-Length: ");
		_wifly->println(strlen(data));
		_wifly->println();
		_wifly->println(data);
		_wifly->println();
		return waitForResponse();
  	}
  return false;
}
boolean WiFlyHQTrestle::waitForResponse(){

	while(_wifly->available() <=0 ){
		delay(10);
	}
	int i=0;
	boolean waiting = true;
	_lastResponse[0]='\0';
	while(_wifly->available() > 0){
		char c = char(_wifly -> read());
		//Wait until we get to the JSON bits.
		if(c == '{'){
			waiting = false;
		}
		if(_wifly->available() <= 0){
			delay(100);
		}
		if(!waiting){
			_lastResponse[i] = c;
			i++;
		}
	}
	_lastResponse[i]='\0';
	return true;
}