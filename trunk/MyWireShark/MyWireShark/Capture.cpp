#include "stdafx.h"
#include "Capture.h"


Capture::Capture(void)
	:isInit(false), isThreadPause(false) ,isRunning(false)
{
	initDevMap();
}


Capture::~Capture(void)
{
	freeDevMap();
	ThreadFlag = false;
}
void Capture::initDevMap()
{
	pcap_if_t *d;
	int i = 0;
	devNum = 0;
	char errbuf[PCAP_ERRBUF_SIZE];
	if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL /* auth is not needed */, &alldevs, errbuf) == -1)
	{
		TRACE("Error in pcap_findalldevs_ex: %s\n", errbuf);
		exit(1);
	}
	for (d = alldevs; d != NULL; d = d->next)
	{
		devNum ++;
		devMap[++i] = *d;
	}

	if (i == 0)
	{
		AfxMessageBox(L"\nNo interfaces found! Make sure WinPcap is installed.\n");
		return;
	}
	isInit = true;
	return;
}

void Capture::freeDevMap()
{
	/* We don't need any more the device list. Free it */
	devMap.clear();
	pcap_freealldevs(alldevs);
}
CString Capture::getDevInfo(int num)
{
	CString arm;
	arm = devMap[num].name;
	arm += devMap[num].description;
	return arm;
}

