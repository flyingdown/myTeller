#include "common.h"

int main ()
{
	int sockfd = 0, iRet = 0, sockClient = 0;
	char caBuf[BUFFER_SIZE];
	char caRspMsg[BUFFER_SIZE];
	RSPSERVERCONFIG rspServerConfig;
	struct timeval timeout;
	int iRspLen = 0;
	
	memset ( caBuf, 0x00, sizeof caBuf );
		
	iRspLen = readRspServerConfig ( &rspServerConfig );
	if ( iRspLen < 0 )
	{
		writeLog ( ERR_LOG, "readRspServerConfig error", 0, 0 );
		return -1;
	}
	
	sockfd = initSocket ();
	if ( sockfd < 0 )
	{
		writeLog ( ERR_LOG, "initSocket error", 0, 0 );
		return -1;
	}
	
	writeLog ( INFO_LOG, "initSocket...", 0, 0 );
	printf ( "initSocket...\n" );
	
	iRet = sockListen ( sockfd );
	if ( sockfd < 0 )
	{
		writeLog ( ERR_LOG, "sockListen error", 0, 0 );
		return -1;
	}
	
	writeLog ( INFO_LOG, "listen...", 0, 0 );
	printf ( "listen...\n" );
	
	while ( 1 )
	{
		sockClient = sockAccept ( sockfd );
		if ( sockClient < 0 )
		{
			writeLog ( ERR_LOG, "sockAccept error", 0, 0 );
			return -1;
		}
		
		writeLog ( INFO_LOG, "accept...", 0, 0 );
		printf ( "accept...\n" );
		
		/* ���ý��ճ�ʱʱ�� */
		timeout.tv_sec = rspServerConfig.timeout;
		timeout.tv_usec = 0;
		iRet = setsockopt ( sockClient, SOL_SOCKET, SO_RCVTIMEO, ( char * ) &timeout, sizeof( timeout ) );
		if ( iRet < 0 )
		{
			writeLog ( ERR_LOG, "setsockoptʧ��", 0, 0 );
			return -1;
		}
		
		while ( 1 )
		{
			iRet = sockRecv ( sockClient, caBuf, sizeof caBuf );
			if ( iRet < 0 )
			{
				writeLog ( ERR_LOG, "sockRecv error", 0, 0 );
				break;
			}else if ( ! iRet )
			{
				printf ( "�Է��ر�����\n" );
				break;
			}
			
			printf ( "���������յ����ģ�\n" );
			printLog ( caBuf, iRet );
			
			sprintf ( caRspMsg, "%08d", iRspLen + 68 );
			memcpy ( caRspMsg + 8, caBuf + 8, 68 );
			memcpy ( caRspMsg + 8 + 68, rspServerConfig.caMsg, iRspLen );
			
			iRet = sockSend ( sockClient, caRspMsg, iRspLen + 8 + 68 );
			if ( iRet < 0 )
			{
				writeLog ( ERR_LOG, "sockSend error", 0, 0 );
				break;
			}
			
			printf ( "�������ӻ�Ӧ���ģ�\n" );
			printLog ( caRspMsg, iRspLen + 8 + 68 );
		}
	
		close ( sockClient );
		
	}
	
	close ( sockfd );
	
	return 0;
	
}