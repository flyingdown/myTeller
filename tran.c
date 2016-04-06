#include "common.h"

extern char g_LogName[];

int dealTran ( const char * tranCode, int tranNum )
{
	int i = 0, iRet = 0, iOk = 0;
	SERVERINFO serverInfo;
	
	for ( i = 0; i < tranNum; i++ )
	{
		iRet = _dealTran ( tranCode, &serverInfo );
		if ( iRet < 0 )
		{
			writeLog ( ERR_LOG, "#############交易失败############", 0, 0 );
			printf ( "×××第[%d]笔交易失败.进程ID[%d]×××\n", i, getpid () );
			iOk = -1;
		}else
			printf ( "√√√第[%d]笔交易成功.进程ID[%d]√√√\n", i, getpid () );
		
	}
	
	return iOk;
}

int _dealTran ( const char * tranCode, SERVERINFO * serverInfo )
{
	int i = 0, iRet = 0, iLen = 0, iMsgLen = 0;
	char caBitMap[BITMAP_LEN];
	TRANDESC * tranDesc = NULL;
	char caMsg[1024*5];
	char caStrMsg[1024*5];
	char caTemp[32];
	int sockfd = 0;
	struct timeval timeout;
	char * pValue = NULL;
	
	memset ( caBitMap, 0x00, sizeof caBitMap );
	
	/* 初始化日志名 
	memset ( g_LogName, 0x00, sizeof g_LogName );*/
	sprintf ( g_LogName, "./log/emulation%s_%08d.log", tranCode, getpid () );
	writeLog ( INFO_LOG, "*******************请求流程开始*******************", 0, 0 );
	
	/* 初始化交易描述 */
	iRet = initTranDesc ( & tranDesc );
	if ( iRet < 0 )
	{
		writeLog ( ERR_LOG, "initTranDesc fail", 0, 0 );
		return -1;
	}
	
	/* 读服务器配置 */
	readServerConfig ( serverInfo );
	if ( iRet < 0 )
	{
		writeLog ( ERR_LOG, "readServerConfig fail", 0, 0 );
		destoryTranDesc ( tranDesc );
		return -1;
	}
	
	/* 读交易配置 */
	iRet = readTranInfoConfig ( tranCode, tranDesc );
	if ( iRet < 0 )
	{
		writeLog ( ERR_LOG, "readTranInfoConfig fail", 0, 0 );
		destoryTranDesc ( tranDesc );
		return -1;
	}
	
	/* 读位图配置 */
	iRet = readBitMapConfig ( tranCode, caBitMap );
	if ( iRet < 0 )
	{
		writeLog ( ERR_LOG, "readBitMapConfig fail", 0, 0 );
		destoryTranDesc ( tranDesc );
		return -1;
	}
	
	/* 组包 */
	memset ( caMsg, 0x00, sizeof caMsg );
	iLen = doPackTran ( tranDesc, caBitMap, caMsg );
	if ( iLen < 0 )
	{
		writeLog ( ERR_LOG, "组包失败", 0, 0 );
		destoryTranDesc ( tranDesc );
		return -1;
	}
	writeLog ( INFO_LOG, "位图", caBitMap, 16 );
	writeLog ( INFO_LOG, "报文内容", caMsg, iLen );
	
	/* 初始化SOCKET */
	sockfd = initSocket ();
	if ( sockfd < 0 )
	{
		writeLog ( ERR_LOG, "initSocket失败", 0, 0 );
		destoryTranDesc ( tranDesc );
		return -1;
	}
	
	/* 设置接收超时时间 */
	timeout.tv_sec = serverInfo->timeout;
	timeout.tv_usec = 0;
	iRet = setsockopt ( sockfd, SOL_SOCKET, SO_RCVTIMEO, ( char * ) &timeout, sizeof( timeout ) );
	if ( iRet < 0 )
	{
		writeLog ( ERR_LOG, "setsockopt失败", 0, 0 );
		destoryTranDesc ( tranDesc );
		return -1;
	}
	
	/* 连接服务器 */
	iRet = sockConnect ( sockfd, serverInfo->ipaddr, serverInfo->port );
	if ( iRet < 0 )
	{
		writeLog ( ERR_LOG, "sockConnect失败", 0, 0 );
		destoryTranDesc ( tranDesc );
		return -1;
	}
	
	/* 获取连接信息 */
	getSockInfo ( sockfd );
	
	/* 发送消息 */
	iRet = sockSend ( sockfd, caMsg, iLen );
	if ( iRet < 0 )
	{
		writeLog ( ERR_LOG, "sockSend失败", 0, 0 );
		destoryTranDesc ( tranDesc );
		return -1;
	}
	writeLog ( INFO_LOG, "*******************请求流程结束*******************", 0, 0 );
	
	writeLog ( INFO_LOG, "*******************应答流程开始*******************", 0, 0 );
	/* 接收数据 */
	memset ( caMsg, 0x00, sizeof caMsg );
	iMsgLen = sockRecv ( sockfd, caMsg, sizeof caMsg );
	if ( iMsgLen <= 0 )
	{
		writeLog ( ERR_LOG, "sockRecv失败", 0, 0 );
		destoryTranDesc ( tranDesc );
		return -1;
	}
	
	/* 关闭链路 */
	iRet = sockClose ( sockfd );
	if ( iRet < 0 )
	{
		writeLog ( ERR_LOG, "sockClose失败", 0, 0 );
		destoryTranDesc ( tranDesc );
		return -1;
	}
	
	/* 清空请求结构体 */
	clearAllField ( tranDesc );
	
	/* 解包 */
	iRet = doUnPackTran ( tranDesc, caMsg, iMsgLen );
	if ( iRet < 0 )
	{
		writeLog ( ERR_LOG, "解包失败", 0, 0 );
		destoryTranDesc ( tranDesc );
		return -1;
	}
	
	/* 响应码 */
	iRet = tranGetField ( 12, &pValue, tranDesc );
	if ( iRet < 0 )
	{
		writeLog ( ERR_LOG, "未获取到响应码", 0, 0 );
		destoryTranDesc ( tranDesc );
		return -1;
	}else
	{
		/* memset ( g_caMsg, 0x00, sizeof g_caMsg );
		sprintf ( g_caMsg, "iRet[%d]", iRet );
		writeLog ( INFO_LOG, g_caMsg, 0, 0 ); */
		
		memset ( caTemp, 0x00, sizeof caTemp );
		memcpy ( caTemp, pValue, iRet );
		writeLog ( INFO_LOG, caTemp, 0, 0 );
		if ( strncmp ( caTemp, "0000", 4 ) )
		{
			writeLog ( ERR_LOG, "交易错误", 0, 0 );
			destoryTranDesc ( tranDesc );
			return -1;
		}
		
		addSuCount ();
	}
	
	printTranDesc ( tranDesc );
	
	destoryTranDesc ( tranDesc );
	writeLog ( INFO_LOG, "*******************应答流程结束*******************", 0, 0 );
	
	return 0;
}


