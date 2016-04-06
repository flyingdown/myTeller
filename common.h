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

/* 服务端信息 */
struct ServerInfo {
	char ipaddr[16];
	int port;
	int timeout;
};
typedef struct ServerInfo SERVERINFO;

/* 域信息 */
struct FieldDesc {
	int flag; /* 0-空 1-合法 2-非法 */
	int fieldno; /* 位元号 */
	int fieldattr; /* 位元变长标志 0-定长 1-1位变长 2-2位变长 */
	int fieldlen; /* 位元长度，变长位元为最大长度 */
	int fieldtype; /* 位元数据类型 */
	int offset; /* 偏移量 */
};
typedef struct FieldDesc FIELDDESC;

/* 交易信息 */
struct TranDesc {
	int max_valuelen; /* 最大value指针长度 */
	int max_field; /* 最大的域个数 */
	int cur_valuelen; /* 当前value长度 */
	int cur_field; /* 当前域的个数 */
	FIELDDESC * fielddesc; /* 域指针 */
	char * value; /* 值指针 */
};
typedef struct TranDesc TRANDESC;

/* 交易对应位图信息 */
struct TrancodeMap {
	char * caTranCode;
	char * BitMap;
};

/* 挡板服务结构 */
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