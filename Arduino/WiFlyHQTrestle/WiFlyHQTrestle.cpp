#include "WiFlyHQTrestle.h"

WiFlyHQTrestle::WiFlyHQTrestle(char* site, int port, WiFly* wifly){
	_site = site;
	_wifly = wifly;
	_port = port;
}

boolean WiFlyHQTrestle::makePost(char* request, char* data){
	if(_wifly-> isConnected()){
		_wifly->close();
	}
	if (_wifly->open("192.168.1.18", 3000)) {
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
	//Add Timeout
  while(_wifly->available() <= 0){
    delay(10);
  }
  _lastResponse[0]='\0';
  int i=0;
  while(_wifly->available() > 0){
    char ch = _wifly->read();
    _lastResponse[i] = ch;
    i++;
  }
  _lastResponse[i]= '\0';
  return true;
}