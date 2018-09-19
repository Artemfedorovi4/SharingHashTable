#include "stdafx.h"
#include "HashTableManager.h"


HashTableManager::HashTableManager()
{
}


HashTableManager::~HashTableManager()
{
}

int HashTableManager::checkFile(std::string hash)
{
	for (const auto& elem : allowedFileT)
	{
		if(elem.first==hash)
			return 1;
	}
	for (const auto& elem : suspiciousFileT)
	{
		if (elem.first == hash)
			return 2;
	}
	for (const auto& elem : infectedFileT)
	{
		if (elem.first == hash)
			return 3;
	}
	return 0;
}

void HashTableManager::addToAllowed(std::string fileName, std::string hash) {
	allowedFileT.insert(std::pair<std::string, std::string>(hash, fileName));
}

void HashTableManager::addToSuspicious(std::string fileName,
	std::string hash) {
	suspiciousFileT.insert(std::pair<std::string, 
		std::string>(hash, fileName));
}

void HashTableManager::addToInfected(std::string fileName, std::string hash) {
	infectedFileT.insert(std::pair<std::string, std::string>(hash, fileName));
}

void HashTableManager::sendAll(const void *buffer, int bufferLen,
	const string &foreignAddress, unsigned short foreignPort){
	 sock.sendTo((void*)buffer, bufferLen, foreignAddress, foreignPort);
}

void HashTableManager::recvFrom(void *buffer, int bufferLen,
	string &sourceAddress,	unsigned short &sourcePort){
	sock.recvFrom((void*)buffer, bufferLen, sourceAddress, sourcePort);
}



