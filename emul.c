
#define MAIN

#include "common.h"


int main ( int argc, char * argv[] )
{
	int i = 0, iTranNum = 1, iPnum = 1, iSuCount = 0, iRet = 0;
	char caTranCode[10+1];
	pid_t * pArr = NULL;
	pthread_mutexattr_t mAttr;
	pid_t tempid = 0;
	int tempnum = 0;
	
	memset ( caTranCode, 0x00, sizeof caTranCode );
	
	switch ( 4 - argc )
	{
		case 0 :
			iPnum = atoi ( argv[3] );
		case 1 :
			iTranNum = atoi ( argv[2] );
		case 2 :
			strcpy ( caTranCode, argv[1] );
			break;
		default :
			printf ( "Usage : %s ������ [�������̱���] [������]\n" );
			return -1;
			
	}
	
	pArr = ( pid_t * ) malloc ( iPnum * sizeof ( pid_t ) );
	if ( ! pArr )
	{
		printf ( "malloc�������\n" );
		return -1;
	}
	
	memset ( pArr, 0x00, iTranNum * sizeof ( pid_t ) );
	
	g_sh_suCount = ( int * ) makeShareMemory ( sizeof ( int ) );
	if ( ! g_sh_suCount )
	{
		printf ( "g_sh_suCount makeShareMemory�������\n" );
		return -1;
	}
	
	g_sh_myLock = ( pthread_mutex_t * ) makeShareMemory ( sizeof ( pthread_mutex_t ) );
	if ( ! g_sh_myLock )
	{
		printf ( "g_sh_myLock makeShareMemory�������\n" );
		return -1;
	}
	
	iRet = pthread_mutexattr_init ( &mAttr );
	if ( iRet )
	{
		printf ( "pthread_mutexattr_init����\n" );
		return -1;
	}
	
	iRet = pthread_mutexattr_setpshared ( &mAttr, PTHREAD_PROCESS_SHARED );
	if ( iRet )
	{
		printf ( "pthread_mutexattr_setpshared����\n" );
		return -1;
	}
	
	iRet = pthread_mutex_init ( g_sh_myLock, &mAttr );
	if ( iRet )
	{
		printf ( "pthread_mutex_init����\n" );
		return -1;
	}
	
	for ( i = 0; i < iPnum; i++ )
	{
		pArr[i] = fork ();
		if ( pArr[i] < 0 )
		{
			printf ( "fork fail\n" );
			return -1;
		}else if ( ! pArr[i] )
		{
			iRet = dealTran ( caTranCode, iTranNum );
			if ( iRet < 0 )
			{
				/* printf ( "dealTran fail\n" ); */
				exit ( -1 );
			}
			exit ( 0 );
		}
		
	}
	
	while ( 1 )
	{
		if ( tempnum >= iPnum )
			break;
			
		tempid = waitpid ( -1, NULL, WNOHANG | WUNTRACED | WCONTINUED );
		if ( tempid < 0 )
		{
			printf ( "waitpid error\n" );
			break;
		}else if ( ! tempid )
		{
			sleep ( 1 );
			continue;
		}else
		{
			for ( i = 0; i < iPnum; i++ )
			{
				if ( pArr[i] == tempid )
				{
					tempnum++;
					break;
				}
			}
		}
	}
	
	
	
	iRet = pthread_mutex_destroy ( g_sh_myLock );
	if ( iRet )
	{
		printf ( "pthread_mutex_destroy����\n" );
		return -1;
	}
	
	iRet = pthread_mutexattr_destroy ( &mAttr );
	if ( iRet )
	{
		printf ( "pthread_mutexattr_destroy����\n" );
		return -1;
	}
	
	iSuCount = *g_sh_suCount;
	printf ( "���׳ɹ��� %.2f%%\n", ( ( float )iSuCount / ( iPnum * iTranNum ) ) * 100.0f );
	
	destoryShareMemory ( g_sh_myLock, sizeof ( pthread_mutex_t ) );
	destoryShareMemory ( g_sh_suCount, sizeof ( int ) );
	
	return 0;
	
}