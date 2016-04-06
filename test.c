#include <time.h>

#include "common.h"

int main ()
{
	int iRet = 0;
	char caValue[5+1];
	char * pValue = NULL;
	char caBitMap[BITMAP_LEN];
	time_t tClock;
	tClock = time ( NULL );
	printf ( "%s", ctime ( &tClock ) );
	printf ( "%08d\n", 220 );
	
	/* TRANDESC * tranDesc = NULL;
	iRet = initTranDesc ( &tranDesc );
	if ( iRet < 0 )
	{
		writeLog ( ERR_LOG, "initTranDesc fail", 0, 0 );
		return -1;
	}
	
	iRet = readTranInfoConfig ( "3601", tranDesc );
	if ( iRet < 0 )
	{
		writeLog ( ERR_LOG, "readTranInfoConfig fail", 0, 0 );
		destoryTranDesc ( tranDesc );
		return -1;
	}
					
	printTranDesc ( tranDesc ); 
	
	memset ( caBitMap, 0x00, sizeof caBitMap );
	iRet = readBitMapConfig ( "3601", caBitMap );
	if ( iRet < 0 )
	{
		writeLog ( ERR_LOG, "readBitMapConfig fail", 0, 0 );
		destoryTranDesc ( tranDesc );
		return -1;
	}
	
	writeLog ( INFO_LOG, "位图", caBitMap, BITMAP_LEN );
	
	iRet = readServerConfig ( &serverInfo );
	if ( iRet < 0 )
	{
		writeLog ( ERR_LOG, "readServerConfig fail", 0, 0 );
		destoryTranDesc ( tranDesc );
		return -1;
	}
	
	memset ( g_caMsg, 0x00, sizeof g_caMsg );
	sprintf ( g_caMsg, "服务器信息:IP[%s],PORT[%d],TIMEOUT[%d]", serverInfo.ipaddr, 
	serverInfo.port, serverInfo.timeout );
	writeLog ( INFO_LOG, g_caMsg, 0, 0 );
	
	destoryTranDesc ( tranDesc ); */
	
	dealTran ( "3601", 1 );
	
	
}
