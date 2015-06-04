
#include <iostream>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "SerialPort.h"

using namespace std;

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
	int bytesRead;

	cout << "starting echo" << endl;

	while (true)
	{
		bytesRead = sp.recv(buffer, sizeof(buffer), 10000);

		if (bytesRead > 0)
		{
			cout << "read " << bytesRead << " bytes" << endl;

			//int sleepTime = rand() % 10000;

			//usleep(sleepTime * 1000);

			int bytesSent = sp.send(buffer, bytesRead);

			if (bytesSent < bytesRead)
			{
				cout << "bytes sent != bytes read" << endl;
			}
		}
		else
		{
			perror("bytes read : ");
		}
	}
}

void handleSend()
{
	char hex[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8',
			'9', 'A', 'B', 'C', 'D', 'E', 'F', 'G'};

	unsigned char buffer[4096];
	unsigned char recvBuffer[4096];

	cout << "starting send" << endl;

	while (true)
	{
		int sleepTime = rand() % 1000;

		cout << "sleeping for " << sleepTime << " ms" << endl;

		usleep(sleepTime * 1000);

		// int bytesToSend = rand() % 4095 + 1;
		int bytesToSend = rand() % 9 + 1;

		cout << "generating " << bytesToSend << " random hex digits to send" << endl;

		for (int i = 0; i < bytesToSend; i++)
		{
			buffer[i] = hex[rand() % 15];
		}

		int bytesSent = sp.send(buffer, bytesToSend);

		if (bytesSent != bytesToSend)
		{
			cout << "wtf aren't all bytes being sent" << endl;
		}

		int bytesInBuffer = 0;

		while (true)
		{
			// int bytesRead = sp.recv(recvBuffer + bytesInBuffer, bytesToSend - bytesInBuffer, 10000);
			int bytesRead = sp.recv(recvBuffer + bytesInBuffer, 1, 10000);

			if (bytesRead <= 0)
				break;

			cout << "read " << bytesRead << endl;

			bytesInBuffer += bytesRead;

			cout << "read " << bytesInBuffer << " of " << bytesToSend << endl;

			if (bytesInBuffer == bytesToSend)
				break;
		}

		if (bytesInBuffer != bytesToSend)
		{
			cout << "expected " << bytesToSend << " bytes but received " << bytesInBuffer << endl;
			perror(" recv : ");
		}
		else
		{
			if (strncmp((const char *)buffer, (const char *)recvBuffer, bytesToSend) != 0)
			{
				cout << "buffers do not match" << endl;
			}
		}
	}
}

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		printUsage();
		return 0;
	}

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
