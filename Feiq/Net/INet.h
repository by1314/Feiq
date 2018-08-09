
#ifndef INET_H
#define INET_H
#include <Winsock2.h>
#include <process.h>

#include "Packdef.h"


#pragma comment(lib,"Ws2_32.lib")
class INet
{
public:
	INet(){}
	virtual ~INet(){}
public:
	virtual bool InitNetWork() = 0;
	virtual void UnInitNetWork()  = 0;
	virtual bool SendData(long lSendIp,char* szbuf,int nLen) = 0;
public:
	static long GetValidIp()
	{
		char szHostName[50] = {0};
		hostent* paddrlist = NULL;
		in_addr addr;
		//1.获得主机名
		if(!gethostname(szHostName,sizeof(szHostName)))
		{
			//2.获得IPlist
			paddrlist = gethostbyname(szHostName);
			if (paddrlist->h_addr_list[0] != 0) {
                addr.s_addr = *(u_long *) paddrlist->h_addr_list[0];
                
            }

		}

		
		return addr.s_addr;
	}
};




#endif