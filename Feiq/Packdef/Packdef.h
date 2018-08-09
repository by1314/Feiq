
#ifndef PACKDEF_H
#define PACKDEF_H


#define _DEF_PORT  1234

#define _DEF_PACKDEF   4096
#define _DEF_SIZE       1024
//��Ϣ
#define UM_ONLINEMSG     WM_USER+1
#define UM_OFFLINEMSG    WM_USER+2
#define UM_DataInfoMSG   WM_USER+3
#define UM_DestroyMSG    WM_USER+4
//Э��
#define _DEF_PROTOCOL_BASE  2000
#define _DEF_PROTOCOL_ONLINE_RQ  _DEF_PROTOCOL_BASE +1
#define _DEF_PROTOCOL_ONLINE_RS  _DEF_PROTOCOL_BASE +2

#define _DEF_PROTOCOL_OFFLINE_RQ _DEF_PROTOCOL_BASE +3

#define _DEF_PROTOCOL_DATAINFO_RQ _DEF_PROTOCOL_BASE +4


typedef short PackType;

//�Զ���Э���
//������������ظ���//���������
struct STRU_ONOFFLINE
{
	PackType m_ntype;
	
   //������Ϣ  
};
//������Ϣ
struct STRU_DATAINFO
{
	PackType m_ntype;
	char     m_szbuf[_DEF_SIZE];
   //������Ϣ  
};





#endif