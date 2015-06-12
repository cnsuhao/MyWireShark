
// MyWireSharkDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyWireShark.h"
#include "MyWireSharkDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMyWireSharkDlg �Ի���



CMyWireSharkDlg::CMyWireSharkDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMyWireSharkDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyWireSharkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NETCARD_LIST, netCardList);
	DDX_Control(pDX, IDC_STATIC_INFO, devInfo);
	DDX_Control(pDX, IDC_LIST_CAPTURE, capturedList);
	DDX_Control(pDX, IDC_TREE_PackInfo, packageInfo);
	DDX_Control(pDX, IDC_EDIT_FILTER, filterEdit);
}

BEGIN_MESSAGE_MAP(CMyWireSharkDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_LBN_SELCHANGE(IDC_NETCARD_LIST, &CMyWireSharkDlg::OnLbnSelchangeNetcardList)
	ON_LBN_DBLCLK(IDC_NETCARD_LIST, &CMyWireSharkDlg::OnLbnDblclkNetcardList)
	ON_NOTIFY(NM_CLICK, IDC_LIST_CAPTURE, &CMyWireSharkDlg::OnNMClickListCapture)
	ON_BN_CLICKED(IDC_BT_STARCAP, &CMyWireSharkDlg::OnBnClickedBtStarcap)
	ON_BN_CLICKED(IDC_BT_STOPCAP, &CMyWireSharkDlg::OnBnClickedBtStopcap)
	ON_BN_CLICKED(IDC_BT_TOTOP, &CMyWireSharkDlg::OnBnClickedBtTotop)
	ON_BN_CLICKED(IDC_BT_BOTTOM, &CMyWireSharkDlg::OnBnClickedBtBottom)
	ON_BN_CLICKED(IDC_BT_FILTER, &CMyWireSharkDlg::OnBnClickedBtFilter)
END_MESSAGE_MAP()


// CMyWireSharkDlg ��Ϣ�������

BOOL CMyWireSharkDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	initNetCartList();
	initCaptureList();
	cap.setCallBack(&(*this),&CMyWireSharkDlg::addPackageToList);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMyWireSharkDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMyWireSharkDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMyWireSharkDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMyWireSharkDlg::initNetCartList()
{
	CString arm;
	int len = cap.getDevNum();
	for (int i = 1 ; i <= len; i++)
	{
		arm = cap.getDevInfo(i);
		netCardList.AddString(arm);
	}
}
void CMyWireSharkDlg::initCaptureList()
{
	capturedList.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	LONG lStyle;
	lStyle = GetWindowLong(capturedList.m_hWnd, GWL_STYLE);//��ȡ��ǰ����style
	lStyle &= ~LVS_TYPEMASK; //�����ʾ��ʽλ
	lStyle |= LVS_REPORT; //����style
	SetWindowLong(capturedList.m_hWnd, GWL_STYLE, lStyle);//����style
	capturedList.InsertColumn(0, L"Time", LVCFMT_LEFT, 100);
	capturedList.InsertColumn(1, L"Protocol", LVCFMT_LEFT, 100);
	capturedList.InsertColumn(2, L"SourceAddr", LVCFMT_LEFT, 100);
	capturedList.InsertColumn(3, L"Destination", LVCFMT_LEFT, 100);
	capturedList.InsertColumn(4, L"DataLen", LVCFMT_LEFT, 100);
}

void CMyWireSharkDlg::addFrameAndEther(int num)
{
	HTREEITEM FrameRoot;     // ���ĸ��ڵ�ľ��   
	HTREEITEM EthernetRoot;
	FrameRoot = packageInfo.InsertItem(_T("Frame"), 0, 0);
	CString frameTime = cap.getPacketDetailInfo(num, Capture::Framenet, 0);
	packageInfo.InsertItem(frameTime, 1, 1, FrameRoot, TVI_LAST);  
	CString frameCaplen = cap.getPacketDetailInfo(num, Capture::Framenet, 1);
	packageInfo.InsertItem(frameCaplen, 1, 1, FrameRoot, TVI_LAST);
	CString framePacklen = cap.getPacketDetailInfo(num, Capture::Framenet, 2);
	packageInfo.InsertItem(framePacklen, 1, 1, FrameRoot, TVI_LAST);

	EthernetRoot = packageInfo.InsertItem(_T("Ethernet"), 0, 0);
	CString ethernetSaddr = cap.getPacketDetailInfo(num, Capture::Ethernet, 0);
	packageInfo.InsertItem(ethernetSaddr, 1, 1, EthernetRoot, TVI_LAST); 
	CString ethernetDaddr = cap.getPacketDetailInfo(num, Capture::Ethernet, 1);
	packageInfo.InsertItem(ethernetDaddr, 1, 1, EthernetRoot, TVI_LAST); 
	CString ethernetType = cap.getPacketDetailInfo(num, Capture::Ethernet, 2);
	packageInfo.InsertItem(ethernetType, 1, 1, EthernetRoot, TVI_LAST);
}
void CMyWireSharkDlg::addIpv4Tree(int num)
{
	HTREEITEM IPv4Root;  
	IPv4Root = packageInfo.InsertItem(_T("Internet Protocol Vesion 4"), 0, 0);

	CString ipnetlen = cap.getPacketDetailInfo(num, Capture::IPnet, 0);
	packageInfo.InsertItem(ipnetlen, 1, 1, IPv4Root, TVI_LAST); 

	CString Identification = cap.getPacketDetailInfo(num, Capture::IPnet, 1);
	packageInfo.InsertItem(Identification, 1, 1, IPv4Root, TVI_LAST); 

	CString timeToLive = cap.getPacketDetailInfo(num, Capture::IPnet, 2);
	packageInfo.InsertItem(timeToLive, 1, 1, IPv4Root, TVI_LAST);

	CString Protocol = cap.getPacketDetailInfo(num, Capture::IPnet, 3);
	packageInfo.InsertItem(Protocol, 1, 1, IPv4Root, TVI_LAST);
	CString headCheckSum = cap.getPacketDetailInfo(num, Capture::IPnet, 4);
	packageInfo.InsertItem(headCheckSum, 1, 1, IPv4Root, TVI_LAST); 

	CString Source = cap.getPacketDetailInfo(num, Capture::IPnet, 5);
	packageInfo.InsertItem(Source, 1, 1, IPv4Root, TVI_LAST);

	CString Destination = cap.getPacketDetailInfo(num, Capture::IPnet, 6);
	packageInfo.InsertItem(Destination, 1, 1, IPv4Root, TVI_LAST);
}
void CMyWireSharkDlg::addTcpTree(int num)
{
	addFrameAndEther(num);
	addIpv4Tree(num);
	HTREEITEM TcpRoot;
	TcpRoot = packageInfo.InsertItem(_T("Transmission Control Protocol"), 0, 0);

	CString sport = cap.getPacketDetailInfo(num, Capture::TCPnet, 0);
	packageInfo.InsertItem(sport, 1, 1, TcpRoot, TVI_LAST); 

	CString dport = cap.getPacketDetailInfo(num, Capture::TCPnet, 1);
	packageInfo.InsertItem(dport, 1, 1, TcpRoot, TVI_LAST);

	CString seq = cap.getPacketDetailInfo(num, Capture::TCPnet, 2);
	packageInfo.InsertItem(seq, 1, 1, TcpRoot, TVI_LAST);

	CString ack = cap.getPacketDetailInfo(num, Capture::TCPnet, 3);
	packageInfo.InsertItem(ack, 1, 1, TcpRoot, TVI_LAST); 

	CString len = cap.getPacketDetailInfo(num, Capture::TCPnet, 4);
	packageInfo.InsertItem(len, 1, 1, TcpRoot, TVI_LAST);

	CString windows = cap.getPacketDetailInfo(num, Capture::TCPnet, 5);
	packageInfo.InsertItem(windows, 1, 1, TcpRoot, TVI_LAST);

	CString checkSum = cap.getPacketDetailInfo(num, Capture::TCPnet, 6);
	packageInfo.InsertItem(checkSum, 1, 1, TcpRoot, TVI_LAST);

}
void CMyWireSharkDlg::addUdpTree(int num)
{
	addFrameAndEther(num);
	addIpv4Tree(num);
	HTREEITEM UdpRoot;
	UdpRoot = packageInfo.InsertItem(_T("User Datagram Protocol"), 0, 0);

	CString sport = cap.getPacketDetailInfo(num, Capture::UDPnet, 0);
	packageInfo.InsertItem(sport, 1, 1, UdpRoot, TVI_LAST); 

	CString dport = cap.getPacketDetailInfo(num, Capture::UDPnet, 1);
	packageInfo.InsertItem(dport, 1, 1, UdpRoot, TVI_LAST);

	CString len = cap.getPacketDetailInfo(num, Capture::UDPnet, 2);
	packageInfo.InsertItem(len, 1, 1, UdpRoot, TVI_LAST);

	CString checksum = cap.getPacketDetailInfo(num, Capture::UDPnet, 3);
	packageInfo.InsertItem(checksum, 1, 1, UdpRoot, TVI_LAST); 
}
void CMyWireSharkDlg::addArpTree(int num)
{
	addFrameAndEther(num);
	HTREEITEM arpRoot;
	arpRoot =  packageInfo.InsertItem(_T("Address Resolution Protocol"), 0, 0);

	CString hard_type = cap.getPacketDetailInfo(num, Capture::ARPnet, 0);
	packageInfo.InsertItem(hard_type, 1, 1, arpRoot, TVI_LAST); 

	CString protocol_type = cap.getPacketDetailInfo(num, Capture::ARPnet, 1);
	packageInfo.InsertItem(protocol_type, 1, 1, arpRoot, TVI_LAST);

	CString hard_size = cap.getPacketDetailInfo(num, Capture::ARPnet, 2);
	packageInfo.InsertItem(hard_size, 1, 1, arpRoot, TVI_LAST);

	CString protocol_size = cap.getPacketDetailInfo(num, Capture::ARPnet, 3);
	packageInfo.InsertItem(protocol_size, 1, 1, arpRoot, TVI_LAST); 

	CString opcode = cap.getPacketDetailInfo(num, Capture::ARPnet, 4);
	packageInfo.InsertItem(opcode, 1, 1, arpRoot, TVI_LAST); 

	CString ether_shost = cap.getPacketDetailInfo(num, Capture::ARPnet, 5);
	packageInfo.InsertItem(ether_shost, 1, 1, arpRoot, TVI_LAST); 

	CString saddr = cap.getPacketDetailInfo(num, Capture::ARPnet, 6);
	packageInfo.InsertItem(saddr, 1, 1, arpRoot, TVI_LAST); 

	CString ether_dhost = cap.getPacketDetailInfo(num, Capture::ARPnet, 7);
	packageInfo.InsertItem(ether_dhost, 1, 1, arpRoot, TVI_LAST); 

	CString daddr = cap.getPacketDetailInfo(num, Capture::ARPnet, 8);
	packageInfo.InsertItem(daddr, 1, 1, arpRoot, TVI_LAST); 


}
void CMyWireSharkDlg::addHttpTree(int num)
{
	addTcpTree(num);
	HTREEITEM HttpRoot;
	HttpRoot = packageInfo.InsertItem(_T("Hypertext Tranfer Protocol"), 0, 0);
	CString httpContent = cap.getPacketDetailInfo(num, Capture::APnet, 0);
	packageInfo.InsertItem(httpContent, 1, 1, HttpRoot, TVI_LAST);
	
}
void CMyWireSharkDlg::addPackageToList(Package *pack)
{
	int nCount = capturedList.GetItemCount();//��ȡ��ǰ�Ѳ��������
	capturedList.SetTextBkColor(RGB(100, 200, 255));
	capturedList.InsertItem(nCount, _T(""));//����һ��
	capturedList.SetItemText(nCount, 0, pack->getPacketTime());//��һ��
	capturedList.SetItemText(nCount, 1, pack->getProtocolName());//��һ��
	capturedList.SetItemText(nCount, 2, pack->getSoureAddr());//�ڶ���
	capturedList.SetItemText(nCount, 3, pack->getDestinationAddr());//�ڶ���
	capturedList.SetItemText(nCount, 4, pack->getPacketLen());//������

}

void CMyWireSharkDlg::OnLbnSelchangeNetcardList()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int row = netCardList.GetCurSel();
	if(row >= 0)
	{
		CString text;
		text = cap.getDevDetailInfo(row + 1);
		devInfo.SetWindowTextW(text);
	}
}


