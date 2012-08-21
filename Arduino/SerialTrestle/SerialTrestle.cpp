#include "SerialTrestle.h"

SerialTrestle::SerialTrestle(char* station_identifier, char* site, int port, Stream* serial){
	_site = site;
	_port = port;
	_serial = serial;
	_biggestAction = -1;
	strncpy(_stationIdentifier, station_identifier, MAX_LENGTH_STATION_IDENTIFIER);
}

boolean SerialTrestle::makePost(char* request, char* data){
	_serial -> flush();
	_serial->write(0x04);
	_serial->print("http://");
	_serial->print(_site);
	_serial->print(":");
	_serial->print(_port);
	_serial->write(0x05);
	_serial->write(0x06);
	_serial->print(request);
	_serial->write(0x07);
	_serial->write(0x08);
	_serial->print(data);
	_serial->write(0x03);
	return waitForResponse();
}
boolean SerialTrestle::waitForResponse(){
	while(_serial->available() <=0 ){
		delay(10);
	}
	int i=0;
	_lastResponse[0]='\0';
	while(_serial->available() > 0){
		_lastResponse[i] = _serial->read();
		i++;
		delay(10);
	}
	_lastResponse[i]='\0';
	return true;
}