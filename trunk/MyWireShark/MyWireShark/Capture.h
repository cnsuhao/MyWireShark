#pragma once
//捕获类
//by 梁文杰，2014年12月
#pragma comment (lib,"wpcap.lib") 

#include <winsock.h>
#include <map>
#include <thread>
#include <pcap.h>
#include "Unity.h"
#include "Package.h"

class CMyWireSharkDlg;
UINT ThreadFunction(LPVOID pParam);//线程函数;
static bool ThreadFlag = true;
class PARAM
{
public:
	std::map<long, Package *> * packMap;
	pcap_t *adhandle;
	CMyWireSharkDlg * dlg;
	void(CMyWireSharkDlg::* add) (Package *);
};
class Capture
{
public:
	Capture(void);
	~Capture(void);

	static enum Layer{Framenet, Ethernet, IPnet, TCPnet, UDPnet ,ARPnet, APnet};
	void initDevMap();
	void freeDevMap();
	CString getDevInfo(int num);
	CString getDevDetailInfo(int num);
	CString getPacketDetailInfo(int num ,Layer lay, int subLay);
	Package::PROTOCOL getPacketType(int num);
	int getDevNum();
	int runDev(int num);
	void setCallBack( CMyWireSharkDlg * dlg, void( CMyWireSharkDlg:: * fun) (Package *)){this->dlg = dlg; this->add = fun;}
	bool setFetFilter(char * filter);

public:
	bool isThreadPause;
	bool isRunning;
	CWinThread *thread;
private:
	CMyWireSharkDlg * dlg;
	void(CMyWireSharkDlg::* add) (Package *);
	pcap_t *adhandle;
	pcap_if_t *alldevs;
	pcap_if_t *rundev;
	std::map<int, pcap_if_t> devMap;
	std::map<long, Package *> packMap;
	int devNum;
	bool isInit;

	
};

