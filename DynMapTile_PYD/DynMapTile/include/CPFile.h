
#ifndef _CPFILE_H_
#define _CPFILE_H_

#include "DataType.h"
#include "CPString.h"

#include <stdio.h>
#include <fcntl.h>

#if defined(CP_OS_CYGWIN)
#include <unistd.h> 
#include <sys/unistd.h>
#endif

#if defined(CP_OS_LINUX)
#include <sys/sysconf.h>
#endif

#if defined(CP_OS_LINUX) || defined(CP_OS_CYGWIN)
#include <sys/mman.h>
#else
#include <io.h>
#include <windows.h>
#include <windef.h>
#include <winnt.h>
#include <basetsd.h>
#include <winbase.h>
#endif

// 定义文件句柄类型
typedef FILE*			ZFileHandle;

#if defined(CP_OS_LINUX) || defined(CP_OS_CYGWIN)
	#define PATH_SEPARATOR		('/')			// 路径分隔符
	#define PATH_SEPARATOR_A	("/")
	#define PATH_SEPARATOR_W	(L"/")
#else
	#define PATH_SEPARATOR		('\\')			// 路径分隔符
	#define PATH_SEPARATOR_A	("\\")
	#define PATH_SEPARATOR_W	(L"\\")
#endif

#define FILECOPY_SIZE		(1024*1024)		// 复制文件一次读写大小，1M	
#define MAX_PATH_LEN		260				// 文件名最大长度，依据windows的定义

// 定义文件seek模式
#ifndef SEEK_SET
#define SEEK_SET		0		// 文件开始
#endif
#ifndef SEEK_CUR
#define SEEK_CUR		1		// 当前位置
#endif
#ifndef SEEK_END
#define SEEK_END		2		// 文件结尾
#endif

// 文件错误码
enum eFileError
{
	FILEERROR_NONE,				// 正常
	FILEERROR_FILENOTFOUND,		// 找不到文件
	FILEERROR_ACCESSDENIED,		// 没有文件操作权限
	FILEERROR_BADSEEK,			// 定位失败
	FILEERROR_DISKFULL,			// 磁盘已满
	FILEERROR_ENDOFFILE,		// 已经到文件尾
	FILEERROR_READFAIL,			// 读失败
	FILEERROR_WRITEFAIL,		// 写失败
	FILEERROR_NOBUFFER			// 读写缓存为空
};

// 文件打开方式
#define FO_READONLY					1		// Opens for reading. If the file does not exist or cannot be found, the fopen call fails.
#define FO_CREATE_WRITE				2		// Opens an empty file for writing. If the given file exists, its contents are destroyed.
#define FO_APPEND					3		// Opens for writing at the end of the file (appending) without removing the EOF marker before writing new data to the file; creates the file first if it doesn't exist.
#define FO_EXIST_READWRITE			4		// Opens for both reading and writing. (The file must exist.)
#define FO_CREATE_READWRITE			5		// Opens an empty file for both reading and writing. If the given file exists, its contents are destroyed.
#define FO_READWRITE				6		// Opens for reading and writing; reates the file first if it doesn't exist.

// 文件打开标识
#ifndef FILE_ATTRIBUTE_NORMAL
#define FILE_ATTRIBUTE_NORMAL           0x00000080	// 默认属性
#endif
#ifndef FILE_FLAG_WRITE_THROUGH
#define FILE_FLAG_WRITE_THROUGH         0x80000000	// 操作系统不得推迟对文件的写操作
#endif
#ifndef FILE_FLAG_OVERLAPPED
#define FILE_FLAG_OVERLAPPED            0x40000000	// 允许对文件进行重叠操作
#endif
#ifndef FILE_FLAG_NO_BUFFERING
#define FILE_FLAG_NO_BUFFERING          0x20000000
#endif
#ifndef FILE_FLAG_RANDOM_ACCESS
#define FILE_FLAG_RANDOM_ACCESS         0x10000000	// 针对随机访问对文件缓冲进行优化
#endif
#ifndef FILE_FLAG_SEQUENTIAL_SCAN
#define FILE_FLAG_SEQUENTIAL_SCAN       0x08000000	// 针对连续访问对文件缓冲进行优化
#endif

// 文件属性
#define FILEATTR_EXECUTE		0x0001		// 可运行，window上不起作用
#define FILEATTR_READONLY		0x0002		// 只读
#define FILEATTR_HIDDEN			0x0003		// 隐藏
#define FILEATTR_NORMAL			0x0007		// 正常，可读写

// 视图权限
#ifndef FILE_MAP_COPY
#define FILE_MAP_COPY       0x0001		// 复制写权限
#endif
#ifndef FILE_MAP_WRITE
#define FILE_MAP_WRITE      0x0002		// 读写权限
#endif
#ifndef FILE_MAP_READ
#define FILE_MAP_READ       0x0004		// 只读权限
#endif
#ifndef FILE_MAP_ALL_ACCESS
#define FILE_MAP_ALL_ACCESS 0x000f001f	// 所有权限
#endif
#ifndef FILE_MAP_EXECUTE
#define FILE_MAP_EXECUTE    0x0020 		// 运行权限
#endif

