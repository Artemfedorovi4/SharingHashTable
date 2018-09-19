#ifndef __MYSOCKET_INCLUDED__
#define __MYSOCKET_INCLUDED__
#include <string>           
#pragma comment (lib,"Ws2_32.lib")

using namespace std;

class Socket {
public:
	~Socket();
	string getLocalAddress();
	unsigned short getLocalPort();
	void setLocalPort(unsigned short localPort);
	void setLocalAddressAndPort(const string &localAddress,
		unsigned short localPort = 0);
	static void cleanUp();

private:
	Socket(const Socket &sock);
protected:
	int sockDesc;
	Socket(int type, int protocol);
	Socket(int sockDesc);
};
/*
class CommunicatingSocket : public Socket 
{
public:
	void connect(const string &foreignAddress, unsigned short foreignPort);

	string getForeignAddress();
	unsigned short getForeignPort();

protected:
	CommunicatingSocket(int type, int protocol);
	CommunicatingSocket(int newConnSD);
};*/
class UDPSocket : public Socket
{
public:
	UDPSocket();
	UDPSocket(unsigned short localPort);
	UDPSocket(const string &localAddress, unsigned short localPort);
	void sendTo(const void *buffer, int bufferLen, const string &foreignAddress,
		unsigned short foreignPort);
	int recvFrom(void *buffer, int bufferLen, string &sourceAddress,
		unsigned short &sourcePort);
private:
	void setBroadcast();
};

#endif