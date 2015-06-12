#include "stdafx.h"
#include "Unity.h"
#define IPTOSBUFFERS  12 

Unity::Unity(void)
{
}

Unity::~Unity(void)
{
}
char *Unity::iptos(u_long in)
{
	static char output[IPTOSBUFFERS][3*4+3+1];
	static short which;
	u_char *p;
	p = (u_char *)&in;
	which = (which + 1 == IPTOSBUFFERS ? 0 : which + 1);
	sprintf(output[which], "%d.%d.%d.%d", p[0], p[1], p[2], p[3]);
	return output[which];
}

CString Unity::iptoCS(u_long in)
{
	CString tmp;
	tmp = iptos(in);
	return tmp;
}

char* Unity::ip6tos(struct sockaddr *sockaddr, char *address, int addrlen)
{
	int sockaddrlen;
	sockaddrlen = sizeof(struct sockaddr_in6);
	if(getnameinfo(sockaddr, sockaddrlen, address, addrlen, NULL, 0, NI_NUMERICHOST) != 0)
		address = NULL;
	return address;
}
CString Unity::ip6toCS(struct sockaddr *sockaddr)
{
	CString tmp;
	char buf[128];
	tmp = ip6tos(sockaddr, buf, sizeof(buf));
	return tmp;
}
CString Unity::ipaddrToCs(ip_address ipaddr)
{
	CString tmp;
	tmp.Format(L"%d,%d,%d,%d",ipaddr.byte1, ipaddr.byte2, ipaddr.byte3, ipaddr.byte4);
	return tmp;
}

CString Unity::ucharsToCs(u_char * uc)
{
	CString tmp;
	tmp.Format(L"(%x %x %x %x %x %x)",uc[0],uc[1],uc[2], uc[3], uc[4], uc[5]);
	return tmp;
}
CString Unity::ucharToCs(u_char uc)
{
	CString tmp;
	tmp.Format(L"%c",uc);
	return tmp;
}
CString Unity::ushortToCs(u_short us)
{
	CString tmp;
	tmp.Format(L"%d",us);
	return tmp;
}
CString Unity::uintToCs(u_int ui)
{
	CString tmp;
	tmp.Format(L"%d",ui);
	return tmp;
}