// 文件映射的宏定义
#if defined(CP_OS_LINUX) || defined(CP_OS_CYGWIN)

// 文件映射的访问权限
#undef PAGE_NOACCESS
#define PAGE_NOACCESS		PROT_NONE				// 无权限

#undef PAGE_READONLY
#define PAGE_READONLY		PROT_READ				// 只读

#undef PAGE_READWRITE
#define PAGE_READWRITE		PROT_READ|PROT_WRITE	// 读写

#undef PAGE_EXECUTE
#define PAGE_EXECUTE		PROT_EXEC				// 运行

#endif

// 文件操作类
class CCPFile
{
public:
	/**
	* 构造函数
	*/
	CCPFile();

	/**
	* 析构函数
	*/
	~CCPFile();
	
	/**
	* 打开文件
	* @param  [in]	lpFileName	文件名
	* @param  [in]	dwMode		打开方式
	* @param  [in]	dwFlag		打开标识
	* @param  [in]	pDefault	用于brew平台传shell指针，wince与linux该值为空
	* @return ZBOOL				是否成功打开文件
	*/
	ZBOOL Open(ZLPCSTR lpFileName, ZDWORD dwMode, ZDWORD dwFlag, ZPVOID pDefault = NULL);

	/**
	* 读文件
	* @param  [in]	pBuffer			读缓存
	* @param  [in]	dwElementSize	单位大小
	* @param  [in]	dwCount			单位个数
	* @return ZDWORD				实际读的大小
	*/
	ZDWORD Read(ZPVOID pBuffer, ZDWORD dwElementSize, ZDWORD dwCount);

	/**
	* 写文件
	* @param  [in]	pBuffer				写缓存
	* @param  [in]	dwElementSize		单位大小
	* @param  [in]	dwCount				单位个数
	* @return ZDWORD					实际写的大小
	*/
	ZDWORD Write(ZPVOID pBuffer, ZDWORD dwElementSize, ZDWORD dwCount);

	/**
	* 定位
	* @param  [in]	lOff	偏移值
	* @param  [in]	nFrom	定位方式
	* @return ZBOOL			是否定位成功
	*/
	ZBOOL Seek(ZLONGLONG lOff, ZUINT32 nFrom);

	/**
	* 定位到文件尾
	* @return ZBOOL	是否定位成功
	*/
	ZBOOL SeekToEnd();

	/**
	* 定位到文件头
	* @return ZBOOL	是否定位成功
	*/
	ZBOOL SeekToBegin();

	/**
	* 获取当前位置
	* @return ZLONGLONG	当前文件偏移值
	*/
	ZLONGLONG GetPosition() const;

	/**
	* 设置文件长度
	* @param  [in]	dwNewLen	文件长度
	* @return ZBOOL				是否设置成功
	*/
	ZBOOL SetLength(ZULONGLONG dwNewLen);

	/**
	* 获取文件长度
	* @return ZLONGLONG	文件长度
	*/
	ZLONGLONG GetLength() const; 

	/**
	* 关闭文件
	* @return ZBOOL	是否关闭成功
	*/
	ZBOOL Close();

	/**
	* 获取文件名
	* @return ZLPCSTR	文件名
	*/
	ZLPCSTR  GetFileName() const;

	/**
	* 获取文件错误码
	* @return ZUINT32	文件错误码
	*/
	ZUINT32 GetFileError() const
	{
		return m_iFileError;
	}

private:

	/**
	* 设置文件错误码
	* @param  [in]	iError	文件错误码
	*/
	ZVOID SetFileError(ZUINT32 iError)
	{
		m_iFileError = iError;
	}

	ZINT32			m_hFile;
	ZHandle			m_hWinFile;

	CCPString		m_pcsFileName;
	ZUINT32			m_iFileError;
};

// 文件管理类
class CCPFileMgr
{
public:
	CCPFileMgr();
	~CCPFileMgr();

	/**
	* 创建目录
	* @param  [in]	lpPathName	目录名
	* @return ZINT32			成功返回0，失败返回1
	*/
	static ZINT32 CreateDirectory(ZLPCSTR lpPathName);

	/**
	* 删除目录
	* @param  [in]	lpPathName	目录名
	* @return ZINT32			成功返回0，失败返回1
	*/
	static ZINT32 RemoveDirectory(ZLPCSTR lpPathName);

	/**
	* 取得当前工作目录
	* @param  [in,out]	pBufDir	目录名
	* @param  [in]		pBufDir 目录名字符串长度
	* @return ZINT32			成功返回0，失败返回1
	*/
	static ZINT32 GetCurrentDiretory(ZCHAR* pBufDir, ZINT32 iLen);

