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
			writeLog ( ERR_LOG, "#############����ʧ��############", 0, 0 );
			printf ( "��������[%d]�ʽ���ʧ��.����ID[%d]������\n", i, getpid () );
			iOk = -1;
		}else
			printf ( "�̵̡̡�[%d]�ʽ��׳ɹ�.����ID[%d]�̡̡�\n", i, getpid () );
		
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
	
	/* ��ʼ����־�� 
	memset ( g_LogName, 0x00, sizeof g_LogName );*/
	sprintf ( g_LogName, "./log/emulation%s_%08d.log", tranCode, getpid () );
	writeLog ( INFO_LOG, "*******************�������̿�ʼ*******************", 0, 0 );
	
	/* ��ʼ���������� */
	iRet = initTranDesc ( & tranDesc );
	if ( iRet < 0 )
	{
		writeLog ( ERR_LOG, "initTranDesc fail", 0, 0 );
		return -1;
	}
	
	/* ������������ */
	readServerConfig ( serverInfo );
	if ( iRet < 0 )
	{
		writeLog ( ERR_LOG, "readServerConfig fail", 0, 0 );
		destoryTranDesc ( tranDesc );
		return -1;
	}
	
	/* ���������� */
	iRet = readTranInfoConfig ( tranCode, tranDesc );
	if ( iRet < 0 )
	{
		writeLog ( ERR_LOG, "readTranInfoConfig fail", 0, 0 );
		destoryTranDesc ( tranDesc );
		return -1;
	}
	
	/* ��λͼ���� */
	iRet = readBitMapConfig ( tranCode, caBitMap );
	if ( iRet < 0 )
	{
		writeLog ( ERR_LOG, "readBitMapConfig fail", 0, 0 );
		destoryTranDesc ( tranDesc );
		return -1;
	}
	
	/* ��� */
	memset ( caMsg, 0x00, sizeof caMsg );
	iLen = doPackTran ( tranDesc, caBitMap, caMsg );
	if ( iLen < 0 )
	{
		writeLog ( ERR_LOG, "���ʧ��", 0, 0 );
		destoryTranDesc ( tranDesc );
		return -1;
	}
	writeLog ( INFO_LOG, "λͼ", caBitMap, 16 );
	writeLog ( INFO_LOG, "��������", caMsg, iLen );
	
	/* ��ʼ��SOCKET */
	sockfd = initSocket ();
	if ( sockfd < 0 )
	{
		writeLog ( ERR_LOG, "initSocketʧ��", 0, 0 );
		destoryTranDesc ( tranDesc );
		return -1;
	}
	
	/* ���ý��ճ�ʱʱ�� */
	timeout.tv_sec = serverInfo->timeout;
	timeout.tv_usec = 0;
	iRet = setsockopt ( sockfd, SOL_SOCKET, SO_RCVTIMEO, ( char * ) &timeout, sizeof( timeout ) );
	if ( iRet < 0 )
	{
		writeLog ( ERR_LOG, "setsockoptʧ��", 0, 0 );
		destoryTranDesc ( tranDesc );
		return -1;
	}
	
	/* ���ӷ����� */
	iRet = sockConnect ( sockfd, serverInfo->ipaddr, serverInfo->port );
	if ( iRet < 0 )
	{
		writeLog ( ERR_LOG, "sockConnectʧ��", 0, 0 );
		destoryTranDesc ( tranDesc );
		return -1;
	}
	
	/* ��ȡ������Ϣ */
	getSockInfo ( sockfd );
	
	/* ������Ϣ */
	iRet = sockSend ( sockfd, caMsg, iLen );
	if ( iRet < 0 )
	{
		writeLog ( ERR_LOG, "sockSendʧ��", 0, 0 );
		destoryTranDesc ( tranDesc );
		return -1;
	}
	writeLog ( INFO_LOG, "*******************�������̽���*******************", 0, 0 );
	
	writeLog ( INFO_LOG, "*******************Ӧ�����̿�ʼ*******************", 0, 0 );
	/* �������� */
	memset ( caMsg, 0x00, sizeof caMsg );
	iMsgLen = sockRecv ( sockfd, caMsg, sizeof caMsg );
	if ( iMsgLen <= 0 )
	{
		writeLog ( ERR_LOG, "sockRecvʧ��", 0, 0 );
		destoryTranDesc ( tranDesc );
		return -1;
	}
	
	/* �ر���· */
	iRet = sockClose ( sockfd );
	if ( iRet < 0 )
	{
		writeLog ( ERR_LOG, "sockCloseʧ��", 0, 0 );
		destoryTranDesc ( tranDesc );
		return -1;
	}
	
	/* �������ṹ�� */
	clearAllField ( tranDesc );
	
	/* ��� */
	iRet = doUnPackTran ( tranDesc, caMsg, iMsgLen );
	if ( iRet < 0 )
	{
		writeLog ( ERR_LOG, "���ʧ��", 0, 0 );
		destoryTranDesc ( tranDesc );
		return -1;
	}
	
	/* ��Ӧ�� */
	iRet = tranGetField ( 12, &pValue, tranDesc );
	if ( iRet < 0 )
	{
		writeLog ( ERR_LOG, "δ��ȡ����Ӧ��", 0, 0 );
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
			writeLog ( ERR_LOG, "���״���", 0, 0 );
			destoryTranDesc ( tranDesc );
			return -1;
		}
		
		addSuCount ();
	}
	
	printTranDesc ( tranDesc );
	
	destoryTranDesc ( tranDesc );
	writeLog ( INFO_LOG, "*******************Ӧ�����̽���*******************", 0, 0 );
	
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
			sprintf ( g_caMsg, "��[%d]����...", i );
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
				sprintf ( g_caMsg, "δ��ȡ����[%d]��ֵ", i );
				writeLog ( ERR_LOG, g_caMsg, 0, 0 );
			}
			
			getFieldInfo ( &fieldDesc, i );
			if ( fieldDesc.fieldattr == 0 ) /* ���� */
			{
				if ( iRet > 0 )
					memcpy ( p + iCurLen, pValue, iRet );
				else 
					memset ( p + iCurLen, 0x20, fieldDesc.fieldlen );
					
				iRet = fieldDesc.fieldlen;
			}else /* �䳤 */
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
	sprintf ( g_caMsg, "8583���Ĵ��볤��[%d]", iLen - iCurLen );
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
				sprintf ( g_caMsg, "��[%03d]��:[%s] ����[%d]", i, caValue, fieldDesc.fieldlen );
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
				sprintf ( g_caMsg, "��[%03d]��:[%s] ����[%d]", i, caValue, iCurBit );
				writeLog ( INFO_LOG, g_caMsg, 0, 0 ); */
			}
		}
	}
	
	memset ( g_caMsg, 0x00, sizeof g_caMsg );
	sprintf ( g_caMsg, "8583����ʵ�ʽ�����ȳ���[%d]", iCurLen - 8 - 68 );
	writeLog ( INFO_LOG, g_caMsg, 0, 0 );
	
	if ( iCurLen != iLen )
	{
		writeLog ( ERR_LOG, "���뱨�ĳ�����ʵ�ʽ�����Ȳ��������ʧ��", 0, 0 );
		return -1;
	}
	
	return 0;
	
}
