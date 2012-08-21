#ifndef WIFLYHQTRESTLE_H
#define WIFLYHQTRESTLE_H
#include <Arduino.h>
#include <WiFlyHQ.h>
#include <Trestle.h>


class WiFlyHQTrestle: public Trestle{
public:
	WiFlyHQTrestle(char* station_identifier,char* site, int port, WiFly* wifly);
protected:
	char* _site; 
	int _port;
	WiFly* _wifly;
	boolean makePost(char* request, char* data);
	boolean waitForResponse();
};

#endif