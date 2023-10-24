
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

// �����ļ��������
typedef FILE*			ZFileHandle;

#if defined(CP_OS_LINUX) || defined(CP_OS_CYGWIN)
	#define PATH_SEPARATOR		('/')			// ·���ָ���
	#define PATH_SEPARATOR_A	("/")
	#define PATH_SEPARATOR_W	(L"/")
#else
	#define PATH_SEPARATOR		('\\')			// ·���ָ���
	#define PATH_SEPARATOR_A	("\\")
	#define PATH_SEPARATOR_W	(L"\\")
#endif

#define FILECOPY_SIZE		(1024*1024)		// �����ļ�һ�ζ�д��С��1M	
#define MAX_PATH_LEN		260				// �ļ�����󳤶ȣ�����windows�Ķ���

// �����ļ�seekģʽ
#ifndef SEEK_SET
#define SEEK_SET		0		// �ļ���ʼ
#endif
#ifndef SEEK_CUR
#define SEEK_CUR		1		// ��ǰλ��
#endif
#ifndef SEEK_END
#define SEEK_END		2		// �ļ���β
#endif

// �ļ�������
enum eFileError
{
	FILEERROR_NONE,				// ����
	FILEERROR_FILENOTFOUND,		// �Ҳ����ļ�
	FILEERROR_ACCESSDENIED,		// û���ļ�����Ȩ��
	FILEERROR_BADSEEK,			// ��λʧ��
	FILEERROR_DISKFULL,			// ��������
	FILEERROR_ENDOFFILE,		// �Ѿ����ļ�β
	FILEERROR_READFAIL,			// ��ʧ��
	FILEERROR_WRITEFAIL,		// дʧ��
	FILEERROR_NOBUFFER			// ��д����Ϊ��
};

// �ļ��򿪷�ʽ
#define FO_READONLY					1		// Opens for reading. If the file does not exist or cannot be found, the fopen call fails.
#define FO_CREATE_WRITE				2		// Opens an empty file for writing. If the given file exists, its contents are destroyed.
#define FO_APPEND					3		// Opens for writing at the end of the file (appending) without removing the EOF marker before writing new data to the file; creates the file first if it doesn't exist.
#define FO_EXIST_READWRITE			4		// Opens for both reading and writing. (The file must exist.)
#define FO_CREATE_READWRITE			5		// Opens an empty file for both reading and writing. If the given file exists, its contents are destroyed.
#define FO_READWRITE				6		// Opens for reading and writing; reates the file first if it doesn't exist.

// �ļ��򿪱�ʶ
#ifndef FILE_ATTRIBUTE_NORMAL
#define FILE_ATTRIBUTE_NORMAL           0x00000080	// Ĭ������
#endif
#ifndef FILE_FLAG_WRITE_THROUGH
#define FILE_FLAG_WRITE_THROUGH         0x80000000	// ����ϵͳ�����Ƴٶ��ļ���д����
#endif
#ifndef FILE_FLAG_OVERLAPPED
#define FILE_FLAG_OVERLAPPED            0x40000000	// ������ļ������ص�����
#endif
#ifndef FILE_FLAG_NO_BUFFERING
#define FILE_FLAG_NO_BUFFERING          0x20000000
#endif
#ifndef FILE_FLAG_RANDOM_ACCESS
#define FILE_FLAG_RANDOM_ACCESS         0x10000000	// ���������ʶ��ļ���������Ż�
#endif
#ifndef FILE_FLAG_SEQUENTIAL_SCAN
#define FILE_FLAG_SEQUENTIAL_SCAN       0x08000000	// ����������ʶ��ļ���������Ż�
#endif

// �ļ�����
#define FILEATTR_EXECUTE		0x0001		// �����У�window�ϲ�������
#define FILEATTR_READONLY		0x0002		// ֻ��
#define FILEATTR_HIDDEN			0x0003		// ����
#define FILEATTR_NORMAL			0x0007		// �������ɶ�д

// ��ͼȨ��
#ifndef FILE_MAP_COPY
#define FILE_MAP_COPY       0x0001		// ����дȨ��
#endif
#ifndef FILE_MAP_WRITE
#define FILE_MAP_WRITE      0x0002		// ��дȨ��
#endif
#ifndef FILE_MAP_READ
#define FILE_MAP_READ       0x0004		// ֻ��Ȩ��
#endif
#ifndef FILE_MAP_ALL_ACCESS
#define FILE_MAP_ALL_ACCESS 0x000f001f	// ����Ȩ��
#endif
#ifndef FILE_MAP_EXECUTE
#define FILE_MAP_EXECUTE    0x0020 		// ����Ȩ��
#endif

// �ļ�ӳ��ĺ궨��
#if defined(CP_OS_LINUX) || defined(CP_OS_CYGWIN)

// �ļ�ӳ��ķ���Ȩ��
#undef PAGE_NOACCESS
#define PAGE_NOACCESS		PROT_NONE				// ��Ȩ��

