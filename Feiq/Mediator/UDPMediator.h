

#ifndef UDPMEDIATOR_H
#define UDPMEDIATOR_H

#include "IMediator.h"
#include "UDPNet.h"

class UDPMediator;
typedef bool (UDPMediator::*PFUN)(long ,char *);

struct ProtocolMap
{
	PackType m_ntype;
	PFUN     m_pfun;
};

class UDPMediator: public IMediator
{
public:
	UDPMediator();
	virtual ~UDPMediator();
public:
	virtual bool Open() ;
	virtual void Close();
	virtual bool SendData(long lSendIp,char* szbuf,int nLen);
	virtual bool DealData(long lRecvIp,char *szbuf);
public:
	bool OnlineRq(long lRecvIp,char *szbuf);
	bool OnlineRs(long lRecvIp,char *szbuf);
	bool OfflineRq(long lRecvIp,char *szbuf);
	bool DataInfoRq(long lRecvIp,char *szbuf);
private:
	INet *m_pUDPNet;
};


#endif