CString Capture::getDevDetailInfo(int num)
{
	CString arm, name, desc, protocolCluster, addr, Netmask ,Broadaddr, Dstaddr;
	name = "名称：";
	name += devMap[num].name;
	name += "\r\n";
	desc = "设备描述：";
	desc += devMap[num].description;
	desc += "\r\n";
	protocolCluster = "协议簇：";
	addr = "地址：";
	switch (devMap[num].addresses->addr->sa_family)
	{
	case AF_INET:
		Netmask = "网关：";
		Broadaddr = "广播地址:";
		Dstaddr = "目标地址:";
		protocolCluster += "AFINET\r\n";
		if(devMap[num].addresses->addr)
			addr += Unity::iptoCS(((struct sockaddr_in *)devMap[num].addresses->addr)->sin_addr.S_un.S_addr);
		addr += "\r\n";
		if(devMap[num].addresses->netmask)
			Netmask +=  Unity::iptoCS(((struct sockaddr_in *)devMap[num].addresses->netmask)->sin_addr.S_un.S_addr);
		Netmask += "\r\n";
		if(devMap[num].addresses->broadaddr)
			Broadaddr += Unity::iptoCS(((struct sockaddr_in *)devMap[num].addresses->broadaddr)->sin_addr.S_un.S_addr);
		Broadaddr += "\r\n";
		if(devMap[num].addresses->dstaddr)
			Dstaddr += Unity::iptoCS(((struct sockaddr_in *)devMap[num].addresses->dstaddr)->sin_addr.S_un.S_addr);
		break;
	case AF_INET6:
		protocolCluster += "AFINET6\r\n";
		if(devMap[num].addresses->addr)
			addr += Unity::ip6toCS(devMap[num].addresses->addr);
		addr += "\r\n";
		break;
	default:
		arm = "地址簇未知";
		return arm;
		break;
	}
	arm = name + desc + protocolCluster + addr + Netmask + Broadaddr + Dstaddr;
	return arm;
}
CString Capture::getPacketDetailInfo(int num, Capture::Layer lay, int subLay)
{
	if(num < 0)
		return L"0";
	switch (lay)
	{
	case Capture::Framenet:
		{
			switch(subLay)
			{
			case 0:
				return packMap[num]->getFrameDetail(Package::PH_TIME);
			case 1:
				return packMap[num]->getFrameDetail(Package::PH_CAPLEN);
			case 2:
				return packMap[num]->getFrameDetail(Package::PH_PACKLEN);
			}
		}
		break;
	case Capture::Ethernet:
		{
			switch(subLay)
			{
			case 0:
				return packMap[num]->getEthernetDetail(Package::EN_SADDR);
			case 1:
				return packMap[num]->getEthernetDetail(Package::EN_DADDR);
			case 2:
				return packMap[num]->getEthernetDetail(Package::EN_TYPE);
			}
		}
		break;
	case Capture::IPnet:
		{
			switch(subLay)
			{
			case 0:
				return packMap[num]->getIpnetDetail(Package::IN_IPLEN);
			case 1:
				return packMap[num]->getIpnetDetail(Package::IN_IDENTIFICATION);
			case 2:
				return packMap[num]->getIpnetDetail(Package::IN_TIMETOLIVE);
			case 3:
				return packMap[num]->getIpnetDetail(Package::IN_PRO);
			case 4:
				return packMap[num]->getIpnetDetail(Package::IN_HEADSUM);
			case 5:
				return packMap[num]->getIpnetDetail(Package::IN_SIPADDR);
			case 6:
				return packMap[num]->getIpnetDetail(Package::IN_DIPADDR);

			}
		}
		break;
	case Capture::TCPnet:
		{
			switch(subLay)
			{
			case 0:
				return packMap[num]->getTransTCPDetail(Package::TR_SPORT);
			case 1:
				return packMap[num]->getTransTCPDetail(Package::TP_DPORT);
			case 2:
				return packMap[num]->getTransTCPDetail(Package::TP_SEQNUM);
			case 3:
				return packMap[num]->getTransTCPDetail(Package::TP_ACKNUM);
			case 4:
				return packMap[num]->getTransTCPDetail(Package::TP_LEN);
			case 5:
				return packMap[num]->getTransTCPDetail(Package::TP_WINSIZE);
			case 6:
				return packMap[num]->getTransTCPDetail(Package::TP_CHECKSUM);

			}
		}
		break;
	case Capture::UDPnet:
		{
			switch(subLay)
			{
			case 0:
				return packMap[num]->getTransUDPDetail(Package::TRUDP_SPORT);
			case 1:
				return packMap[num]->getTransUDPDetail(Package::TPUDP_DPORT);
			case 2:
				return packMap[num]->getTransUDPDetail(Package::TPUDP_LEN);
			case 3:
				return packMap[num]->getTransUDPDetail(Package::TPUDP_CHECKSUM);

			}
		break;
		}
	case Capture::APnet:
			switch(subLay)
			{
			case 0:
				return packMap[num]->getApnetDetail(Package::AP_HTTP);
			}
		break;
	case Capture::ARPnet:
		{
			switch(subLay)
			{
			case 0:
				return packMap[num]->getARPDetail(Package::ARP_HARDTYPE);
			case 1:
				return packMap[num]->getARPDetail(Package::ARP_PROTYPE);
			case 2:
				return packMap[num]->getARPDetail(Package::ARP_HARDSIZE);
			case 3:
				return packMap[num]->getARPDetail(Package::ARP_PROSIZE);
			case 4:
				return packMap[num]->getARPDetail(Package::ARP_OPCODE);
			case 5:
				return packMap[num]->getARPDetail(Package::ARP_SMACADDR);
			case 6:
				return packMap[num]->getARPDetail(Package::ARP_SIPADDR);
			case 7:
				return packMap[num]->getARPDetail(Package::ARP_DMACADDR);
			case 8:
				return packMap[num]->getARPDetail(Package::ARP_DIPZDDR);

			}
		}
	default:
		return L"";
	}	
	return L"";
}
bool Capture::setFetFilter(char * filter)
{
	if(!isInit || !isRunning)
		return false;
	else
	{
		if(!isThreadPause)
		{
			thread->SuspendThread();
			isThreadPause = true;
		}
		u_int netmask;
		struct bpf_program fcode ;
		if (rundev->addresses != NULL)
			netmask = ((struct sockaddr_in *)(rundev->addresses->netmask))->sin_addr.S_un.S_addr;
		else
			netmask = 0xffffff;
		if (pcap_compile(adhandle, &fcode, filter, 1, netmask) < 0)
		{
			pcap_freealldevs(alldevs);
			return false;
		}
		if (pcap_setfilter(adhandle, &fcode) < 0)
		{
			pcap_freealldevs(alldevs);
			return false;
		}
		if(isThreadPause)
		{
			thread->ResumeThread();
			isThreadPause = false;
		}
		
	}
}
int Capture::getDevNum()
{
	return devNum;
}
Package::PROTOCOL Capture::getPacketType(int num)
{
	return packMap[num]->getPacketType();
}
int Capture::runDev(int num)
{
	if(!isInit || num <= 0)
		return -1;
	char errbuf[PCAP_ERRBUF_SIZE];
	if((adhandle = pcap_open(devMap[num].name, 65535, PCAP_OPENFLAG_PROMISCUOUS, 1000, NULL, errbuf)) == NULL)
	{
		AfxMessageBox(L"无法打开该适配器！！");
		return -1;
	}
	rundev = &devMap[num];
	AfxMessageBox(L"开始抓包!!!!");
	PARAM *param= new PARAM;
	param->add = this->add;
	param->adhandle = adhandle;
	param->dlg = this->dlg;
	param->packMap = & (this->packMap);
	thread = AfxBeginThread(ThreadFunction, (LPVOID)param);//启动线程
	isRunning = true;
	//setFetFilter("udp");
	return 0;
}
UINT ThreadFunction(LPVOID pParam)//线程函数
{
	PARAM *Param = (PARAM *)pParam;
	while(ThreadFlag)
	{
		int res;
		struct pcap_pkthdr *header;
		const u_char *pkt_data;
		static long Num = 0;
		while((res = pcap_next_ex( Param->adhandle, &header, &pkt_data)) >= 0)
		{
			if(res == 0)
				continue;
			else
			{
				Package *pack = new Package(header, pkt_data);
				(* (Param->packMap))[++Num] = pack;
				(Param->dlg->*(Param->add))(pack);
			}

		}

		if(res == -1){
			printf("Error reading the packets: %s\n", pcap_geterr(Param->adhandle));
			return -1;
		}
	}
	return 0;
}