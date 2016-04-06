#include "common.h"


int getFieldInfo ( FIELDDESC * fieldDesc, int fieldNo )
{
	char caTemp[3+1];
	
	char * fieldStyle[MAX_FIELD_STYLE_SIZE] = {
		
		/*
		 *  fieldattr 位元属性 0-定长 1-一位变长 2-两位变长
		 *  fieldlen 位元长度 变长时为最大长度
		 *  fieldtype 位元类型 1-字符类型 2-数值类型
		 */
		
		/*
						0, 1, 2, 3, 4, 5, 6, 7
						8, 9, 10, 11, 12, 13, 14, 15
		*/
		
     "       ",     "1:0:5:2",     "2:0:5:1",     "3:0:5:1",     "4:0:9:2",     "5:0:8:2",     "6:0:6:2",     "7:0:6:1",
     "8:0:6:1",    "9:3:27:1",   "10:0:12:1",    "11:0:1:1",    "12:0:4:1",   "13:0:50:1",    "14:0:8:2",  "15:3:127:1",
    
    /* 16,  17,  18,  19,  20,  21,  22,  23,
     * 24,  25,  26,  27,  28,  29,  30,  31,
     */
     
    "16:0:4:1",    "17:0:6:2",    "18:0:4:1",   "19:3:17:1",    "20:0:1:1",    "21:0:2:1",    "22:0:2:1",    "23:0:3:1",
    "24:0:3:1",   "25:0:60:1",   "26:0:60:1",   "27:0:60:1",    "28:0:8:2",    "29:0:8:2",   "30:0:24:1",   "31:0:24:1",
  
   /* 32,  33,  34,  35,  36,  37,  38,  39,
    * 40,  41,  42,  43,  44,  45,  46,  47,
    */
   
   "32:0:24:1",   "33:0:24:1",    "34:0:4:2",    "35:0:4:2",    "36:0:4:2",   "37:0:24:1",   "38:0:24:1",   "39:0:16:2",
   "40:0:16:2",   "41:0:16:2",   "42:0:16:2",   "43:0:16:2",    "44:0:8:2",    "45:0:8:2",    "46:0:8:2",    "47:0:8:2",
    
    /* 48,  49,  50,  51,  52,  53,  54,  55,
     * 56,  57,  58,  59,  60,  61,  62,  63,
     */
     
    "48:0:4:2",    "49:0:4:2",    "50:0:3:2",    "51:0:3:2",   "52:0:12:2",   "53:0:10:2",   "54:0:10:2",   "55:0:10:2",
   "56:0:19:1",   "57:0:19:1",   "58:0:16:1",   "59:0:16:1",   "60:0:16:1",   "61:0:16:1",   "62:0:20:1",   "63:0:20:1",
   
   /* 64,  65,  66,  67,  68,  69,  70,  71,
    * 72,  73,  74,  75,  76,  77,  78,  79,
    */
   
   "64:0:10:1",   "65:0:10:1",    "66:0:1:1",    "67:0:1:1",    "68:0:1:1",    "69:0:1:1",    "70:0:1:1",    "71:0:1:1",
    "72:0:1:1",   "73:0:10:1",   "74:0:76:1",   "75:0:37:1",  "76:0:141:1",    "77:0:9:2",    "78:0:9:2",   "79:0:16:1",
  
   /* 80,  81,  82,  83,  84,  85,  86,  87,
    * 88,  89,  90,  91,  92,  93,  94,  95,
    */
  
   "80:0:16:1",   "81:0:60:1",   "82:0:60:1",   "83:0:60:1",    "84:0:8:1",    "85:0:8:1",   "86:0:20:1",   "87:0:20:1",
   "88:0:20:1",    "89:0:3:1",    "90:0:3:1",    "91:0:5:1",    "92:0:6:1",    "93:0:1:1",    "94:0:8:1",  "95:0:600:1",
   
   /* 96,  97,  98,  99,  100,  101,  102, 103,
    * 104,  105,  106,  107,  108,  109,  110,  111,
    */
  
  "96:3:691:1",    "97:0:8:1",    "98:0:1:1",    "99:0:1:1", "100:3:112:1", "101:3:205:1", "102:3:412:1", "103:3:173:1", 
   "104:3:0:1", "105:3:174:1",  "106:3:24:1", "107:3:279:1", "108:3:359:1", "109:3:270:1",   "110:3:0:1",   "111:3:0:1",
 
   /* 112,  113,  114,  115,  116,  117,  118,  119,
    * 120,  121,  122,  123,  124,  125,  126,  127,
    */
 
 "112:3:154:1",  "113:3:63:1",  "114:3:59:1",  "115:2:59:1", "116:3:162:1",  "117:3:17:1",  "118:3:90:1",  "119:3:90:1",
 "120:3:162:1", "121:3:162:1", "122:3:260:1", "123:3:999:1", "124:3:226:1", "125:3:281:1",  "126:3:43:1",   "127:0:1:1",
  
   /* 128 */
  
  "128:0:8:1" 
  };
  
  if ( fieldNo >= MAX_FIELD_STYLE_SIZE )
  {
  	writeLog ( ERR_LOG, "非法域号", 0, 0 );
  	return -1;
  }
  
  fieldDesc->flag = 0;
  fieldDesc->fieldno = fieldNo;
  
  memset ( caTemp, 0x00, sizeof caTemp );
  doGetStringBySplit ( fieldStyle[fieldNo], ":", 2, caTemp );
  fieldDesc->fieldattr = atoi ( caTemp );
  
  memset ( caTemp, 0x00, sizeof caTemp );
  doGetStringBySplit ( fieldStyle[fieldNo], ":", 3, caTemp );
  fieldDesc->fieldlen = atoi ( caTemp );
  
  memset ( caTemp, 0x00, sizeof caTemp );
  doGetStringBySplit ( fieldStyle[fieldNo], ":", 4, caTemp );
  fieldDesc->fieldtype = atoi ( caTemp );
  
  return 0;
  
}

