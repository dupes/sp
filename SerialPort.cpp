/*
 * CEA2045SerialPort.cpp
 *
 *  Created on: Feb 19, 2015
 *      Author: rgause
 */

#include "SerialPort.h"

SerialPort::SerialPort()
{
	m_fd = -1;
}

/*********************************************************************************/

SerialPort::~SerialPort()
{
}

/*********************************************************************************/

bool SerialPort::open(string serialPort)
{
	m_serialPort = serialPort;

	m_fd = ::open(m_serialPort.c_str(), O_RDWR | O_NOCTTY | O_NDELAY); //Open in non blocking read/write mode

	if (m_fd == -1)
		return false;

	tcgetattr(m_fd, &m_tio);

	m_tio.c_cflag = B19200 | CS8 | CLOCAL | CREAD;		//<Set baud rate
	// tio.c_cflag = B9600 | CS8 | CLOCAL | CREAD;		//<Set baud rate
	m_tio.c_iflag = IGNPAR;
	m_tio.c_oflag = 0;
	m_tio.c_lflag = 0;

	tcflush(m_fd, TCIFLUSH);

	tcsetattr(m_fd, TCSANOW, &m_tio);

	return true;
}

/*********************************************************************************/

void SerialPort::close()
{
	if (m_fd != -1)
		::close(m_fd);

	m_fd = -1;
}

/*********************************************************************************/

int SerialPort::send(unsigned char *buffer, int length)
{
	ssize_t bytesSent;
	ssize_t totalBytesSent = 0;

	while (totalBytesSent < length)
	{
		if ((bytesSent = write(m_fd, buffer, length)) < 0)
			return bytesSent;

		totalBytesSent += bytesSent;
	}

	return totalBytesSent;
}

/*********************************************************************************/

int SerialPort::flush()
{
	// TODO: implement
	return 0;
}

/*********************************************************************************/

bool SerialPort::dataAvailable(int timeoutMS)
{
	fd_set rfds;
	struct timeval tv;

	FD_ZERO(&rfds);
	FD_SET(m_fd, &rfds);

	tv.tv_sec = 0;
	tv.tv_usec = 1000 * timeoutMS;

	int retval = select(m_fd + 1, &rfds, NULL, NULL, &tv);

	if (retval == -1)
		return false;

	if (FD_ISSET(m_fd, &rfds))
		return true;

	return false;
}

/*********************************************************************************/

int SerialPort::recv(unsigned char *buffer, int length, int waitMS)
{
	if (dataAvailable(waitMS))
	{
		return read(m_fd, buffer, length);
	}

	return 0;
}

/*********************************************************************************/

int SerialPort::tcflow(int flag)
{
	return ::tcflow(m_fd, flag);
}
