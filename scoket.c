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
	inet_pton ( AF_INET, ip, &saddr_in.sin_addr );
	
	return connect ( sockFd, ( struct sockaddr * )&saddr_in, 
	sizeof ( struct sockaddr_in ) );
}

int sockSend ( int sockFd, const void * caBuf, size_t sLen )
{
	int iRet = 0, iLef = 0;
	
	while ( iLef < sLen )
	{
		iRet = send ( sockFd, caBuf + iLef, sLen - iLef, 0 );
		if ( iRet < 0 )
		{
			if ( errno == EINTR )
				continue;
		}else
		{
			writeLog ( ERR_LOG, "Ì×½Ó×Ö·¢ËÍÊ§°Ü", 0, 0 );
			return -1;
		}
	}
	
	return 0;
}

int sockRecv ( int sockFd, void * caBuf, size_t iLen )
{
	int iRet = 0; iReadLen = 0;
	
	iRet = recv ( sockFd, caBuf + iReadLen, iLen, 0 );
}