void CMyWireSharkDlg::OnLbnDblclkNetcardList()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int row = netCardList.GetCurSel();
	if(row >= 0)
	{
		cap.runDev(row + 1);
	}
}


void CMyWireSharkDlg::OnNMClickListCapture(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	packageInfo.DeleteAllItems();
	int row = capturedList.GetSelectionMark();
	if(row >= 0)
	{
		Package::PROTOCOL pro = cap.getPacketType(row + 1);
		switch (pro)
		{
		case Package::TCP:
			addTcpTree(row + 1);
			break;
		case Package::UDP:
			addUdpTree(row + 1);
			break;
		case Package::HTTP:
			addHttpTree(row + 1);
			break;
		case Package::ARP:
			addArpTree(row + 1);
			break;
		case Package::UNREALIZED:
			break;
		default:
			break;
		}
		
		
	}
	*pResult = 0;
}


void CMyWireSharkDlg::OnBnClickedBtStarcap()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(!cap.isRunning)
		return;
	if(cap.isThreadPause)
	{
		cap.thread->ResumeThread();
		cap.isThreadPause = false;
	}
}


void CMyWireSharkDlg::OnBnClickedBtStopcap()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(!cap.isRunning)
		return;
	if(!cap.isThreadPause)
	{
		cap.thread->SuspendThread();
		cap.isThreadPause = true;
	}
}


void CMyWireSharkDlg::OnBnClickedBtTotop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	capturedList.EnsureVisible(0, FALSE);
}


void CMyWireSharkDlg::OnBnClickedBtBottom()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int count = capturedList.GetItemCount() - 1;
	capturedList.EnsureVisible(count, FALSE);
}


void CMyWireSharkDlg::OnBnClickedBtFilter()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString tmp ;
	filterEdit.GetWindowTextW(tmp);
	USES_CONVERSION;
           //���ú�����T2A��W2A��֧��ATL��MFC�е��ַ�ת��
   char * pFileName = T2A(tmp); 

	if(!cap.setFetFilter(pFileName))
	{
		AfxMessageBox(L"�﷨���󣡣�");
	}
}
