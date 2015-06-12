
// MyWireSharkDlg.h : ͷ�ļ�
//������
//by ���Ľܣ�2014��12��
#pragma once

#include <stdio.h>
#include "afxwin.h"
#include "Capture.h"
#include "afxcmn.h"


// CMyWireSharkDlg �Ի���
class CMyWireSharkDlg : public CDialogEx
{
// ����
public:
	CMyWireSharkDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MYWIRESHARK_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

private:
	void initNetCartList();
	void initCaptureList();
	void addPackageToList(Package *pack);
	Capture cap;

	void addTcpTree(int num);
	void addUdpTree(int num);
	void addHttpTree(int num);
	void addArpTree(int num);
	void addFrameAndEther(int num);
	void addIpv4Tree(int num);
// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListBox netCardList;
	CListCtrl capturedList;
	CStatic devInfo;
	CTreeCtrl packageInfo;
	afx_msg void OnLbnSelchangeNetcardList();
	afx_msg void OnLbnDblclkNetcardList();
	afx_msg void OnNMClickListCapture(NMHDR *pNMHDR, LRESULT *pResult);
	
	afx_msg void OnBnClickedBtStarcap();
	afx_msg void OnBnClickedBtStopcap();
	afx_msg void OnBnClickedBtTotop();
	afx_msg void OnBnClickedBtBottom();
	CEdit filterEdit;
	afx_msg void OnBnClickedBtFilter();
};
