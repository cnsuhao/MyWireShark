#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include "PackageStruct.h"

class Unity
{
public:
	Unity(void);
	~Unity(void);
	static char *iptos(u_long in);
	static CString iptoCS(u_long in);
	static char* ip6tos(struct sockaddr *sockaddr, char *address, int addrlen);
	static CString ip6toCS(struct sockaddr *sockaddr);
	static CString ipaddrToCs(ip_address ipaddr);
	static CString ucharsToCs(u_char * ucs);
	static CString ushortToCs(u_short us);
	static CString ucharToCs(u_char uc);
	static CString uintToCs(u_int ui);
};

