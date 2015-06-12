
// MyWireSharkDlg.h : 头文件
//主界面
//by 梁文杰，2014年12月
#pragma once

#include <stdio.h>
#include "afxwin.h"
#include "Capture.h"
#include "afxcmn.h"


// CMyWireSharkDlg 对话框
class CMyWireSharkDlg : public CDialogEx
{
// 构造
public:
	CMyWireSharkDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MYWIRESHARK_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

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
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
