
// FeiqDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "MyDlgChat.h"
#include <map>
using namespace std;
// CFeiqDlg 对话框
class CFeiqDlg : public CDialogEx
{
// 构造
public:
	CFeiqDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_FEIQ_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg void OnLbnDblclkList1();
	DECLARE_MESSAGE_MAP()
public:
	LRESULT  OnLineMsg(WPARAM w,LPARAM l);
	LRESULT  OffLineMsg(WPARAM w,LPARAM l);
	LRESULT  DataInfoMsg(WPARAM w,LPARAM l);
	LRESULT  DestroyMsg(WPARAM w,LPARAM l);
public:
	CMyDlgChat* GetDlg(CString strip);
public:

	CListBox m_lstip;
	HHOOK    m_hKeyBoardHook;
 
	
	 map<CString,CMyDlgChat*> m_mapIpToDlg;
};