int doPackTran ( TRANDESC * tranDesc, char * caBitMap, char * outMsg )
{
	int i = 0, j = 0, k = 0, iRet = 0, iCurLen = 0, iLen = 0;
	int iBitMapLen = 8;
	char caMsg[1024*4];
	char * p = caMsg;
	char * pValue = NULL;
	FIELDDESC fieldDesc;
	char caCharMap[CHARMAP_LEN];
	
	memset ( caCharMap, 0x00, sizeof caCharMap );
	memset ( caMsg, 0x00, sizeof caMsg );
	
	convertCharMap ( caBitMap, caCharMap );
	
	for ( i = 2; i < MAX_FIELD_STYLE_SIZE; i++ )
	{
		if ( caCharMap[i] == '1' )
		{
			/* memset ( g_caMsg, 0x00, sizeof g_caMsg );
			sprintf ( g_caMsg, "域[%d]处理...", i );
			writeLog ( INFO_LOG, g_caMsg, caBitMap, 16 ); */
		
			if ( i > 64 )
			{
				caBitMap[0] |= ( char ) 0x80;
				iBitMapLen = 16;
			}
			
			iRet  = tranGetField ( i, &pValue, tranDesc );
			if ( iRet <= 0 )
			{
				memset ( g_caMsg, 0x00, sizeof g_caMsg );
				sprintf ( g_caMsg, "未获取到域[%d]的值", i );
				writeLog ( ERR_LOG, g_caMsg, 0, 0 );
			}
			
			getFieldInfo ( &fieldDesc, i );
			if ( fieldDesc.fieldattr == 0 ) /* 定长 */
			{
				if ( iRet > 0 )
					memcpy ( p + iCurLen, pValue, iRet );
				else 
					memset ( p + iCurLen, 0x20, fieldDesc.fieldlen );
					
				iRet = fieldDesc.fieldlen;
			}else /* 变长 */
			{
				memset ( p + iCurLen, '0', fieldDesc.fieldattr );
				j = fieldDesc.fieldattr - 1;
				k = iRet;
				while ( 1 )
				{
					* ( p + iCurLen + j ) = ( char ) ( k % 10 + 0x30 );
					if ( k < 10 )
						break;
					k /= 10;
					j--;
				}
				iCurLen += fieldDesc.fieldattr;
				memcpy ( p + iCurLen, pValue, iRet );
			}
			
			iCurLen += iRet;
		}
	}	
	
	sprintf ( outMsg, "%08d", 68 + iCurLen + iBitMapLen );
	iLen += 8;
	
	memset ( outMsg + iLen, 0x20, 68 );
	memcpy ( outMsg + iLen, "00", 2 );
	iLen += 68;
	
	memcpy ( outMsg + iLen, caBitMap, iBitMapLen );
	iLen += iBitMapLen;
	
	memcpy ( outMsg + iLen, p, iCurLen );
	iLen += iCurLen;
	
	return iLen;
}

