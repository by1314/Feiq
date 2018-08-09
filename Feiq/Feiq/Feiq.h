
// Feiq.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "UDPMediator.h"

// CFeiqApp:
// �йش����ʵ�֣������ Feiq.cpp
//

class CFeiqApp : public CWinApp
{
public:
	CFeiqApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
private:
	IMediator *m_pUDPMediator;
public:
	IMediator * GetUDPMediator()
	{
		return m_pUDPMediator;
	}
public:
	virtual int ExitInstance();
};

extern CFeiqApp theApp;