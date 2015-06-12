#pragma once
//数据包结构
//by 梁文杰，2014年12月
#include <pcap.h>
#include <string.h>
#include "Unity.h"
class Package
{
public:
	static enum PROTOCOL{TCP, UDP, HTTP, ARP, IPV6, UNREALIZED};
	static enum PHYSICAL{PH_TIME, PH_CAPLEN, PH_PACKLEN, PH_ALL};
	static enum ETHERNET{EN_SADDR, EN_DADDR, EN_TYPE, ENALL};
	static enum IPNET{IN_IPLEN, IN_IDENTIFICATION, IN_TIMETOLIVE, IN_PRO, IN_HEADSUM, IN_SIPADDR, IN_DIPADDR};
	static enum TRANSTCP{TR_SPORT, TP_DPORT, TP_SEQNUM, TP_ACKNUM, TP_LEN, TP_WINSIZE, TP_CHECKSUM};
	static enum TRANSUDP{TRUDP_SPORT, TPUDP_DPORT,TPUDP_LEN, TPUDP_CHECKSUM};
	static enum ARPPRO{ARP_HARDTYPE, ARP_PROTYPE, ARP_HARDSIZE, ARP_PROSIZE, ARP_OPCODE, ARP_SMACADDR, ARP_SIPADDR, ARP_DMACADDR, ARP_DIPZDDR};
	static enum APNET{AP_HTTP}; 
	Package( const struct pcap_pkthdr *header, const u_char *pkt_data);
	~Package(void);
	CString getProtocolName();
	CString getSoureAddr();
	CString getDestinationAddr();
	CString getPacketLen();
	CString getPacketTime();
	PROTOCOL getPacketType();


	CString getFrameDetail(PHYSICAL ph);
	CString getEthernetDetail(ETHERNET en);
	CString getIpnetDetail(IPNET in);
	CString getTransTCPDetail(TRANSTCP trtcp);
	CString getTransUDPDetail(TRANSUDP trudp);
	CString getApnetDetail(APNET ap);
	CString getARPDetail(ARPPRO arp);
private:
	CString time;
	ether_header Ethernet;
	ip_header IPHEAD;
	tcp_header TCPHEAD;
	udp_header UDPHEAD;
	arp_header ARPHEAD;
	CString HttpContent;
	PROTOCOL analyze();
	PROTOCOL proName;
	const struct pcap_pkthdr *header;
	const u_char *pktData;
};