#undef PAGE_READONLY
#define PAGE_READONLY		PROT_READ				// ֻ��

#undef PAGE_READWRITE
#define PAGE_READWRITE		PROT_READ|PROT_WRITE	// ��д

#undef PAGE_EXECUTE
#define PAGE_EXECUTE		PROT_EXEC				// ����

#endif

// �ļ�������
class CCPFile
{
public:
	/**
	* ���캯��
	*/
	CCPFile();

	/**
	* ��������
	*/
	~CCPFile();
	
	/**
	* ���ļ�
	* @param  [in]	lpFileName	�ļ���
	* @param  [in]	dwMode		�򿪷�ʽ
	* @param  [in]	dwFlag		�򿪱�ʶ
	* @param  [in]	pDefault	����brewƽ̨��shellָ�룬wince��linux��ֵΪ��
	* @return ZBOOL				�Ƿ�ɹ����ļ�
	*/
	ZBOOL Open(ZLPCSTR lpFileName, ZDWORD dwMode, ZDWORD dwFlag, ZPVOID pDefault = NULL);

	/**
	* ���ļ�
	* @param  [in]	pBuffer			������
	* @param  [in]	dwElementSize	��λ��С
	* @param  [in]	dwCount			��λ����
	* @return ZDWORD				ʵ�ʶ��Ĵ�С
	*/
	ZDWORD Read(ZPVOID pBuffer, ZDWORD dwElementSize, ZDWORD dwCount);

	/**
	* д�ļ�
	* @param  [in]	pBuffer				д����
	* @param  [in]	dwElementSize		��λ��С
	* @param  [in]	dwCount				��λ����
	* @return ZDWORD					ʵ��д�Ĵ�С
	*/
	ZDWORD Write(ZPVOID pBuffer, ZDWORD dwElementSize, ZDWORD dwCount);

	/**
	* ��λ
	* @param  [in]	lOff	ƫ��ֵ
	* @param  [in]	nFrom	��λ��ʽ
	* @return ZBOOL			�Ƿ�λ�ɹ�
	*/
	ZBOOL Seek(ZLONGLONG lOff, ZUINT32 nFrom);

	/**
	* ��λ���ļ�β
	* @return ZBOOL	�Ƿ�λ�ɹ�
	*/
	ZBOOL SeekToEnd();

	/**
	* ��λ���ļ�ͷ
	* @return ZBOOL	�Ƿ�λ�ɹ�
	*/
	ZBOOL SeekToBegin();

	/**
	* ��ȡ��ǰλ��
	* @return ZLONGLONG	��ǰ�ļ�ƫ��ֵ
	*/
	ZLONGLONG GetPosition() const;

	/**
	* �����ļ�����
	* @param  [in]	dwNewLen	�ļ�����
	* @return ZBOOL				�Ƿ����óɹ�
	*/
	ZBOOL SetLength(ZULONGLONG dwNewLen);

	/**
	* ��ȡ�ļ�����
	* @return ZLONGLONG	�ļ�����
	*/
	ZLONGLONG GetLength() const; 

	/**
	* �ر��ļ�
	* @return ZBOOL	�Ƿ�رճɹ�
	*/
	ZBOOL Close();

	/**
	* ��ȡ�ļ���
	* @return ZLPCSTR	�ļ���
	*/
	ZLPCSTR  GetFileName() const;

	/**
	* ��ȡ�ļ�������
	* @return ZUINT32	�ļ�������
	*/
	ZUINT32 GetFileError() const
	{
		return m_iFileError;
	}

private:

	/**
	* �����ļ�������
	* @param  [in]	iError	�ļ�������
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

// �ļ�������
class CCPFileMgr
{
public:
	CCPFileMgr();
	~CCPFileMgr();

	/**
	* ����Ŀ¼
	* @param  [in]	lpPathName	Ŀ¼��
	* @return ZINT32			�ɹ�����0��ʧ�ܷ���1
	*/
	static ZINT32 CreateDirectory(ZLPCSTR lpPathName);

	/**
	* ɾ��Ŀ¼
	* @param  [in]	lpPathName	Ŀ¼��
	* @return ZINT32			�ɹ�����0��ʧ�ܷ���1
	*/
	static ZINT32 RemoveDirectory(ZLPCSTR lpPathName);

	/**
	* ȡ�õ�ǰ����Ŀ¼
	* @param  [in,out]	pBufDir	Ŀ¼��
	* @param  [in]		pBufDir Ŀ¼���ַ�������
	* @return ZINT32			�ɹ�����0��ʧ�ܷ���1
	*/
	static ZINT32 GetCurrentDiretory(ZCHAR* pBufDir, ZINT32 iLen);

