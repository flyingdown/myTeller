#include "common.h"

extern pthread_mutex_t * g_sh_myLock;
extern int * g_sh_suCount;

void * makeShareMemory ( size_t stSize )
{
	void * mem = NULL;
	stSize /= getpagesize ();
	stSize++;
	
	mem = mmap ( NULL, stSize * getpagesize (), PROT_READ | PROT_WRITE, 
	MAP_SHARED | MAP_ANONYMOUS, -1, 0 ); 
	if ( mem == MAP_FAILED )
	{
		memset ( g_caMsg, 0x00, sizeof g_caMsg );
		sprintf ( g_caMsg, "”≥…‰ƒ⁄¥Ê“≥ ß∞‹, [%s]", strerror ( errno ) );
		writeLog ( ERR_LOG, g_caMsg, 0, 0 );
	}
	
	memset ( g_caMsg, 0x00, sizeof g_caMsg );
	sprintf ( g_caMsg, "∑÷≈‰µÿ÷∑[%p]", mem );
	writeLog ( ERR_LOG, g_caMsg, 0, 0 );
	
	return mem;
}

int destoryShareMemory ( void * start, size_t stSize )
{
	stSize /= getpagesize ();
	stSize++;
	
	return munmap ( start, stSize * getpagesize () );
}

int addSuCount ()
{
	int iRet = 0;
	
	iRet = pthread_mutex_lock ( g_sh_myLock );
	if ( iRet )
	{
		writeLog ( ERR_LOG, strerror(errno), 0, 0 );
		return -1;
	}
	
	( * g_sh_suCount )++;
	
	iRet = pthread_mutex_unlock ( g_sh_myLock );
	if ( iRet )
	{
		writeLog ( ERR_LOG, strerror(errno), 0, 0 );
		return -1;
	}
	
	return 0;
}
