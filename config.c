#include "common.h"

static char * g_ConfigFiles[CONFIGFILE__SIZE] = {
	"config/server.ini",
	"config/bitmap.ini",
	"config/traninfo.ini"
};

/* 读服务配置 */
int readServerConfig ( SERVERINFO * serverInfo )
{
	int iOk = 0;
	char caBuf[512];
	char caTemp[128];
	FILE * fp = NULL;
	
	memset ( caBuf, 0x00, sizeof caBuf );
	
	fp = fopen ( g_ConfigFiles[CONFIGFILE__SERVER], "r" );
	if ( ! fp )
	{
		memset ( g_caMsg, 0x00, sizeof g_caMsg );
		sprintf ( g_caMsg, "打开服务端配置失败, [%s]", strerror ( errno ) );
		writeLog ( ERR_LOG, g_caMsg, 0, 0 );
		return -1;
	}
	
	while ( fgets ( caBuf, sizeof caBuf, fp ) )
	{
		delSpace ( caBuf );
		
		if ( caBuf[0] != '#' && caBuf[0] != '\0' )
		{
			if ( ! strncmp ( caBuf + 1, "CLIENT", 6 ) )
			{
				iOk = 1;
				while ( fgets ( caBuf, sizeof caBuf, fp ) )
				{
					delSpace ( caBuf );
					bzero ( caTemp, sizeof caTemp );
					if ( ! strncmp ( caBuf, "ADDR", 4 ) )
					{
						memcpy ( serverInfo->ipaddr, caBuf + 4 + 1, strlen ( caBuf ) - 4 - 2 );
						serverInfo->ipaddr[strlen ( caBuf ) - 4 - 2 ] = '\0';
					}else if ( ! strncmp ( caBuf, "PORT", 4 ) )
					{
						memcpy ( caTemp, caBuf + 4 + 1, strlen ( caBuf ) - 4 - 2 );
						serverInfo->port = atoi ( caTemp );
					}else if ( ! strncmp ( caBuf, "TIME", 4 ) )
					{
						memcpy ( caTemp, caBuf + 4 + 1, strlen ( caBuf ) - 4 - 2 );
						serverInfo->timeout = atoi ( caTemp );
					}else if ( ! strncmp ( caBuf + 1, "CLIENT_END", 10 ) )
						break;
				}
			}else
				continue;
				
			if ( iOk )
				break;
			
		}
		
		memset ( caBuf, 0x00, sizeof caBuf );
	}
	
	fclose ( fp );
	
	return 0;
	
}

/* 读位图配置 */
int readBitMapConfig ( char * tranCode, char * bitMap )
{
	int iOk = 0;
	char caBuf[512];
	FILE * fp = NULL;
	
	memset ( caBuf, 0x00, sizeof caBuf );
	
	fp = fopen ( g_ConfigFiles[CONFIGFILE__BITMAP], "r" );
	if ( ! fp )
	{
		memset ( g_caMsg, 0x00, sizeof g_caMsg );
		sprintf ( g_caMsg, "打开位图配置文件失败, [%s]", strerror ( errno ) );
		writeLog ( ERR_LOG, g_caMsg, 0, 0 );
		return -1;
	}
	
	while ( fgets ( caBuf, sizeof caBuf, fp ) )
	{
		delSpace ( caBuf );
		
		if ( caBuf[0] != '#' && caBuf[0] != '\0' )
		{				
			if ( ! strncmp ( caBuf + 1, tranCode, strlen ( tranCode ) ) )
			{
				while ( fgets ( caBuf, sizeof caBuf, fp ) )
				{
					delSpace ( caBuf );
					
					if ( caBuf[0] != '#' && caBuf[0] != '\0' 
						&& ! strncmp ( caBuf, "bitmap", 6 ) )
					{
						pcakBit ( caBuf + 7, bitMap );
						iOk = 1;
						break;
					}else if ( ! strncmp ( caBuf + 1, tranCode, strlen ( tranCode ) ) )
						break;
					
				}
			}else
				continue;
				
			if ( iOk )
				break;
		}
		
		memset ( caBuf, 0x00, sizeof caBuf );
	}
	
	fclose ( fp );
	
	if ( ! iOk )
	{
		memset ( g_caMsg, 0x00, sizeof g_caMsg );
		sprintf ( g_caMsg, "未找到交易[%s]的配置", tranCode );
		writeLog ( ERR_LOG, g_caMsg, 0, 0 );
	}
	
	return iOk ? 0 : -1;
}

