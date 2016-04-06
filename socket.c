#include "common.h"

int initSocket ()
{
	return socket ( AF_INET, SOCK_STREAM, 0 );
}

int sockConnect ( int sockFd, const char * ip, int iPort )
{
	int iRet = 0;
	struct sockaddr_in saddr_in;
	
	saddr_in.sin_family = AF_INET;
	saddr_in.sin_port = htons ( iPort ); 
	/* writeLog ( INFO_LOG, ip, 0, 0 ); */
	inet_pton ( AF_INET, ip, &saddr_in.sin_addr );
	
	return connect ( sockFd, ( struct sockaddr * )&saddr_in, 
	sizeof ( struct sockaddr_in ) );
}

int sockSend ( int sockFd, const void * caBuf, size_t sLen )
{
	int iRet = 0, iLeft = 0;
	
	while ( iLeft < sLen )
	{
		iRet = send ( sockFd, caBuf + iLeft, sLen - iLeft, 0 );
		if ( iRet < 0 )
		{
			if ( errno == EINTR )
				continue;
			else
			{
				memset ( g_caMsg, 0x00, sizeof g_caMsg );
				sprintf ( g_caMsg, "套接字发送失败, [%s]", strerror ( errno ) );
				writeLog ( ERR_LOG, g_caMsg, 0, 0 );
				return -1;
			}
		}
		
		iLeft += iRet;
	}
	
	return 0;
}

int sockRecv ( int sockFd, void * caBuf, size_t iLen )
{
	int iRet = 0, iReadLen = 0, iHeadLen = 8, iDataLen = 0;
	char caHeadLen[iHeadLen+1];
	
	while ( iReadLen < iLen )
	{
		iRet = recv ( sockFd, caBuf+iReadLen, iLen-iReadLen, 0 );
		
		/* memset ( g_caMsg, 0x00, sizeof g_caMsg );
		sprintf ( g_caMsg, "iRet[%d]", iRet );
		writeLog ( INFO_LOG, g_caMsg, 0, 0 ); */
		
		if ( iRet < 0 )
		{
			writeLog ( ERR_LOG, "套接字接收失败", 0, 0 );
			close ( sockFd );
			return -1;
		}else if ( iRet == 0 )
		{
			writeLog ( ERR_LOG, "对方关闭链接", 0, 0 );
			close ( sockFd );
			return iRet;
		}else
		{
			memset ( caHeadLen, 0x00, sizeof caHeadLen );
			memcpy ( caHeadLen, caBuf, iHeadLen );
			iDataLen = atoi ( caHeadLen );
			iReadLen += iRet;
			
			memset ( g_caMsg, 0x00, sizeof g_caMsg );
			sprintf ( g_caMsg, "iDataLen[%d], iHeadLen[%d], iReadLen[%d]", 
			iDataLen, iHeadLen, iReadLen );
			writeLog ( INFO_LOG, g_caMsg, 0, 0 );
			
			if ( iDataLen + iHeadLen <= iReadLen )
			{
				writeLog ( INFO_LOG, "接收到的报文:", caBuf, iReadLen );
				/* close ( sockFd ); */
				break;
			}
		}
		
	}
	
	return iReadLen;
}

int sockClose ( int sockFd )
{
	return close ( sockFd );
}


int getSockInfo ( int sockFd )
{
	struct sockaddr_in saddr_in;
	int iRet = 0;
	char ip[INET_ADDRSTRLEN+1];
	short port;
	socklen_t sLen = sizeof saddr_in;
	
	bzero ( &saddr_in, sizeof saddr_in );
	iRet = getsockname ( sockFd, ( struct sockaddr * ) &saddr_in, &sLen );
	if ( iRet < 0 )
	{
		writeLog ( ERR_LOG, "getsockname出错", 0, 0 );
		return -1;
	}
	
	/* memset ( ip, 0x00, sizeof ip );
	inet_ntop ( AF_INET, & saddr_in.sin_addr, ip, INET_ADDRSTRLEN ); */
	
	port = ntohs ( saddr_in.sin_port );
	
	memset ( g_caMsg, 0x00, sizeof g_caMsg );
	sprintf ( g_caMsg, "本地地址端口:ip=[%s], port=[%hu]", inet_ntoa ( saddr_in.sin_addr ), port );
	writeLog ( INFO_LOG, g_caMsg, 0, 0 );

	return 0;
}

int sockListen ( int sockFd )
{
	int iRet = 0;
	struct sockaddr_in addr_in;
	
	addr_in.sin_family = AF_INET;
	addr_in.sin_port = htons ( 8989 ); 
	addr_in.sin_addr.s_addr = INADDR_ANY;
	
	iRet = bind ( sockFd, ( struct sockaddr * ) &addr_in, sizeof ( addr_in ) );
	if ( iRet < 0 )
	{
		writeLog ( ERR_LOG, "sock bind error", 0, 0 );
		return -1;
	}
	
	iRet = listen ( sockFd, 5 );
	if ( iRet < 0 )
	{
		writeLog ( ERR_LOG, "sock listen error", 0, 0 );
		return -1;
	}
	
}

int sockAccept ( int sockFd )
{
	int iClientfd = 0, iLen = 0;
	
	struct sockaddr_in addr_in;
	iLen = sizeof addr_in;
	
	iClientfd = accept ( sockFd, ( struct sockaddr * ) &addr_in, &iLen );
	if ( iClientfd < 0 )
	{
		writeLog ( ERR_LOG, "sock accept error", 0, 0 );
		return -1;
	}
	
	memset ( g_caMsg, 0x00, sizeof g_caMsg );
	sprintf ( g_caMsg, "client addr = [%s], port = [%d]", 
	inet_ntoa ( addr_in.sin_addr ), ntohs ( addr_in.sin_port ) );
	writeLog ( ERR_LOG, g_caMsg, 0, 0 );
	
	return iClientfd;
	
}