	/**
	* �����ļ�Ȩ��
	* @param  [in]	lpFileName			�ļ���
	* @param  [in]	dwFileAttributes	windows�ļ�Ȩ�ޣ�linux�ļ��û�Ȩ��
	* @param  [in]	dwGroupAttri		linux�ļ����û�Ȩ�ޣ�Ĭ��:��/д/����
	* @param  [in]	dwOtherAttri		linux�ļ������û�Ȩ�ޣ�Ĭ��:����
	* @return ZINT32					�ɹ�����0��ʧ�ܷ���1
	*/
	static ZINT32 SetFileAttribute(ZLPCSTR lpFileName, ZDWORD dwFileAttributes, 
		ZDWORD dwGroupAttri = FILEATTR_NORMAL, ZDWORD dwOtherAttri = FILEATTR_NORMAL);

	/**
	* �����ļ�
	* @param  [in]	lpExistingFileName	Դ�ļ���
	* @param  [in]	lpNewFileName		Ŀ���ļ���
	* @param  [in]	bFailIfExists		true - �ļ������򷵻�ʧ�ܣ�false - �ļ������򸲸�
	* @return ZINT32					�ɹ�����0��ʧ�ܷ���1
	*/
	static ZINT32 CopyFile(ZLPCSTR lpExistingFileName, ZLPCSTR lpNewFileName,ZBOOL bFailIfExists);

	/**
	* �ƶ��ļ�
	* @param  [in]	lpExistingFileName	Դ�ļ���
	* @param  [in]	lpNewFileName		Ŀ���ļ���
	* @return ZINT32					�ɹ�����0��ʧ�ܷ���1
	*/
	static ZINT32 MoveFile(ZLPCSTR lpExistingFileName, ZLPCSTR lpNewFileName);

	/**
	* ɾ���ļ�
	* @param  [in]	lpFileName	�ļ���
	* @return ZINT32			�ɹ�����0��ʧ�ܷ���1
	*/
	static ZINT32 Remove(ZLPCSTR lpFileName);
};

// �ļ�ӳ����
class CCPMapFile
{
public:
	/**
	* ���캯��
	*/
	CCPMapFile();

	/**
	* ��������
	*/
	~CCPMapFile();

	/**
	* ���ļ�
	* @param  [in]	lpFileName	�ļ���
	* @param  [in]	dwMode		�򿪷�ʽ
	* @param  [in]	dwFlag		�򿪱�ʶ
	* @param  [in]	pDefault	����brewƽ̨��shellָ�룬wince��linux��ֵΪ��
	* @return ZBOOL				�Ƿ�ɹ����ļ�
	*/
	ZBOOL Open(ZLPCSTR lpFileName, ZDWORD dwMode, ZDWORD dwFlag, ZPVOID pDefault = NULL);

	/**
	* ���ϵͳ��ҳ��С��ϵͳ�����ȣ�
	* @return ZDWORD	��ҳ��С
	*/
	ZDWORD GetPageSize();

	/**
	* �����ļ�ӳ���ں˶���
	* @param  [in]	flProtect			�ļ�ӳ��Ȩ��
	* @param  [in]	dwMaximumSizeHigh	�ļ�ӳ���С�ĸ�λ
	* @param  [in]	dwMaximumSizeLow	�ļ�ӳ���С�ĵ�λ
	* @param  [in]	lpName				ӳ����
	* @return ZBOOL						�Ƿ�ӳ��ɹ�
	*/
	ZBOOL CreateFileMapping(ZDWORD flProtect, ZDWORD dwMaximumSizeHigh, ZDWORD dwMaximumSizeLow, ZLPCWSTR lpName);

	/**
	* ������ͼ
	* @param  [in]	dwDesiredAccess			��ͼȨ��
	* @param  [in]	dwFileOffsetHigh		��ͼ��ʼλ�õĸ�λ
	* @param  [in]	dwFileOffsetLow			��ͼ��ʼλ�õĵ�λ
	* @param  [in]	dwNumberOfBytesToMap	��ͼ��С
	* @return ZPVOID						��ͼ���
	*/
	ZPVOID MapViewOfFile(ZDWORD dwDesiredAccess, ZDWORD dwFileOffsetHigh, ZDWORD dwFileOffsetLow, ZDWORD dwNumberOfBytesToMap);

	/**
	* �����ͼ
	* @return ZBOOL	�Ƿ����ɹ�
	*/
	ZBOOL UnmapViewOfFile();

	/**
	* ��ȡ�ļ�����
	* @return ZLONGLONG	�ļ�����
	*/
	ZLONGLONG GetFileLength() const;

	/**
	* �ر��ļ�ӳ��
	* @return ZBOOL	�Ƿ�رճɹ�
	*/
	ZBOOL Close();

	/**
	* ��ȡ�ļ�ӳ�������
	* @return ZUINT32	�ļ�������
	*/
	ZUINT32 GetFileMapError() const
	{
		return m_iFileMapError;
	}

private:

	/**
	* �����ļ�ӳ�������
	* @param  [in]	iError	�ļ�������
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