int convertCharMap ( char * caBitMap, char * caCharMap )
{
	int i = 0, j = 0, k = 1;
	
	for ( i = 0; i < BITMAP_LEN; i++ )
	{
		for ( j = 0; j < 8; j++, k++ )
		{
			if ( caBitMap[i] & ( 0x01 << ( 7 - j ) ) )
				caCharMap[k] = '1';
		}
	}
}


int doUnPackTran ( TRANDESC * tranDesc, char * pcaMsg, int iLen )
{
	int i = 0, iCurLen = 68 + 8;
	int iBitMapLen = 16, iCurBit = 0;
	char caBitMap[BITMAP_LEN];
	char caCharMap[CHARMAP_LEN];
	FIELDDESC fieldDesc;
	char * pValue = NULL;
	char caTemp[3+1];
	char caValue[1024];
	
	memset ( g_caMsg, 0x00, sizeof g_caMsg );
	sprintf ( g_caMsg, "8583报文传入长度[%d]", iLen - iCurLen );
	writeLog ( INFO_LOG, g_caMsg, 0, 0 );
	
	memset ( caBitMap, 0x00, sizeof caBitMap );
	memcpy ( caBitMap, pcaMsg + iCurLen, iBitMapLen );
	
	iBitMapLen = ( caBitMap[0] & 0x80 ) ? 16 : 8;
	iCurLen += iBitMapLen;
	
	convertCharMap ( caBitMap, caCharMap );
	
	for ( i = 2; i <= iBitMapLen * 8; i++ )
	{
		if ( caCharMap[i] == '1' )
		{
			getFieldInfo ( &fieldDesc, i );
			
			/* memset ( g_caMsg, 0x00, sizeof g_caMsg );
			sprintf ( g_caMsg, "fieldno[%d], fieldattr[%d], fieldlen[%d]", 
			fieldDesc.fieldno, fieldDesc.fieldattr, fieldDesc.fieldlen );
			writeLog ( INFO_LOG, g_caMsg, 0, 0 ); */
			
			if ( fieldDesc.fieldattr == 0 )
			{
				/* memset ( caValue, 0x00, sizeof caValue );
				memcpy ( caValue, pcaMsg + iCurLen, fieldDesc.fieldlen ); */
				
				tranPutField ( i, pcaMsg + iCurLen, fieldDesc.fieldlen, tranDesc );
				iCurLen += fieldDesc.fieldlen; 
				
				/* memset ( g_caMsg, 0x00, sizeof g_caMsg );
				sprintf ( g_caMsg, "第[%03d]域:[%s] 长度[%d]", i, caValue, fieldDesc.fieldlen );
				writeLog ( INFO_LOG, g_caMsg, 0, 0 ); */
			}else
			{
				memset ( caTemp, 0x00, sizeof caTemp );
				memcpy ( caTemp, pcaMsg + iCurLen, fieldDesc.fieldattr );
				iCurBit = atoi ( caTemp );
				iCurLen += fieldDesc.fieldattr;
				
				/* memset ( caValue, 0x00, sizeof caValue );
				memcpy ( caValue, pcaMsg + iCurLen, iCurBit ); */
				
				tranPutField ( i, pcaMsg + iCurLen, iCurBit, tranDesc );
				iCurLen += iCurBit;
				
				/* memset ( g_caMsg, 0x00, sizeof g_caMsg );
				sprintf ( g_caMsg, "第[%03d]域:[%s] 长度[%d]", i, caValue, iCurBit );
				writeLog ( INFO_LOG, g_caMsg, 0, 0 ); */
			}
		}
	}
	
	memset ( g_caMsg, 0x00, sizeof g_caMsg );
	sprintf ( g_caMsg, "8583报文实际解包长度长度[%d]", iCurLen - 8 - 68 );
	writeLog ( INFO_LOG, g_caMsg, 0, 0 );
	
	if ( iCurLen != iLen )
	{
		writeLog ( ERR_LOG, "传入报文长度与实际解包长度不符，解包失败", 0, 0 );
		return -1;
	}
	
	return 0;
	
}
