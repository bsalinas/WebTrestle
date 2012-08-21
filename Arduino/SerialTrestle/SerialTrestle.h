#ifndef SERIALTRESTLE_H
#define SERIALTRESTLE_H
#include <Arduino.h>
#include <Stream.h>
#include <Trestle.h>



class SerialTrestle: public Trestle{
public:
	SerialTrestle(char* station_identifier, char* site, int port, Stream* serial);
protected:
	char* _site; 
	int _port;
	boolean makePost(char* request, char* data);
	boolean waitForResponse();
	Stream* _serial;
};

#endif