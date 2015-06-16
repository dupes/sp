/*
 * CEA2045SerialPort.h
 *
 *  Created on: Feb 19, 2015
 *      Author: rgause
 */

#ifndef CEA2045SERIALPORT_H_
#define CEA2045SERIALPORT_H_

#include <iostream>
#include <string>

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <sys/select.h>

using namespace std;

class SerialPort
{
private:
	struct termios m_tio;

	string m_serialPort;
	int m_fd;

	bool dataAvailable(int timeoutMS);

public:
	SerialPort();
	virtual ~SerialPort();

	bool open(string serialPort);

	void close();

	int flush();

	int send(unsigned char *buffer, int length);
	int recv(unsigned char *buffer, int length, int waitMS);

	int tcflow(int flag);
};

#endif /* CEA2045SERIALPORT_H_ */
