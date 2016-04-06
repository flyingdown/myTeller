#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <strings.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <pthread.h>

#define MAX_BUF_LEN 2048
#define BUFFER_SIZE 1024*512
#define FIELD_UNIT 64
#define VALUE_UNIT 1024
#define MAX_FIELD_STYLE_SIZE 129
#define BITMAP_LEN 17
#define CHARMAP_LEN 129

#define FAILED_ARG EXIT_FAILURE, errno, __FILE__, __LINE__

#ifdef DEBUG
#define INFO_LEVEL      0x1000
#define DEBUG_LEVEL     0x2000
#define WARN_LEVEL      0x3000
#define ERR_LEVEL       0x4000
#else
#define INFO_LEVEL     0x0
#define DEBUG_LEVEL    0x0
#define WARN_LEVEL     0x0
#define ERR_LEVEL      0x0
#endif

#define INFO_LOG  INFO_LEVEL, __FILE__, __func__, __LINE__
#define DEBUG_LOG DEBUG_LEVEL, __FILE__, __func__, __LINE__
#define WARN_LOG  WARN_LEVEL, __FILE__, __func__, __LINE__
#define ERR_LOG   ERR_LEVEL, __FILE__, __func__, __LINE__


char g_caMsg[1024];


enum ConfigFileEnum {
	CONFIGFILE__SERVER,
	CONFIGFILE__BITMAP,
	CONFIGFILE__TRANINFO,
	CONFIGFILE__SIZE
};

/* �������Ϣ */
struct ServerInfo {
	char ipaddr[16];
	int port;
	int timeout;
};
typedef struct ServerInfo SERVERINFO;

/* ����Ϣ */
struct FieldDesc {
	int flag; /* 0-�� 1-�Ϸ� 2-�Ƿ� */
	int fieldno; /* λԪ�� */
	int fieldattr; /* λԪ�䳤��־ 0-���� 1-1λ�䳤 2-2λ�䳤 */
	int fieldlen; /* λԪ���ȣ��䳤λԪΪ��󳤶� */
	int fieldtype; /* λԪ�������� */
	int offset; /* ƫ���� */
};
typedef struct FieldDesc FIELDDESC;

/* ������Ϣ */
struct TranDesc {
	int max_valuelen; /* ���valueָ�볤�� */
	int max_field; /* ��������� */
	int cur_valuelen; /* ��ǰvalue���� */
	int cur_field; /* ��ǰ��ĸ��� */
	FIELDDESC * fielddesc; /* ��ָ�� */
	char * value; /* ֵָ�� */
};
typedef struct TranDesc TRANDESC;

/* ���׶�Ӧλͼ��Ϣ */
struct TrancodeMap {
	char * caTranCode;
	char * BitMap;
};

/* �������ṹ */
struct RspServerConfig {
	int port;
	int timeout;
	char filename[128];
	char caMsg[BUFFER_SIZE];
};
typedef struct RspServerConfig RSPSERVERCONFIG;


#ifdef MAIN
pthread_mutex_t * g_sh_myLock = NULL;
int * g_sh_suCount = NULL;
#else
extern pthread_mutex_t * g_sh_myLock;
extern int * g_sh_suCount;
#endif


#endif