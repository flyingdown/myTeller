#include "common.h"

static int g_LogLevel;
char g_LogName[512] = "emulationDebug.log";
char *g_LogMsg[] = {
	"INFO", "DEBUG", "WARN", "ERR"
};

/*
static int toPrintCh ( void *buff, int len )
{
	int i = 0;
	char * tempBuf = buff;
	
	if ( tempBuf == NULL )
	{
		puts ( "输入参数不能为空" );
		retrun -1;
	}
	
	for ( i = 0; i < strlen ( tempBuf ); i++ )
	{
		if ( iscntrl ( tempBuf[i] ) )
			tempBuf[i] = '*';
	}
		
	return 0;
} */


static int memWrite ( int fd, const void * mem, int iLen )
{
	int i = 0, j = 0, k = 0, iTmpLen = 0, iTotal = 0, iRet = 0;
	char caTemp[32], caBuf[BUFFER_SIZE];
	const char * p = ( const char * ) mem;
	
	memset ( caTemp, 0x00, sizeof caTemp );
	memset ( caBuf, 0x00, sizeof caBuf );
	
	iTotal = iLen >> 4;
	
	strcpy ( caBuf, "Displacement  +00+01+02+03+04+05+06+07+08+09+10+11+12+13+14+15+  ++ASCII  Value++\n" );
	iTmpLen = strlen ( caBuf );
	
	for ( i = 0; i < iTotal + 1; i++ )
	{
		k = 16; /* 每行打印字符数 */
		
		if ( i == iTotal ) /* 最后一行不足16个字符时 */
			k = iLen % 16;
			
		if ( ! k ) /* 打印字符数刚好为16的倍数 */
			break;
		
		sprintf ( caBuf + iTmpLen, "%05d(%05X)   ", i*16, i*16 );
		iTmpLen += 15;
		
		for ( j = 0; j < k; j++ )
		{
			sprintf ( caBuf + iTmpLen, "%02X ", ( unsigned char ) * ( p + j + i * 16 ) );
			iTmpLen += 3;
		}
		
		for ( j = k; j < 16; j++ )
		{
			memcpy ( caBuf + iTmpLen, "   ", 3 );
			iTmpLen += 3;
		}
		
		memcpy ( caBuf + iTmpLen, "  ", 2 );
		iTmpLen += 2;
		
		for ( j = 0; j < k; j++ )
		{
			if ( iscntrl ( * ( p + j + i * 16 ) ) )
				memcpy ( caBuf + iTmpLen, "*", 1 );
			else
				sprintf ( caBuf + iTmpLen, "%c", ( unsigned char ) * ( p + j + i * 16 ) );
				
			iTmpLen += 1;
		}
		
		caBuf[iTmpLen++] = '\n';
		
	}
	
	write ( fd, caBuf, iTmpLen );
	
	
	return iTmpLen;
	
}

void initLogLevel ( int iLogLevel )
{
	g_LogLevel = iLogLevel;
}

void chgLogLevel ( int iLogLevel )
{
	g_LogLevel = iLogLevel;
}

int writeLog ( 
	unsigned int logLevel, 
	const char * fileName, 
	const char * funcName, 
	int iLineNo, 
	const char * caMsg, 
	const char * mem,
	int iLen 
)
{
	int fd_log, iRet = 0;
	struct timeval tClock;
	struct tm * tTime;
	char caStr[MAX_BUF_LEN];
	char caTime[32];
	
	if ( fileName == NULL || funcName == NULL || caMsg == NULL )
		return -1;
	
	memset ( caStr, 0x00, sizeof caStr );
	memset ( caTime, 0x00, sizeof caTime );
		
	if ( logLevel < g_LogLevel )
		return 0;
	
	switch ( logLevel )
	{
		case 0x1000 :
		case 0x2000 :
		case 0x3000 :
		case 0x4000 :
			break;
		default :
			puts ( "日志级别非法" );
			printf ( "%x", logLevel );
			exit ( -1 );
	}
		
	fd_log = open ( g_LogName, O_RDWR | O_CREAT | O_APPEND | O_EXCL, 0644 );
	if ( fd_log < 0 )
	{
		if ( errno == EEXIST )
		{
			fd_log = open ( g_LogName, O_RDWR | O_APPEND );
			if ( fd_log < 0 )
			{
				puts ( "打开日志错误" );
				exit ( -1 );
			}
		}else
		{
				puts ( "打开日志错误" );
				exit ( -1 );
		}
			
	}
	
	gettimeofday ( &tClock, 0 );
	tTime = localtime ( &tClock.tv_sec );
	strftime ( caTime, sizeof caTime, "%Y/%m/%d-%H:%M:%S", tTime );
	sprintf ( caStr, "Pid [%d] | File[%s] Function[%s] Line[%d] | Message :\n", 
	getpid (), fileName, funcName, iLineNo );
	sprintf ( caStr + strlen ( caStr ), "Time [%s.%06ld] | %s>> %s \n", 
	caTime, tClock.tv_usec, g_LogMsg[(logLevel >> 12) - 1], caMsg );
	
	iRet = write ( fd_log, caStr, strlen ( caStr ) );
	if ( iRet < 0 )
	{
		puts ( "写日志错误" );
		exit ( -1 );
	}
	
	if ( iLen > 0 )
		memWrite ( fd_log, mem, iLen );
	
	close ( fd_log );
	
	return 0;
}  

void printLog ( const char * mem, int iLen )
{
	if ( iLen > 0 )
		memWrite ( STDOUT_FILENO, mem, iLen );
}
