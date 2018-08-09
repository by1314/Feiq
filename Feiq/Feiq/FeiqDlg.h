
// FeiqDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "MyDlgChat.h"
#include <map>
using namespace std;
// CFeiqDlg �Ի���
class CFeiqDlg : public CDialogEx
{
// ����
public:
	CFeiqDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_FEIQ_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