/* 组位图 */
int pcakBit ( char * bitMap, char * outBit )
{
	/*
   * b[0]  { 7-0     0, 1, 2, 3, 4, 5, 6, 7 }
   * b[1]  { 15-8    8, 9, 10, 11, 12, 13, 14, 15 }
   * b[2]  { 23-16   16, 17, 18, 19, 20, 21, 22, 23 }
   * b[3]  { 31-24   24, 25, 26, 27, 28, 29, 30 ,31 }
   * b[4]  { 39-32   32, 33, 34, 35, 36, 37, 38, 39 }
   * b[5]  { 47-40   40, 41, 42, 43, 44, 45, 46, 47 }
   * b[6]  { 55-48   48, 49, 50, 51, 52, 53, 54, 55 }
   * b[7]  { 63-56   56, 57, 58, 59, 60, 61, 62, 63 }
   * b[8]  { 71-64   64, 65, 66, 67, 68, 69, 70, 71 }
   * b[9]  { 79-72   72, 73, 74, 75, 76, 77, 78, 79 }
   * b[10] { 87-80   80, 81, 82, 83, 84, 85, 86, 87 }
   * b[11] { 95-88   88, 89, 90, 91, 92, 93, 94, 95 }
   * b[12] { 103-96  96, 97, 98, 99, 100, 101, 102, 103 }
   * b[13] { 111-104 104, 105, 106, 107, 108, 109, 110, 111 }
   * b[14] { 119-112 112, 113, 114, 115, 116, 117, 118, 119 }
   * b[15] { 127-120 120, 121, 122, 123, 124, 125, 126, 127 }
   */
  
  int iPos = 0, i = 0, j = 0;
	char * p = NULL, * q = NULL;
	char caBit[3+1];
	
	q = bitMap;
	while ( p = strchr ( q, ',' ) ) 
	{
		memset ( caBit, 0x00, sizeof caBit );
		memcpy ( caBit, q, p - q );
		
		iPos = atoi ( caBit );
		if ( ! iPos )
			continue;
			
		iPos--;
		
		i = iPos / 8;
		j = iPos % 8;
		
		outBit[i] = outBit[i] | ( 0x01 << ( 7 - j ) );
		
		q = p + 1;
	} 
	
	return 0;
}

/* 读交易配置 */
int readTranInfoConfig ( char * tranCode, TRANDESC * tranDesc )
{
	int iOk = 0;
	char caBuf[512];
	FILE * fp = NULL;
	
	memset ( caBuf, 0x00, sizeof caBuf );
	
	fp = fopen ( g_ConfigFiles[CONFIGFILE__TRANINFO], "r" );
	if ( ! fp )
	{
		memset ( g_caMsg, 0x00, sizeof g_caMsg );
		sprintf ( g_caMsg, "打开交易配置文件失败,[%s]", strerror ( errno ) );
		writeLog ( ERR_LOG, g_caMsg, 0, 0 );
		return -1;
	}
	
	while ( fgets ( caBuf, sizeof caBuf, fp ) )
	{
		delSpace ( caBuf );
		
		if ( caBuf[0] != '#' && caBuf[0] != '\0' && caBuf[0] != '\n' )
		{
			/* memset ( g_caMsg, 0x00, sizeof g_caMsg );
			sprintf ( g_caMsg, "[%s][%d]", caBuf+1, strlen ( tranCode ) );
			writeLog ( INFO_LOG, g_caMsg, caBuf, strlen ( caBuf ) ); */ 
			if ( caBuf[0] == '<' && ! strncmp ( caBuf+1, tranCode, strlen ( tranCode ) ) )
			{
				iOk = 1;
				while ( fgets ( caBuf, sizeof caBuf, fp ) )
				{
					delSpace ( caBuf );
					if ( caBuf[0] != '#' && caBuf[0] != '\0' && caBuf[0] != '\n' )
						continue;
					if ( caBuf[0] == '<' && ! strncmp ( caBuf+1, tranCode, strlen ( tranCode ) ) )
						break;
						
					fillTranInfo ( tranDesc, caBuf );
					
				}
			}else 
				continue;
			
			if ( ! iOk )
				break;
		}
		
		
		memset ( caBuf, 0x00, sizeof caBuf );
	}
	
	fclose ( fp );
	
	return iOk ? 0 : -1;
}

