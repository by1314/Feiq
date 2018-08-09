

#ifndef UDPNET_H
#define UDPNET_H
#include "INet.h"
#include "IMediator.h"
class UDPNet:public INet
{
public:
	UDPNet(IMediator *pMediator);
	~UDPNet();
public:
	virtual bool InitNetWork();
	virtual void UnInitNetWork();
	virtual bool SendData(long lSendIp,char* szbuf,int nLen);
public:
	static  unsigned _stdcall ThreadProc( void * );
	bool  SelectSocket(SOCKET sock);
private:
	SOCKET m_sock;
	HANDLE m_hThread;
	bool   m_bFlagQuit;
	IMediator *m_pMediator;
};




#endif