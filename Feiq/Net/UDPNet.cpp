#include "stdafx.h"
#include "UDPNet.h"

UDPNet::UDPNet(IMediator *pMediator)
{
	m_sock = NULL;
	m_hThread = NULL;
	m_bFlagQuit = true;
	m_pMediator = pMediator;
}

UDPNet::~UDPNet()
{}

bool UDPNet::InitNetWork()
{
	WORD wVersionRequested;
    WSADATA wsaData;
    int err;

/* Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h */
    wVersionRequested = MAKEWORD(2, 2);

    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
      
       
        return false;
    }


    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
       
        
        UnInitNetWork();
        return false;
    }
   
        

	//2.创建摊位 -- socket 套接字
	m_sock =  socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	if(INVALID_SOCKET  == m_sock)
	{
		UnInitNetWork();
        return false;
	}
	//改变socket 属性非阻塞
	u_long argp = 1;
	ioctlsocket(m_sock,FIONBIO ,&argp);
	//设置广播属性
	BOOL bflag = TRUE;
	setsockopt(m_sock,SOL_SOCKET,SO_BROADCAST,(char*)&bflag,sizeof(BOOL));
	//3.找个地 ---bind
	sockaddr_in  addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr =INADDR_ANY;
	addr.sin_port = htons(_DEF_PORT);
	if( SOCKET_ERROR == bind(m_sock,(const sockaddr*)&addr,sizeof(addr)))
	{
		UnInitNetWork();
        return false;
	}

	//recv

	m_hThread = (HANDLE)_beginthreadex(NULL,0,&ThreadProc,this,0,0);
	return true;
}



unsigned _stdcall UDPNet::ThreadProc( void * lpvoid)
{
	UDPNet *pthis = (UDPNet*)lpvoid;
	char szbuf[_DEF_PACKDEF] = {0};
	sockaddr_in addrClient;
	int nSize = sizeof(addrClient);
	int nRes;
	while(pthis->m_bFlagQuit)
	{
		if(!pthis->SelectSocket(pthis->m_sock))
			continue;

		nRes = recvfrom(pthis->m_sock,szbuf,_DEF_PACKDEF,0,(sockaddr*)&addrClient,&nSize);
		//nRes = recvfrom(pthis->,szbuf,_DEF_PACKDEF,0,(sockaddr*)&addrClient,&nSize);
		if(nRes >0)
		{
			//交给中介者处理
			pthis->m_pMediator->DealData(addrClient.sin_addr.S_un.S_addr,szbuf);
		}
	}
	return 0;
}

void UDPNet::UnInitNetWork()
{
	m_bFlagQuit = false;
	if(m_hThread)
	 {
		 if(WAIT_TIMEOUT == WaitForSingleObject(m_hThread,100))
			 TerminateThread(m_hThread,-1);

		 CloseHandle(m_hThread);
		 m_hThread = NULL;
	 }
	 WSACleanup();
	 if(m_sock)
	 {
		 closesocket(m_sock);
		 m_sock = NULL;
	 }
	
}


bool UDPNet::SendData(long lSendIp,char* szbuf,int nLen)
{
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = lSendIp;
	addr.sin_port = htons(_DEF_PORT);
	if(sendto(m_sock,szbuf,nLen,0,(const sockaddr*)&addr,sizeof(addr))<=0)
	{
		return false;
	}
	return true;
}

bool  UDPNet::SelectSocket(SOCKET sock)
{
	timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 100;

	fd_set fdsets;
	FD_ZERO(&fdsets);
	FD_SET(sock,&fdsets);
	select(0,&fdsets,NULL,NULL,&tv);

	if(!FD_ISSET(sock,&fdsets))
	{
		return false;
	}
	return true;
}