int fillFieldInfo ( TRANDESC * tranDesc, int fieldNo )
{
	int iRet = 0;
	FIELDDESC * tmp = NULL;
	
	if ( tranDesc->cur_field == tranDesc->max_field )
	{
		tranDesc->max_field += FIELD_UNIT;
		tmp = ( FIELDDESC * ) realloc ( tranDesc->fielddesc, tranDesc->max_field * sizeof ( FIELDDESC ) );
		if ( ! tmp )
		{
			writeLog ( ERR_LOG, "重新分配空间失败", 0, 0 );
			return -1;
		}
		
		tranDesc->fielddesc = tmp;
		tmp = NULL;
		
		memset ( g_caMsg, 0x00, sizeof g_caMsg );
		sprintf ( g_caMsg, "tranDesc->fielddesc新地址%p, 重新分配空间后大小为%zu", 
		tranDesc->fielddesc, tranDesc->max_field * sizeof ( FIELDDESC ) );
		writeLog ( INFO_LOG, g_caMsg, 0, 0 );
		
	}
	
	iRet = getFieldInfo ( tranDesc->fielddesc + tranDesc->cur_field, fieldNo );
	if ( iRet < 0 )
	{
		writeLog ( ERR_LOG, "初始化单个域信息错误", 0, 0 );
		return -1;
	}
	
	tranDesc->cur_field++;
	
	return 0;
}

