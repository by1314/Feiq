#include "stdafx.h"
#include "UDPMediator.h"
#include "Feiq.h"
UDPMediator::UDPMediator()
{
	m_pUDPNet = new UDPNet(this);
}

UDPMediator::~UDPMediator()
{
	if(m_pUDPNet)
	{
		delete m_pUDPNet;
		m_pUDPNet = NULL;
	}
}

static const ProtocolMap g_ProtocolMapEntries[] = 
{
	{_DEF_PROTOCOL_ONLINE_RQ,&UDPMediator::OnlineRq},
	{_DEF_PROTOCOL_ONLINE_RS,&UDPMediator::OnlineRs},
	{_DEF_PROTOCOL_OFFLINE_RQ,&UDPMediator::OfflineRq},
	{_DEF_PROTOCOL_DATAINFO_RQ,&UDPMediator::DataInfoRq},
	{0,0}
};



bool UDPMediator::Open() 
{
	if(!m_pUDPNet->InitNetWork())
	{
		return false;
	}
	STRU_ONOFFLINE so;
	so.m_ntype = _DEF_PROTOCOL_ONLINE_RQ;
	if(!m_pUDPNet->SendData(INADDR_BROADCAST,(char*)&so,sizeof(so)))
	{
		return false;
	}
	return true;
}

void UDPMediator::Close()
{
		//发送下线消息
	STRU_ONOFFLINE so;
	so.m_ntype = _DEF_PROTOCOL_OFFLINE_RQ;
	m_pUDPNet->SendData(INADDR_BROADCAST,(char*)&so,sizeof(so));
	m_pUDPNet->UnInitNetWork();
}

bool UDPMediator::SendData(long lSendIp,char* szbuf,int nLen)
{
	if(!m_pUDPNet->SendData(lSendIp,szbuf,nLen))
		return false;

	return true;
}

bool UDPMediator::OnlineRq(long lRecvIp,char *szbuf)
{
		//1.将对方加入好友列表
	::PostMessage(theApp.m_pMainWnd->m_hWnd,UM_ONLINEMSG,0,lRecvIp);
		//判断是不是自己
	if(INet::GetValidIp() == lRecvIp)
	{
			return false;
	}
	//2.给对方回复
	STRU_ONOFFLINE so;
	so.m_ntype = _DEF_PROTOCOL_ONLINE_RS;
	m_pUDPNet->SendData(lRecvIp,(char*)&so,sizeof(so));
	return true;
}

bool UDPMediator::OnlineRs(long lRecvIp,char *szbuf)
{
	//1.将对方加入好友列表
	::PostMessage(theApp.m_pMainWnd->m_hWnd,UM_ONLINEMSG,0,lRecvIp);
	return true;
}

bool UDPMediator::OfflineRq(long lRecvIp,char *szbuf)
{
	if(INet::GetValidIp() == lRecvIp)
	{
		return false;
	}
    ::PostMessage(theApp.m_pMainWnd->m_hWnd,UM_OFFLINEMSG,0,lRecvIp);
	return true;
}

bool UDPMediator::DataInfoRq(long lRecvIp,char *szbuf)
{
	STRU_DATAINFO *psd = (STRU_DATAINFO *)szbuf;
	 //psd->m_szbuf
	SendMessage(theApp.m_pMainWnd->m_hWnd,UM_DataInfoMSG,(WPARAM)psd->m_szbuf,lRecvIp);
	return true;
}

bool UDPMediator::DealData(long lRecvIp,char *szbuf)
{
	//处理数据
	//分析数据包
	PackType *ptype = (PackType *)szbuf;
	//遍历协议映射表
	int i = 0;
	while(1)
	{
		if(g_ProtocolMapEntries[i].m_ntype == 0 &&g_ProtocolMapEntries[i].m_pfun == 0 )
			break;
		if(g_ProtocolMapEntries[i].m_ntype == *ptype)
		{
			(this->*g_ProtocolMapEntries[i].m_pfun)(lRecvIp,szbuf);
			break;
		}

		i++;

	}
	return true;
}