
// FeiqDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Feiq.h"
#include "FeiqDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CFeiqDlg 对话框



CFeiqDlg::CFeiqDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFeiqDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFeiqDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_lstip);
}

BEGIN_MESSAGE_MAP(CFeiqDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(UM_ONLINEMSG,&CFeiqDlg::OnLineMsg)
	ON_MESSAGE(UM_OFFLINEMSG,&CFeiqDlg::OffLineMsg)
	ON_MESSAGE(UM_DataInfoMSG,&CFeiqDlg::DataInfoMsg)
	ON_MESSAGE(UM_DestroyMSG,&CFeiqDlg::DestroyMsg)
	ON_WM_DESTROY()
	ON_LBN_DBLCLK(IDC_LIST1, &CFeiqDlg::OnLbnDblclkList1)
END_MESSAGE_MAP()


// CFeiqDlg 消息处理程序

LRESULT CALLBACK KeyboardProc(
  _In_  int code,
  _In_  WPARAM wParam,
  _In_  LPARAM lParam
)
{
	if(VK_RETURN  == wParam)
	{
		return 1;
	}
	
	return CallNextHookEx(((CFeiqDlg*)theApp.m_pMainWnd)->m_hKeyBoardHook,code,wParam,lParam);
}

BOOL CFeiqDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	if(!theApp.GetUDPMediator()->Open())
	{
		return false;
	}

	m_hKeyBoardHook = SetWindowsHookEx(WH_KEYBOARD,&KeyboardProc,NULL,GetCurrentThreadId());
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}


LRESULT  CFeiqDlg::OnLineMsg(WPARAM w,LPARAM l)
{
	
	in_addr addr;
	addr.S_un.S_addr = l;
	char *szip =  inet_ntoa(addr);
	m_lstip.AddString(szip);
	return 0;
}

LRESULT  CFeiqDlg::OffLineMsg(WPARAM w,LPARAM l)
{   
	in_addr addr;
	addr.S_un.S_addr = l;
	char *szip =  inet_ntoa(addr);
	CString strip;
	for(int i = 0; i < m_lstip.GetCount();i++)
	{
		m_lstip.GetText(i,strip);
		if(szip == strip)
		{
			m_lstip.DeleteString(i);
			break;
		}
	}
	return 0;
}

LRESULT  CFeiqDlg::DataInfoMsg(WPARAM w,LPARAM l)
{
	//w -- 内容
	char *szbuf = (char*)w;
	//l --- ip
	in_addr addr;
	addr.S_un.S_addr = l;
	char *szip =  inet_ntoa(addr);
	CMyDlgChat *pDlg =  GetDlg(szip);
	CString strbuf = szip;
	pDlg->m_lstChat.AddString(strbuf +":"+szbuf);
	return 0;
}

LRESULT  CFeiqDlg::DestroyMsg(WPARAM w,LPARAM l)
{
	CMyDlgChat *pdlg = (CMyDlgChat*)w;
	auto ite = m_mapIpToDlg.begin();
	while(ite != m_mapIpToDlg.end())
	{
		if(ite->second == pdlg)
		{
			delete ite->second;
			ite->second = NULL;
			ite = m_mapIpToDlg.erase(ite);
			break;
		}
		ite++;
	}
	return 0;
}

void CFeiqDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CFeiqDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CFeiqDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CFeiqDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	if(m_hKeyBoardHook)
	{
		UnhookWindowsHookEx(m_hKeyBoardHook);
		m_hKeyBoardHook = NULL;
	}
	auto ite = m_mapIpToDlg.begin();
	while(ite != m_mapIpToDlg.end())
	{
		if(ite->second)
		{
			delete ite->second;
			ite->second = NULL;
		}
		ite++;
	}
	m_mapIpToDlg.clear();
}


void CFeiqDlg::OnLbnDblclkList1()
{
	// TODO: 在此添加控件通知处理程序代码
	//获得选中Ip
	CString strip;
	int nindex = m_lstip.GetCurSel();
	if(-1 == nindex) return;
	m_lstip.GetText(nindex,strip);
	//弹出窗口
	GetDlg(strip);
}

CMyDlgChat* CFeiqDlg::GetDlg(CString strip)
{
	CMyDlgChat* pdlg = m_mapIpToDlg[strip];
	if(!pdlg)
	{
	    pdlg = new CMyDlgChat;
		pdlg->Create(IDD_DIALOG1);
		
		pdlg->SetWindowText(strip);
		m_mapIpToDlg[strip] = pdlg;
	}
	pdlg->ShowWindow(SW_SHOWNORMAL);
	pdlg->SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
	return pdlg;
}