int initTranDesc ( TRANDESC ** tranDesc )
{
	int i = 0, iRet = 0;
	
	if ( *tranDesc != NULL )
		return -1;
		
	* tranDesc = ( TRANDESC * ) malloc ( sizeof ( TRANDESC ) );
	if ( *tranDesc == NULL )
	{
		writeLog ( ERR_LOG, strerror ( errno ), 0, 0 );
		return -1;
	}
	
	writeLog ( INFO_LOG, "交易描述结构分配内存成功", 0, 0 );
	
	( * tranDesc )->max_valuelen = VALUE_UNIT;
	( * tranDesc )->max_field = FIELD_UNIT;
	( * tranDesc )->cur_valuelen = 0;
	( * tranDesc )->cur_field = 0;
	( * tranDesc )->fielddesc = NULL;
	( * tranDesc )->value = NULL;
	
	( * tranDesc )->fielddesc = ( FIELDDESC * ) calloc ( FIELD_UNIT, sizeof ( FIELDDESC ) );
	if ( ( * tranDesc )->fielddesc == NULL )
	{
		free ( * tranDesc );
		writeLog ( ERR_LOG, strerror ( errno ), 0, 0 );
		return -1;
	}
	
	writeLog ( INFO_LOG, "域描述结构分配内存成功", 0, 0 );
	
	( * tranDesc )->value = ( char * ) calloc ( VALUE_UNIT, sizeof ( char ) );
	if ( ( * tranDesc )->value == NULL )
	{
		free ( * tranDesc );
		free ( ( * tranDesc )->fielddesc );
		writeLog ( ERR_LOG, strerror ( errno ), 0, 0 );
		return -1;
	}
	
	writeLog ( INFO_LOG, "数据存储区分配内存成功", 0, 0 );
	
	for ( i = 0; i < MAX_FIELD_STYLE_SIZE; i++ )
	{
		iRet = fillFieldInfo ( * tranDesc, i );
		if ( iRet < 0 )
		{
			free ( * tranDesc );
			free ( ( * tranDesc )->fielddesc );
			free ( ( * tranDesc )->value );
			writeLog ( ERR_LOG, "初始化域信息错误", 0, 0 );
			return -1;
		}
	}
	
	writeLog ( INFO_LOG, "初始化域信息成功", 0, 0 );
	/*
	for ( i = 0; i < MAX_FIELD_STYLE_SIZE; i++ )
	{
		memset ( g_caMsg, 0x00, sizeof g_caMsg );
		sprintf ( g_caMsg, "flag[%d], fieldno[%d], fieldattr[%d], fieldlen[%d], fieldtype[%d]", 
		( ( * tranDesc )->fielddesc + i )->flag, ( ( * tranDesc )->fielddesc + i )->fieldno, 
		( ( * tranDesc )->fielddesc + i )->fieldattr, ( ( * tranDesc )->fielddesc + i )->fieldlen, 
		( ( * tranDesc )->fielddesc + i )->fieldtype );
		writeLog ( INFO_LOG, g_caMsg, 0, 0 );
	} */
	
	return 0;
}


int destoryTranDesc ( TRANDESC * tranDesc )
{
	if ( tranDesc->fielddesc )
	{
		free ( tranDesc->fielddesc );
		tranDesc->fielddesc = NULL;
	}
	
	writeLog ( INFO_LOG, "释放域内存成功", 0, 0 );
		
	if ( tranDesc->value )
	{
		free ( tranDesc->value );
		tranDesc->value = NULL;
	}
	
	writeLog ( INFO_LOG, "释放域值内存成功", 0, 0 );
		
	if ( tranDesc )
	{
		free ( tranDesc );
		tranDesc = NULL;
	}
	
	writeLog ( INFO_LOG, "释放交易内存成功", 0, 0 );
	
	return 0;
	
}

