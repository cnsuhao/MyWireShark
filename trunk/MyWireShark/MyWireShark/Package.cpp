#include "stdafx.h"
#include "Package.h"

Package::Package(const struct pcap_pkthdr *header, const u_char *pkt_data)
	:header(header), pktData(pkt_data), proName(UNREALIZED)
{
	analyze();
}
Package::~Package(void)
{

}
CString Package::getProtocolName()
{
	switch (proName)
	{
	case Package::TCP:
		return L"TCP";
		break;
	case Package::UDP:
		return L"UDP";
		break;
	case Package::HTTP:
		return L"HTTP";
		break;	
	case Package::ARP:
		return L"ARP";
		break;	
	default:
		return L"UNREALIZED";
		break;
	}
}
CString Package::getPacketLen()
{
	CString tmp;
	tmp.Format(L"%d byte", header->len);
	return tmp;
}

CString Package::getSoureAddr()
{
	switch (proName)
	{
	case Package::TCP:
	case Package::UDP:
	case Package::HTTP:
		return Unity::ipaddrToCs(IPHEAD.saddr);
	case Package::ARP:
		return Unity::ipaddrToCs(ARPHEAD.saddr);
	case Package::UNREALIZED:
		break;
	default:
		return L"000000000";
		break;
	}
	return L"000000000";
}

CString Package::getDestinationAddr()
{
	switch (proName)
	{
	case Package::TCP:
	case Package::UDP:
	case Package::HTTP:
		return Unity::ipaddrToCs(IPHEAD.daddr);
	case Package::ARP:
		return Unity::ipaddrToCs(ARPHEAD.daddr);
	case Package::UNREALIZED:
		break;
	default:
		return L"000000000";
		break;
	}
	return L"000000000";
}
CString Package::getPacketTime()
{
	return time;
}
Package::PROTOCOL Package::getPacketType()
{
	return proName;
}
Package::PROTOCOL Package::analyze()
{
	struct tm *ltime;
    char timestr[16];
    time_t local_tv_sec;
    local_tv_sec = header->ts.tv_sec;
    ltime=localtime(&local_tv_sec);
    strftime( timestr, sizeof timestr, "%H:%M:%S", ltime);

	time = timestr;
	ether_header * eheader = (ether_header*)pktData;
    /* 将时间戳转换成可识别的格式 */
	Ethernet = *eheader;
	if(eheader->ether_type == htons(ETHERTYPE_IPV4))/* ipv4 packet only */
	{
		ip_header * ih = (ip_header*)(pktData+14); /* get ip header */
		IPHEAD = *ih;
		if(ih->proto == htons(TCP_PROTOCOL))  /* tcp packet only */
		{			
			tcp_header *th = (tcp_header *)(ih + sizeof(ih)); 
			proName = TCP;
			TCPHEAD = *th;
			int ip_len = ntohs(ih->tlen);
			int find_http = false;
			char* ip_pkt_data = (char*)ih;
			int n = 0;
			char buffer[2048];
			int bufsize = 0;

			for(; n<ip_len; n++)
			{
				if(!find_http && ((n+3<ip_len && strncmp(ip_pkt_data+n,"GET",strlen("GET")) ==0 )
					|| (n+4<ip_len && strncmp(ip_pkt_data+n,"POST",strlen("POST")) == 0)) )
					find_http = true;

				if(!find_http && n+8<ip_len && strncmp(ip_pkt_data+n,"HTTP/1.1",strlen("HTTP/1.1"))==0)
					find_http = true;
				if(find_http)
				{
					buffer[bufsize] = ip_pkt_data[n]; 
					bufsize ++;
				}
			}
			if(find_http)
			{
				HttpContent = buffer;
				proName = HTTP;
			}
			return proName;
		}
		else if(ih->proto == htons(UDP_PROTOCOL))
		{
			udp_header *uh = (udp_header*)(ih + sizeof(ih));
			UDPHEAD = *uh;
			proName = UDP;
			return UDP;
		}
		return UNREALIZED;
	}
	else if(eheader->ether_type == htons(ETHERTYPE_IPV6))/* ipv6 packet only */
	{
		ip6_header *ih6 = (ip6_header *)(pktData+14);
		proName = IPV6;
		return IPV6;

	}
	else if(eheader->ether_type == htons(ARP_PROTOCOL))/* arp packet */
	{
		arp_header *ah = (arp_header *)(pktData+14);; 
		proName = ARP;
		ARPHEAD = *ah;
		return ARP;
	}
	return UNREALIZED;
}


