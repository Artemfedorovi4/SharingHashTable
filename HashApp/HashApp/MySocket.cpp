#include "stdafx.h"
#include "MySocket.h"
#include <winsock.h> 
typedef int socklen_t;
typedef char raw_type;

using namespace std;
static bool initialized = false;

static void fillAddr(const string &address, unsigned short port,
	sockaddr_in &addr) {
	memset(&addr, 0, sizeof(addr));  
	addr.sin_family = AF_INET;       

	hostent *host;  
	if ((host = gethostbyname(address.c_str())) == NULL) {
		return;
	}
	addr.sin_addr.s_addr = *((unsigned long *)host->h_addr_list[0]);

	addr.sin_port = htons(port); 
}

Socket::Socket(int type, int protocol)   {
	if (!initialized) {
		WORD wVersionRequested;
		WSADATA wsaData;

		wVersionRequested = MAKEWORD(2, 0);              
		if (WSAStartup(wVersionRequested, &wsaData) != 0) {  
			return ;
		}
		initialized = true;
	}

	// Make a new socket
	if ((sockDesc = socket(PF_INET, type, protocol)) < 0) {
		return;
	}
}

Socket::Socket(int sockDesc) {
	this->sockDesc = sockDesc;
}

Socket::~Socket() {
	::closesocket(sockDesc);
	sockDesc = -1;
}

string Socket::getLocalAddress()   {
	sockaddr_in addr;
	unsigned int addr_len = sizeof(addr);

	if (getsockname(sockDesc, (sockaddr *)&addr, (socklen_t *)&addr_len) < 0) {
		printf("Error of get local address");
		exit(0);
	}
	return inet_ntoa(addr.sin_addr);
}

unsigned short Socket::getLocalPort()   {
	sockaddr_in addr;
	unsigned int addr_len = sizeof(addr);

	if (getsockname(sockDesc, (sockaddr *)&addr, (socklen_t *)&addr_len) < 0) {
		printf("Error of get local port");
		exit(0);
	}
	return ntohs(addr.sin_port);
}

void Socket::setLocalPort(unsigned short localPort)   {
	// Bind the socket to its port
	sockaddr_in localAddr;
	memset(&localAddr, 0, sizeof(localAddr));
	localAddr.sin_family = AF_INET;
	localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	localAddr.sin_port = htons(localPort);

	if (bind(sockDesc, (sockaddr *)&localAddr, sizeof(sockaddr_in)) < 0) {
		printf("Error of set local port");
		return;
	}
}

void Socket::setLocalAddressAndPort(const string &localAddress,
	unsigned short localPort)   {
	// Get the address of the requested host
	sockaddr_in localAddr;
	fillAddr(localAddress, localPort, localAddr);

	if (bind(sockDesc, (sockaddr *)&localAddr, sizeof(sockaddr_in)) < 0) {
		printf("Error of set local address");
		return;
	}
}

void Socket::cleanUp()   {
#ifdef WIN32
	if (WSACleanup() != 0) {
		printf("WSAS cleanup error");
		return;
	}
#endif
}
// CommunicatingSocket Code
/*
CommunicatingSocket::CommunicatingSocket(int type, int protocol)
  : Socket(type, protocol) {
}

CommunicatingSocket::CommunicatingSocket(int newConnSD) : Socket(newConnSD) {
}

void CommunicatingSocket::connect(const string &foreignAddress,
	unsigned short foreignPort)   {
	// Get the address of the requested host
	sockaddr_in destAddr;
	fillAddr(foreignAddress, foreignPort, destAddr);

	// Try to connect to the given port
	if (::connect(sockDesc, (sockaddr *)&destAddr, sizeof(destAddr)) < 0) {
		printf("Error of getting address of the request host");
		return;
	}
}

string CommunicatingSocket::getForeignAddress()
  {
	sockaddr_in addr;
	unsigned int addr_len = sizeof(addr);

	if (getpeername(sockDesc, (sockaddr *)&addr, (socklen_t *)&addr_len) < 0) {
		printf("Error of getting Foreign address");
		exit(0);
	}
	return inet_ntoa(addr.sin_addr);
}

unsigned short CommunicatingSocket::getForeignPort()   {
	sockaddr_in addr;
	unsigned int addr_len = sizeof(addr);

	if (getpeername(sockDesc, (sockaddr *)&addr, (socklen_t *)&addr_len) < 0) {
		return 0;
	}
	return ntohs(addr.sin_port);
}
*/
UDPSocket::UDPSocket()   : Socket(SOCK_DGRAM,
	IPPROTO_UDP) {
	setBroadcast();
}

UDPSocket::UDPSocket(unsigned short localPort)    :
	Socket(SOCK_DGRAM, IPPROTO_UDP) {
	setLocalPort(localPort);
	setBroadcast();
}

UDPSocket::UDPSocket(const string &localAddress, unsigned short localPort)
  : Socket(SOCK_DGRAM, IPPROTO_UDP) {
	setLocalAddressAndPort(localAddress, localPort);
	setBroadcast();
}

void UDPSocket::setBroadcast() {
	int broadcastPermission = 1;
	setsockopt(sockDesc, SOL_SOCKET, SO_BROADCAST,
		(raw_type *)&broadcastPermission, sizeof(broadcastPermission));
}
void UDPSocket::sendTo(const void *buffer, int bufferLen,
	const string &foreignAddress, unsigned short foreignPort)
	  {
	sockaddr_in destAddr;
	fillAddr(foreignAddress, foreignPort, destAddr);

	if (sendto(sockDesc, (raw_type *)buffer, bufferLen, 0,
		(sockaddr *)&destAddr, sizeof(destAddr)) != bufferLen) {
		return;
	}
}

int UDPSocket::recvFrom(void *buffer, int bufferLen, string &sourceAddress,
	unsigned short &sourcePort)   {
	sockaddr_in clntAddr;
	socklen_t addrLen = sizeof(clntAddr);
	int rtn;
	if ((rtn = recvfrom(sockDesc, (raw_type *)buffer, bufferLen, 0,
		(sockaddr *)&clntAddr, (socklen_t *)&addrLen)) < 0) {
		return -1;
	}
	sourceAddress = inet_ntoa(clntAddr.sin_addr);
	sourcePort = ntohs(clntAddr.sin_port);

	return rtn;
}