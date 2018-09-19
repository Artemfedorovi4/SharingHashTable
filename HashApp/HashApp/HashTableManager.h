#pragma once
#include <string>
#include <map>
#include <WS2tcpip.h>

#include "MySocket.h"

class HashTableManager
{
public:
	HashTableManager();
	~HashTableManager();
	int checkFile(std::string);
	void addToAllowed(std::string, std::string);
	void addToSuspicious(std::string, std::string);
	void addToInfected(std::string, std::string);

	void sendAll(const void *buffer, int bufferLen,
		const string &foreignAddress, unsigned short foreignPort);
	void recvFrom(void *buffer, int bufferLen, string &sourceAddress,
		unsigned short &sourcePort);

private:
	std::map<std::string, std::string> allowedFileT;
	std::map<std::string, std::string> suspiciousFileT;
	std::map<std::string, std::string> infectedFileT;
	UDPSocket sock;
};