int tranPutField ( int fieldNo, const void * value, int iLen, 
TRANDESC * tranDesc )
{
	int i = 0, iRet = 0;
	void * tmp = NULL;
	
	for ( i = 0; i < tranDesc->cur_field; i++ )
	{
		/* memset ( g_caMsg, 0x00, sizeof g_caMsg );
		sprintf ( g_caMsg, "i=[%d], fieldno[%d], ", i, ( tranDesc->fielddesc + i )->fieldno );
		writeLog ( INFO_LOG, g_caMsg, 0, 0 ); */
		if ( ( tranDesc->fielddesc + i )->fieldno == fieldNo )
			break;
	}
	
	if ( i == tranDesc->cur_field )
	{
		tranDesc->max_field += FIELD_UNIT;
		tmp = ( FIELDDESC * ) realloc ( tranDesc->fielddesc, tranDesc->max_field * sizeof ( FIELDDESC ) );
		if ( ! tmp )
		{
			writeLog ( ERR_LOG, "重新分配空间失败", 0, 0 );
			return -1;
		}
		
		tranDesc->fielddesc = tmp;
		tmp = NULL;
		
		memset ( g_caMsg, 0x00, sizeof g_caMsg );
		sprintf ( g_caMsg, "tranDesc->fielddesc新地址%p, 重新分配空间后大小为%zu", 
		tranDesc->fielddesc, tranDesc->max_field * sizeof ( FIELDDESC ) );
		writeLog ( ERR_LOG, g_caMsg, 0, 0 );
		
		if ( tranDesc->cur_valuelen + iLen > tranDesc->max_valuelen )
		{
			tranDesc->max_valuelen += VALUE_UNIT;
			tmp = ( char * ) realloc ( tranDesc->value, tranDesc->max_valuelen * sizeof ( char ) );
			if ( ! tmp )
			{
				writeLog ( ERR_LOG, "重新分配空间失败", 0, 0 );
				return -1;
			}
			
			tranDesc->value = tmp;
			tmp = NULL;
			
		}
		
		( tranDesc->fielddesc + i )->flag = 1;
		( tranDesc->fielddesc + i )->fieldno = fieldNo;
		( tranDesc->fielddesc + i )->fieldlen = iLen;
		( tranDesc->fielddesc + i )->offset = tranDesc->cur_valuelen;
		tranDesc->cur_field++;
		memcpy ( tranDesc->value + tranDesc->cur_valuelen, value, iLen );
		tranDesc->cur_valuelen += iLen;
		
		return 0;
	}
	
	if ( tranDesc->cur_valuelen + iLen > tranDesc->max_valuelen )
	{
		tranDesc->max_valuelen += VALUE_UNIT;
		tmp = ( char * ) realloc ( tranDesc->value, tranDesc->max_valuelen * sizeof ( char ) );
		if ( ! tmp )
		{
			writeLog ( ERR_LOG, "重新分配空间失败", 0, 0 );
			return -1;
		}
		
		tranDesc->value = tmp;
		tmp = NULL;
		
	}
	
	if ( ( tranDesc->fielddesc + i )->fieldattr != 0 )
	{
		if ( ( tranDesc->fielddesc + i )->fieldlen >= iLen )
		{
			( tranDesc->fielddesc + i )->fieldlen = iLen;
		}else
		{
			memset ( g_caMsg, 0x00, sizeof g_caMsg );
			sprintf ( g_caMsg, "域[%d]最大长度为%d, 长度非法!", fieldNo, 
			( tranDesc->fielddesc + i )->fieldlen );
			writeLog ( ERR_LOG, g_caMsg, 0, 0 );
			return -1;
		} 
		
		( tranDesc->fielddesc + i )->offset = tranDesc->cur_valuelen;
		memcpy ( tranDesc->value + tranDesc->cur_valuelen, value, iLen );
		tranDesc->cur_valuelen += iLen;
			
	}else
	{
		if ( ( tranDesc->fielddesc + i )->fieldlen < iLen )
			iLen = ( tranDesc->fielddesc + i )->fieldlen;
		
		if ( ( tranDesc->fielddesc + i )->fieldtype == 2 )
		{
			memset ( tranDesc->value + tranDesc->cur_valuelen, '0', ( tranDesc->fielddesc + i )->fieldlen );
			memcpy ( tranDesc->value + tranDesc->cur_valuelen + ( tranDesc->fielddesc + i )->fieldlen - iLen, value, iLen );
		}else
		{
			memset ( tranDesc->value + tranDesc->cur_valuelen, 0x20, ( tranDesc->fielddesc + i )->fieldlen );
			memcpy ( tranDesc->value + tranDesc->cur_valuelen + ( tranDesc->fielddesc + i )->fieldlen - iLen, value, iLen );
		}
		
		( tranDesc->fielddesc + i )->offset = tranDesc->cur_valuelen;
		tranDesc->cur_valuelen += ( tranDesc->fielddesc + i )->fieldlen;
	}
	
	( tranDesc->fielddesc + i )->flag = 1;
	/* tranDesc->cur_field++; */
	/*
	memset ( g_caMsg, 0x00, sizeof g_caMsg );
	sprintf ( g_caMsg, "域[%d]", ( tranDesc->fielddesc + i )->fieldno );
	writeLog ( INFO_LOG, g_caMsg, tranDesc->value + ( tranDesc->fielddesc + i )->offset, ( tranDesc->fielddesc + i )->fieldlen );
	*/
	return 0;
	
	
}

