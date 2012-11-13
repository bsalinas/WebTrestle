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
	if (_wifly->open(_site, _port)) {
		_wifly->print("POST ");
		_wifly->print(request);
		_wifly->println(" HTTP/1.1");
		_wifly->println("Content-Type: application/x-www-form-urlencoded");
		_wifly->println("Connection: close");
		_wifly->print("Content-Length: ");
		_wifly->println(strlen(data));
		_wifly->println();
		_wifly->print(data);
		return waitForResponse();
  	}
  return false;
}
boolean WiFlyHQTrestle::waitForResponse(){
	long starttime = millis();
	long now = millis();
	while(_wifly->available() <=0 ){
		now = millis();
		if((now - starttime) > 10000){
			return false;
		}
		delay(10);
	}
	int i=0;
	boolean waitingForJSON = true;
	_lastResponse[0]='\0';
	boolean done = false;
	while(!done){
		if(_wifly->available() > 0){
			char c = char(_wifly -> read());
			//Wait until we get to the JSON bits.
			// Serial.print(c);
			if(c == '{'){
				waitingForJSON = false;
			}
		
			if(!waitingForJSON){
				_lastResponse[i] = c;
				i++;
				if(c=='}'){
					done = true;
					break;
				}
			// else{
			// 	if(_wifly->available()<=0){
			// 		Serial.println("delaying");
			// 		waiting = true;
			// 		delay(100);
			// 	}
			// 	else{
			// 		Serial.println("more left");
			// 	}
			// }
			}
		}
		
	}
	_lastResponse[i]='\0';
	return true;
}