CString Package::getFrameDetail(PHYSICAL ph)
{

	CString time, caplen, packlen;
	time = "Arrival Time : ";
	time += getPacketTime();;
	caplen.Format(L"Capture Len : %d byte", header->caplen);
	packlen = "Frame Len : ";
	packlen += getPacketLen();
	switch (ph)
	{
	case Package::PH_TIME:
		return time;
	case Package::PH_CAPLEN:
		return caplen;
	case Package::PH_PACKLEN:
		return packlen;
	case Package::PH_ALL:
		return time + caplen + packlen;
	default:
		return L"PH_ERROR";
		break;
	}	
	return L"";
}
CString Package::getEthernetDetail(ETHERNET en)
{
	switch (en)
	{
	case Package::EN_SADDR:
		return L"Source: " + Unity::ucharsToCs(Ethernet.ether_shost);
	case Package::EN_DADDR:
		return L"Destination: "  + Unity::ucharsToCs(Ethernet.ether_dhost);
	case Package::EN_TYPE:
		if(Ethernet.ether_type == htons(ETHERTYPE_IPV4))
			return L"Type: IP(0x0800)" ;
		else if(Ethernet.ether_type == htons(ETHERTYPE_IPV6))
			return L"Type: IPv6(0x86dd)";
		else if(Ethernet.ether_type == htons(ARP_PROTOCOL))
			return L"Type: ARP(0x0806)";
	case Package::ENALL:
		break;
	default:
		break;
	}
	return L"";
}
CString Package::getIpnetDetail(IPNET in)
{
	switch (in)
	{
	case Package::IN_IPLEN:
		return L"Total Len:" + Unity::ushortToCs(IPHEAD.tlen);
	case Package::IN_IDENTIFICATION:
		return L"Identification:" + Unity::ushortToCs(IPHEAD.identification);
	case Package::IN_TIMETOLIVE:
		return L"Time to live:" + Unity::ucharToCs(IPHEAD.ttl);
	case Package::IN_PRO:
		return L"Protocol:" + getProtocolName();
	case Package::IN_HEADSUM:
		return L"Head Check Sum:" + Unity::ushortToCs(IPHEAD.crc);
	case Package::IN_SIPADDR:
		return L"Source: " + getSoureAddr();
	case Package::IN_DIPADDR:
		return L"Destination: "  + getDestinationAddr();
	default:
		break;
	}
	return L"";
}

CString Package::getApnetDetail(APNET ap)
{
	switch (ap)
	{
	case Package::AP_HTTP:
		return HttpContent;
	default:
		break;
	}
	return L"";
}
CString Package::getARPDetail(ARPPRO arp)
{
	switch (arp)
	{
	case Package::ARP_HARDTYPE:
		return L"Hard Type:" + Unity::ushortToCs(ARPHEAD.hard_type);
	case Package::ARP_PROTYPE:
		return L"Protocol Type:" + Unity::ushortToCs(ARPHEAD.protocol_type);
	case Package::ARP_HARDSIZE:
		return L"Hard Size:" + Unity::ucharToCs(ARPHEAD.hard_size);
	case Package::ARP_PROSIZE:
		return L"Protocol Size:" +  Unity::ucharToCs(ARPHEAD.protocol_size);
	case Package::ARP_OPCODE:
		return L"Opcode:" + Unity::ushortToCs(ARPHEAD.opcode);
	case Package::ARP_SMACADDR:
		return L"Sender MAC address:" + Unity::ucharsToCs(ARPHEAD.ether_shost);
	case Package::ARP_SIPADDR:
		return L"Sender IP address: " + Unity::ipaddrToCs(ARPHEAD.saddr);
	case Package::ARP_DMACADDR:
		return L"Target MAC address:" + Unity::ucharsToCs(ARPHEAD.ether_dhost);
	case Package::ARP_DIPZDDR:
		return L"Target IP address: "  + Unity::ipaddrToCs(ARPHEAD.saddr);
	default:
		break;
	}
	return L"";
}
CString Package::getTransTCPDetail(TRANSTCP trtcp)
{
	switch (trtcp)
	{
	case Package::TR_SPORT:
		return L"Source port:" + Unity::ushortToCs(TCPHEAD.th_sport);
	case Package::TP_DPORT:
		return L"Destination port:" + Unity::ushortToCs(TCPHEAD.th_dport);
	case Package::TP_SEQNUM:
		return L"Sequence number:" + Unity::uintToCs(TCPHEAD.th_seq);
	case Package::TP_ACKNUM:
		return L"Acknowledgement number:" + Unity::uintToCs(TCPHEAD.th_ack);
	case Package::TP_LEN:
		return L"Datagram length and reserved code :" + Unity::ushortToCs(TCPHEAD.th_len_resv_code);
	case Package::TP_WINSIZE:
		return L"Window Size Value:" + Unity::ushortToCs(TCPHEAD.th_window);
	case Package::TP_CHECKSUM:
		return L"Checksum:" + Unity::ushortToCs(TCPHEAD.th_sum);
	default:
		break;
	}
	return L"";
}
CString Package::getTransUDPDetail(TRANSUDP trudp)
{
	switch (trudp)
	{
	case Package::TRUDP_SPORT:
		return L"Source port:" + Unity::ushortToCs(UDPHEAD.sport);
	case Package::TPUDP_DPORT:
		return L"Destination port:" + Unity::ushortToCs(UDPHEAD.dport);
	case Package::TPUDP_LEN:
		return L"Total Len:" + Unity::ushortToCs(UDPHEAD.len);
	case Package::TPUDP_CHECKSUM:
		return L"Checksum:" + Unity::ushortToCs(UDPHEAD.crc);
	default:
		break;
	}
	return L"";
}