int tranGetField ( int fieldNo, char ** value, TRANDESC * tranDesc )
{
	int i = 0, iRet = 0;
	
	for ( i = 0; i < tranDesc->cur_field; i++ )
	{
		if ( ( tranDesc->fielddesc + i )->fieldno == fieldNo 
			&& ( tranDesc->fielddesc + i )->flag == 1 )
			break;
	}
	
	if ( i == tranDesc->cur_field )
	{
		memset ( g_caMsg, 0x00, sizeof g_caMsg );
		sprintf ( g_caMsg, "域[%d]不存在", fieldNo );
		writeLog ( ERR_LOG, g_caMsg, 0, 0 );
		return -1;
	}
	
	* value = tranDesc->value + ( tranDesc->fielddesc + i )->offset;
	
	return ( tranDesc->fielddesc + i )->fieldlen;
}

int tranDelField ( int fieldNo, TRANDESC * tranDesc )
{
	int i = 0, j = 0, iDlen = 0, iOffset = 0;
	
	for ( i = 0; i < tranDesc->cur_field; i++ )
	{
		if ( ( tranDesc->fielddesc + i )->fieldno == fieldNo 
			&& ( tranDesc->fielddesc + i )->flag == 1 )
			break;
	}
	
	if ( i == tranDesc->cur_field )
	{
		memset ( g_caMsg, 0x00, sizeof g_caMsg );
		sprintf ( g_caMsg, "域[%d]不存在", fieldNo );
		writeLog ( ERR_LOG, g_caMsg, 0, 0 );
		return -1;
	}
	
	iDlen = ( tranDesc->fielddesc + i )->fieldlen;
	iOffset = ( tranDesc->fielddesc + i )->offset;
	( tranDesc->fielddesc + i )->flag = 0;
	memmove ( tranDesc->value + iOffset, tranDesc->value + iOffset + iDlen, 
	tranDesc->cur_valuelen - iOffset - iDlen );
	
	for ( j = i; j < tranDesc->cur_field; j++ )
	{
		( tranDesc->fielddesc + j )->offset -= iDlen;
	}
	
	tranDesc->cur_valuelen -= iDlen;
	
	return 0;
	
}

void printTranDesc ( TRANDESC * tranDesc )
{
	int i = 0;
	
	for ( i = 0; i < MAX_FIELD_STYLE_SIZE; i++ )
	{
		if ( ( tranDesc->fielddesc + i )->flag == 1 )
		{
			sprintf ( g_caMsg, "域[%d]", ( tranDesc->fielddesc + i )->fieldno );
			writeLog ( INFO_LOG, g_caMsg, tranDesc->value + ( tranDesc->fielddesc + i )->offset, ( tranDesc->fielddesc + i )->fieldlen );
		}
	}
}

int clearAllField ( TRANDESC * tranDesc )
{
	int i = 0;
	
	for ( i = 0; i < tranDesc->cur_field; i++ )
	{
		( tranDesc->fielddesc + i )->flag = 0;
		( tranDesc->fielddesc + i )->offset = 0;
	}
	
	memset ( tranDesc->value, 0x00, tranDesc->max_valuelen );
	tranDesc->cur_valuelen = 0;
	
	return 0;
	
}