	/**
	* 设置文件权限
	* @param  [in]	lpFileName			文件名
	* @param  [in]	dwFileAttributes	windows文件权限；linux文件用户权限
	* @param  [in]	dwGroupAttri		linux文件组用户权限，默认:读/写/运行
	* @param  [in]	dwOtherAttri		linux文件其他用户权限，默认:运行
	* @return ZINT32					成功返回0，失败返回1
	*/
	static ZINT32 SetFileAttribute(ZLPCSTR lpFileName, ZDWORD dwFileAttributes, 
		ZDWORD dwGroupAttri = FILEATTR_NORMAL, ZDWORD dwOtherAttri = FILEATTR_NORMAL);

	/**
	* 复制文件
	* @param  [in]	lpExistingFileName	源文件名
	* @param  [in]	lpNewFileName		目的文件名
	* @param  [in]	bFailIfExists		true - 文件存在则返回失败；false - 文件存在则覆盖
	* @return ZINT32					成功返回0，失败返回1
	*/
	static ZINT32 CopyFile(ZLPCSTR lpExistingFileName, ZLPCSTR lpNewFileName,ZBOOL bFailIfExists);

	/**
	* 移动文件
	* @param  [in]	lpExistingFileName	源文件名
	* @param  [in]	lpNewFileName		目的文件名
	* @return ZINT32					成功返回0，失败返回1
	*/
	static ZINT32 MoveFile(ZLPCSTR lpExistingFileName, ZLPCSTR lpNewFileName);

	/**
	* 删除文件
	* @param  [in]	lpFileName	文件名
	* @return ZINT32			成功返回0，失败返回1
	*/
	static ZINT32 Remove(ZLPCSTR lpFileName);
};

// 文件映射类
class CCPMapFile
{
public:
	/**
	* 构造函数
	*/
	CCPMapFile();

	/**
	* 析构函数
	*/
	~CCPMapFile();

	/**
	* 打开文件
	* @param  [in]	lpFileName	文件名
	* @param  [in]	dwMode		打开方式
	* @param  [in]	dwFlag		打开标识
	* @param  [in]	pDefault	用于brew平台传shell指针，wince与linux该值为空
	* @return ZBOOL				是否成功打开文件
	*/
	ZBOOL Open(ZLPCSTR lpFileName, ZDWORD dwMode, ZDWORD dwFlag, ZPVOID pDefault = NULL);

	/**
	* 获得系统分页大小（系统颗粒度）
	* @return ZDWORD	分页大小
	*/
	ZDWORD GetPageSize();

	/**
	* 创建文件映射内核对象
	* @param  [in]	flProtect			文件映射权限
	* @param  [in]	dwMaximumSizeHigh	文件映射大小的高位
	* @param  [in]	dwMaximumSizeLow	文件映射大小的低位
	* @param  [in]	lpName				映射名
	* @return ZBOOL						是否映射成功
	*/
	ZBOOL CreateFileMapping(ZDWORD flProtect, ZDWORD dwMaximumSizeHigh, ZDWORD dwMaximumSizeLow, ZLPCWSTR lpName);

	/**
	* 创建视图
	* @param  [in]	dwDesiredAccess			视图权限
	* @param  [in]	dwFileOffsetHigh		视图起始位置的高位
	* @param  [in]	dwFileOffsetLow			视图起始位置的低位
	* @param  [in]	dwNumberOfBytesToMap	视图大小
	* @return ZPVOID						视图句柄
	*/
	ZPVOID MapViewOfFile(ZDWORD dwDesiredAccess, ZDWORD dwFileOffsetHigh, ZDWORD dwFileOffsetLow, ZDWORD dwNumberOfBytesToMap);

	/**
	* 解除视图
	* @return ZBOOL	是否解除成功
	*/
	ZBOOL UnmapViewOfFile();

	/**
	* 获取文件长度
	* @return ZLONGLONG	文件长度
	*/
	ZLONGLONG GetFileLength() const;

	/**
	* 关闭文件映射
	* @return ZBOOL	是否关闭成功
	*/
	ZBOOL Close();

	/**
	* 获取文件映射错误码
	* @return ZUINT32	文件错误码
	*/
	ZUINT32 GetFileMapError() const
	{
		return m_iFileMapError;
	}

private:

	/**
	* 设置文件映射错误码
	* @param  [in]	iError	文件错误码
	*/
	ZVOID SetFileMapError(ZUINT32 iError)
	{
		m_iFileMapError = iError;
	}

	ZINT32			m_hFile;
	ZHandle			m_hWinFile;

	ZHandle			m_hFileMapping;
	ZBYTE*			m_pbFile;

	ZDWORD			m_flProtect;

	ZUINT64			m_iMappingSize;
	ZUINT32			m_iFileMapError;
};


#endif // end of _CPFILE_H_