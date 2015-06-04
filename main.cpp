
#include <iostream>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "SerialPort.h"
#include "easylogging++.h"

using namespace std;

INITIALIZE_EASYLOGGINGPP

void printUsage()
{

	cout << "Missing parameters" << endl;
	cout << "  Usage: ./sp </path/to/dev> <echo true|false>" << endl;
	cout << "  ./sp /dev/ttyS0 true" << endl;
	cout << "echo == true will echo everything received" << endl;
}

SerialPort sp;

void handleEcho()
{
	unsigned char buffer[4096];

	cout << "starting echo" << endl;

	while (true)
	{
		int bytesInBuffer = 0;

		while (true)
		{
			int bytesRead = sp.recv(buffer + bytesInBuffer, sizeof(buffer) - bytesInBuffer, 40);

			if (bytesRead == 0 && bytesInBuffer > 0)
				break;

			bytesInBuffer += bytesRead;
		}

		if (bytesInBuffer > 0)
		{
			stringstream s;

			s << "read " << bytesInBuffer << " bytes: ";

			for (int i = 0; i < bytesInBuffer; i++)
			{
				//s << buffer[i];
			}

			LOG(INFO) << s.str();

			int bytesSent = sp.send(buffer, bytesInBuffer);

			if (bytesSent < bytesInBuffer)
			{
				LOG(ERROR) << "bytes sent != bytes read";
			}
		}
		else
		{
			perror("bytes read : ");
		}

		int sleepTime = rand() % 5000 + 40;

		LOG(INFO) << "sleeping for " << sleepTime << " ms";

		usleep(sleepTime * 1000);
	}
}

void handleSend()
{
	char hex[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8',
			'9', 'A', 'B', 'C', 'D', 'E', 'F', 'G'};

	unsigned char buffer[4096];
	unsigned char recvBuffer[4096];

	LOG(INFO) << "starting send";

	while (true)
	{
		int sleepTime = rand() % 1000 + 40;

		LOG(INFO) << "sleeping for " << sleepTime << " ms";

		usleep(sleepTime * 1000);

		int bytesToSend = rand() % 4095 + 1;
		// int bytesToSend = rand() % 9 + 1;

		stringstream s;

		s << "generating " << bytesToSend << " random hex digits to send: ";

		for (int i = 0; i < bytesToSend; i++)
		{
			buffer[i] = hex[rand() % 15];

			//s << buffer[i];
		}

		LOG(INFO) << s.str();

		int bytesSent = sp.send(buffer, bytesToSend);

		if (bytesSent != bytesToSend)
		{
			LOG(ERROR) << "bytes sent does not match bytesToSend " << bytesSent << ", " << bytesToSend;
		}

		int bytesInBuffer = 0;

		while (true)
		{
			int bytesRead = sp.recv(recvBuffer + bytesInBuffer, bytesToSend - bytesInBuffer, 10000);

			if (bytesRead <= 0)
				break;

			bytesInBuffer += bytesRead;

			if (bytesInBuffer == bytesToSend)
				break;
		}

		if (bytesInBuffer == bytesToSend)
		{
			if (strncmp((const char *)buffer, (const char *)recvBuffer, bytesToSend) != 0)
			{
				LOG(ERROR) << "buffers do not match";
			}
		}
	}
}

//===============================================================================================

void initLogging()
{

	el::Configurations c;

	c.setToDefault();

	c.setGlobally(el::ConfigurationType::ToFile, "true");
	c.setGlobally(el::ConfigurationType::Filename, "sptest.log");

	el::Loggers::setDefaultConfigurations(c, true);

	LOG(INFO) << "***************************";
	LOG(INFO) << "** logging started       **";
	LOG(INFO) << "***************************";
}

//===============================================================================================

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		printUsage();
		return 0;
	}

	initLogging();

	char *serialPortName = argv[1];
	bool echo = ((strcmp(argv[2], "true") == 0) ? true : false);

	if (!sp.open(serialPortName))
	{
		perror("failed to open serial port : ");
		return 0;
	}

	if (echo)
	{
		handleEcho();
	}
	else
	{
		handleSend();
	}

	return 0;
}
