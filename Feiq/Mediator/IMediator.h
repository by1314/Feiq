
#ifndef IMEDIATOR_H
#define IMEDIATOR_H
#include "Packdef.h"
class IMediator
{
public:
	IMediator(){}
	virtual ~IMediator(){}
public:
	virtual bool Open() = 0;
	virtual void Close() = 0;
	virtual bool SendData(long lSendIp,char* szbuf,int nLen) = 0;
	virtual bool DealData(long lRecvIp,char *szbuf) = 0;
	
};


#endif