/* 填充单个交易域配置 */
int fillTranInfo ( TRANDESC * tranDesc, char * caDataBuf )
{
	int bitno = 0;
	char * p = NULL, * q = NULL;
	char caData[1024];
	
	q = caDataBuf;
	p = strchr ( q, ']' );
	memset ( caData, 0x00, sizeof caData );
	memcpy ( caData, q + 1, p - q - 1 );
	/* 获取域号 */
	bitno = atoi ( caData );
	/* memset ( g_caMsg, 0x00, sizeof g_caMsg );
	sprintf ( g_caMsg, "域号[%d]", bitno );
	writeLog ( INFO_LOG, g_caMsg, 0, 0 ); */
	
	/* 获取数据 */
	q = p + 1;
	p = strchr ( q, ']' );
	memset ( caData, 0x00, sizeof caData );
	memcpy ( caData, q + 1, p - q - 1 );
	/* memset ( g_caMsg, 0x00, sizeof g_caMsg );
	sprintf ( g_caMsg, "数据[%s]", caData );
	writeLog ( INFO_LOG, g_caMsg, 0, 0 ); */
	
	return tranPutField ( bitno, caData, strlen ( caData ), tranDesc );
}

/* 为挡板服务读配置 */
int readRspServerConfig ( RSPSERVERCONFIG * rspServerConfig )
{
	int iOk = 0, iRet = 0;
	char caBuf[512];
	char caTemp[128];
	FILE * fp = NULL;
	struct stat stFile;
	
	memset ( caBuf, 0x00, sizeof caBuf );
	
	fp = fopen ( g_ConfigFiles[CONFIGFILE__SERVER], "r" );
	if ( ! fp )
	{
		memset ( g_caMsg, 0x00, sizeof g_caMsg );
		sprintf ( g_caMsg, "打开挡板服务端配置失败, [%s]", strerror ( errno ) );
		writeLog ( ERR_LOG, g_caMsg, 0, 0 );
		return -1;
	}
	
	while ( fgets ( caBuf, sizeof caBuf, fp ) )
	{
		delSpace ( caBuf );
		
		if ( caBuf[0] != '#' && caBuf[0] != '\0' )
		{
			if ( ! strncmp ( caBuf + 1, "SERVER", 6 ) )
			{
				iOk = 1;
				while ( fgets ( caBuf, sizeof caBuf, fp ) )
				{
					delSpace ( caBuf );
					bzero ( caTemp, sizeof caTemp );
					if ( ! strncmp ( caBuf, "FILE", 4 ) )
					{
						memcpy ( rspServerConfig->filename, caBuf + 4 + 1, strlen ( caBuf ) - 4 - 2 );
						rspServerConfig->filename[strlen ( caBuf ) - 4 - 2 ] = '\0';
					}else if ( ! strncmp ( caBuf, "PORT", 4 ) )
					{
						memcpy ( caTemp, caBuf + 4 + 1, strlen ( caBuf ) - 4 - 2 );
						rspServerConfig->port = atoi ( caTemp );
					}else if ( ! strncmp ( caBuf, "TIME", 4 ) )
					{
						memcpy ( caTemp, caBuf + 4 + 1, strlen ( caBuf ) - 4 - 2 );
						rspServerConfig->timeout = atoi ( caTemp );
					}else if ( ! strncmp ( caBuf + 1, "SERVER_END", 10 ) )
						break;
				}
			}else
				continue;
				
			if ( iOk )
				break;
			
		}
		
		memset ( caBuf, 0x00, sizeof caBuf );
	}
	
	fclose ( fp );
	
	stat ( rspServerConfig->filename, &stFile );
	
	memset ( g_caMsg, 0x00, sizeof g_caMsg );
	sprintf ( g_caMsg, "报文配置文件大小[%d]", stFile.st_size );
	writeLog ( ERR_LOG, g_caMsg, 0, 0 );
	
	fp = fopen ( rspServerConfig->filename, "r" );
	if ( ! fp )
	{
		memset ( g_caMsg, 0x00, sizeof g_caMsg );
		sprintf ( g_caMsg, "打开挡板报文配置失败, [%s]", strerror ( errno ) );
		writeLog ( ERR_LOG, g_caMsg, 0, 0 );
		return -1;
	}
	
	if ( ( iRet = fread ( rspServerConfig->caMsg, stFile.st_size, 1, fp ) ) < 0 )
	{
		writeLog ( ERR_LOG, "读挡板报文配置失败", 0, 0 );
		return -1;
	}else
	{
		memset ( g_caMsg, 0x00, sizeof g_caMsg );
		sprintf ( g_caMsg, "读入字节大小[%d]", iRet * stFile.st_size );
		writeLog ( ERR_LOG, g_caMsg, 0, 0 );
	}
	
	fclose ( fp );
	
	
	return iRet * stFile.st_size;
	
}
