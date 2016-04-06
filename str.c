#include "common.h"

/* É¾³ý¿Õ¸ñ */
void delSpace ( char * caBuf )
{
	int i = 0, j = 0;
	int iLen = strlen ( caBuf );
	
	for ( i = 0; i < iLen; i++ )
	{
		if ( caBuf[i] != ' ' && caBuf[i] != '\t' )
			caBuf[j++] = caBuf[i];
	}
	
	if ( caBuf[j-1] = '\n' )
		caBuf[j-1] = '\0';
	else
		caBuf[j] = '\0';
}



/* È¡×Ö·û´® */
int doGetStringBySplit ( const char * caBuf, char * caSplitFlag, int iPosition, char * caData )
{
	int iLen = 0, i = 0, j = 0;
	char * pStr = NULL, * pEnd = NULL, * p = NULL;
	iLen = strlen ( caBuf );
	
	p = caBuf;
	while ( i < iPosition )
	{
		pStr = p;
		
		pEnd = strstr ( p, caSplitFlag );
		if ( ! pEnd )
		{
			pEnd = caBuf + iLen;
			break;
		}
		
		p = pEnd + strlen ( caSplitFlag );
		
		i++;
	}
	
	memcpy ( caData, pStr, pEnd-pStr );
	
	return 0;
	
}