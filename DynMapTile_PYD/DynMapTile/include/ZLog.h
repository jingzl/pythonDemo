
#ifndef _ZLOG_H_
#define _ZLOG_H_

#include "DataType.h"

// 日志存储文件名(与动态库同一目录)
#define FILE_NAME_LOG   "CPCOOLMAP.log"      
// 日志总开关
#define LOG_ALL_ON           1
//#define FEATURE_LOG_ANDORID
const ZINT32 szModuleOn[21] = {
    1,    // TAG_NONE
	1,    // TAG_JNI
	1,    // TAG_TAT
	1,    // TAG_LCMAIN
	1,    // TAG_LCBROWSE
	1,    // TAG_LCNAVI
	1,    // TAG_LCQUERY
	1,    // TAG_LCNET
	1,    // TAG_AGG
	1,    // TAG_MAPDISPLAY
	1,    // TAG_MAPENGINE
	1,    // TAG_MAPQUERY
	1,    // TAG_MAPCONFIG
	1,    // TAG_CAMERA
	1,    // TAG_GPS
	1,    // TAG_TTS
	1,    // TAG_GUIDANCE
	1,    // TAG_ROUTEPLAN
	1,    // TAG_PLATFORM
	1,    // TAG_UTIL
	1,    // TAG_VALIDITY
};

// 日志模块的标识
typedef enum _tag_enum_module
{
//--------------------------------------------------------------------
	TAG_NONE = 0,
	TAG_JNI,
	TAG_TAT,

	TAG_LCMAIN = 3,
	TAG_LCBROWSE,
	TAG_LCNAVI,
	TAG_LCQUERY,
	TAG_LCNET,

	TAG_AGG = 8,
	TAG_MAPDISPLAY,
	TAG_MAPENGINE,
	TAG_MAPQUERY,
	TAG_MAPCONFIG,

	TAG_CAMERA = 13,
	TAG_GPS,
	TAG_TTS,
	TAG_GUIDANCE,
	TAG_ROUTEPLAN,

	TAG_PLATFORM = 18,
	TAG_UTIL,
	TAG_VALIDITY

}TAG_MODULE;

ZVOID ZLOGD(TAG_MODULE module, const ZCHAR *format, ...);
ZVOID ZLOGI(TAG_MODULE module, const ZCHAR *format, ...);
ZVOID ZLOGW(TAG_MODULE module, const ZCHAR *format, ...);
ZVOID ZLOGE(TAG_MODULE module, const ZCHAR *format, ...);

#endif	// end of _ZLOG_H_