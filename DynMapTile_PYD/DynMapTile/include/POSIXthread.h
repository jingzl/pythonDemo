
#pragma once
#include "DataType.h"
#include "ZLog.h"

#if defined(CP_OS_LINUX) || defined(CP_OS_CYGWIN)
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>
//#include <sys/types.h>
//#include <sys/stat.h>
#include <fcntl.h>
#else
#include <WTypes.h>
#include <WinBase.h>
#include <process.h>
#endif

class CCPMutex;
class CCPEvent;
class CCPWait;
class CCPSema;
class CCPCriticalSection;

class CCPThread 
{
public:

	CCPThread();
	~CCPThread();
    
	  /**
	  * �����߳�
	  * @param  [in]    pThreadAttri	    �߳�����    
	  * @param  [in]    dwStackSize         �̶߳�ջ��С    
	  * @param  [in]    pStartAddress       �̺߳�����ַ
	  * @param  [in]    pParam              �̲߳���
	  * @param  [in]    dwCreationFlags     �̱߳�ʶ
	  * @return ZINT32                      INVALID��ʾ����SUCCESS��ʾ�ɹ� 
	  */
    ZINT32 CreateThread(ZPVOID pThreadAttri, ZDWORD dwStackSize, PTHREAD_START
                              pStartAddress, ZPVOID pParam, ZDWORD dwCreationFlags);
                              
    /**
    * ��ѯ�߳����ȼ�
	* @return ZINT32                        �߳����ȼ�(Ŀǰֻ���Ƿ���Ԥ���峣�������������ȼ���ֵ)
	*/
    ZINT32 GetThreadPriority();
    
	/**
	* �����߳����ȼ�
	* @param  [in]    nPriority             �̵߳����ȼ�
	* @return ZBOOL                         �ɹ�����ZTRUE��ʧ�ܷ���ZFALSE
	*/
    ZBOOL SetThreadPriority(ZINT32 nPriority);

	/**
	* ��ȡ��ǰ�߳�ID
	* @return ZDWORD                        ��ǰ�߳�ID
	*/
	static ZDWORD GetCurThreadID();

	/**
	* �����߳�
	* @return ZDWORD                        INVALID��ʾ��������ֵ��ʾ�ɹ�           
	*/
    ZDWORD SuspendThread();
    
	/**
	* �ָ��߳�
	* @return ZDWORD                        INVALID��ʾ��������ֵ��ʾ�ɹ�
	*/
    ZDWORD ResumeThread();
    
	/**
	* �ȴ��߳���ֹ
	* @return ZVOID                       
	*/
    ZVOID JoinThread();

	/**
	* �߳�����
	* @param  [in]    iMillionSeconds       �̵߳�����ʱ��(��λ������)
	* @return ZVOID                       
	*/
	static ZVOID Sleep(ZUINT32 iMillionSeconds);
    
	/**
	* �����߳�
	* @return ZVOID                       
	*/
    ZVOID ExitThread();
    
private:

    ZHandle m_hThread;                      // �̵߳�Handle��Linux���̵߳�ID
	CCPEvent* m_pEvent;

#if defined(CP_OS_LINUX) || defined(CP_OS_CYGWIN)
	pthread_t m_tid;
#endif

};

class CCPEvent
{
public:
	CCPEvent();
	~CCPEvent();

	/**
	* ��ʼ��
	* @param  [in]    bManualReset        ���÷�ʽ(�˹�����1,�Զ�����0)
	* @param  [in]    bInitialState       �¼���ʼ�ź�̬
	* @param  [in]    pName               �¼�����
	* @return ZINT32                      �ɹ�����SUCCESS��ʧ�ܷ���INVALID                       
	*/
	ZINT32 Init(ZBOOL bManualReset, ZBOOL bInitialState, ZLPCTSTR pName);

	/**
	* ���¼��õ��ź�̬
	* @return ZINT32                      �ɹ�����SUCCESS��ʧ�ܷ���INVALID                         
	*/
	ZINT32 SetEvent();
 
	/**
	* ���¼��õ����ź�̬
	* @return ZINT32                      �ɹ�����SUCCESS��ʧ�ܷ���INVALID                        
	*/
	ZINT32 ResetEvent();

	/**
	* ��ȡ�¼�������
	* @return CCPMutex*                   �¼�������ָ��                     
	*/
	CCPMutex* GetEventMutex(){return m_pMutex;};

	/**
	* ��ȡ�¼����
	* @return ZHandle                     �¼����                 
	*/
	ZHandle GetHandle(){return m_hHandle;};

	/**
	* ��ȡ�źż���״̬
	* @return ZBOOL                       �źż���״̬                                  
	*/
	ZBOOL  GetSignalState(){ return m_bSignal;};

	/**
	* �����źż���״̬
	* @param  [in]    bSignal             �¼��ź�̬����״̬
	* @return ZVOID                                                 
	*/
	ZVOID  SetSignalState(ZBOOL bSignal){m_bSignal = bSignal;};

	/**
	* �ж��Ƿ��Զ�����
	* @return ZBOOL                       ZTURE��ʾ�Զ����ã�ZFALSE��ʾ�˹�����                                 
	*/
	ZBOOL  IsAutoReset(){return m_bAutoReset;};

private:

	ZBOOL IsValid();
	ZVOID Close();

private:

    CCPMutex* m_pMutex;                  // �����������Ļ���������Ϊ�����ں����ڲ��Դ���
	ZHandle   m_hHandle;                 // �¼����
	ZBOOL     m_bAutoReset;              // ��ʶ�Ƿ��Զ����ã�Ĭ��ΪZTURE
	ZBOOL     m_bSignal;                 // ���������Ƿ񼤻�������¼�����ʱ����ֵΪZTRUE�����ڵȴ��¼�ʱѭ�����ȴ�����

#if defined(CP_OS_LINUX) || defined(CP_OS_CYGWIN)
	pthread_cond_t m_cond;
	pthread_condattr_t m_cattr;
#endif

};


class CCPWait
{
public:
    CCPWait();
	~CCPWait();
 
	/**
	* �������ȴ��¼�
	* @param  [in]    pEvent              �ȴ��Ķ���ָ��
	* @return ZINT32                      �ɹ�����ZWAIT_OBJECT_0��ʧ�ܷ���ZWAIT_FAILED                       
	*/
	static ZINT32 WaitEvent(CCPEvent* pEvent);

	/**
	* ʱ��ȴ��¼�
	* @param  [in]    pEvent              �ȴ��Ķ���ָ��
	* @param  [in]    dwMilliSeconds      �ȴ��ĳ�ʱʱ��(��λ:����)
	* @return ZINT32                      �ɹ�����ZWAIT_OBJECT_0��ʧ�ܷ���ZWAIT_FAILED                       
	*/
	static ZINT32 TimeWaitEvent(CCPEvent* pEvent, ZDWORD dwMilliSeconds);

};

class CCPSema
{
public:

	CCPSema();
	~CCPSema();

	/**
	* ��ʼ��
	* @param  [in]    nInitialCount       �ź�����ʼ����ֵ����ֵ������ڻ����0
	* @param  [in]    nMaxCount           �ź��������������ֵ����ֵ�������0
	* @param  [in]    pName               �ź�������
	* @return ZINT32                      �ɹ�����SUCCESS��ʧ�ܷ���INVALID                       
	*/
	ZINT32 Init(ZUINT32 nInitialCount, ZUINT32 nMaxCount, ZLPCTSTR pName);

	/**
	* �����ź���
	* @return ZINT32                      �ɹ�����SUCCESS��ʧ�ܷ���INVALID                       
	*/
	ZINT32 Lock();

	/**
	* ���������ź���
	* @param  [in]    dwMilliseconds      ����������ʱ��
	* @return ZINT32                      �ɹ�����SUCCESS��ʧ�ܷ���INVALID                       
	*/
	ZINT32 TryLock(ZDWORD dwMilliseconds);

	/**
	* �ͷ��ź���
	* @return ZINT32                      �ɹ�����SUCCESS��ʧ�ܷ���INVALID                       
	*/
	ZINT32 UnLock();

private:

	ZBOOL IsValid();
	ZVOID Close();

private:

	ZHandle m_hHandle;

#if defined(CP_OS_LINUX) || defined(CP_OS_CYGWIN)
	sem_t m_sema;
#endif

};

class CCPMutex
{
public:

	CCPMutex();
	~CCPMutex();

	/**
	* ��ʼ��
	* @param  [in]    bInitalOwner        ��ʶָ�������߳�ʱ�Ƿ�����ӵ�иû���������Ȩ
	* @param  [in]    pName               ����������
	* @return ZINT32                      �ɹ�����SUCCESS��ʧ�ܷ���INVALID                       
	*/
	ZINT32 Init(ZBOOL bInitalOwner, ZLPCTSTR pName);

	/**
	* ����������
	* @return ZINT32                      �ɹ�����SUCCESS��ʧ�ܷ���INVALID                       
	*/
	ZINT32 Lock();

	/**
	* ��������������
	* @param  [in]    dwMilliseconds      ����������ʱ��
	* @return ZINT32                      �ɹ�����SUCCESS��ʧ�ܷ���INVALID                       
	*/
	ZINT32 TryLock(ZDWORD dwMilliseconds);

	/**
	* �������������
	* @return ZINT32                      �ɹ�����SUCCESS��ʧ�ܷ���INVALID                       
	*/
	ZINT32 UnLock();

	/**
	* ��ȡ���������
	* @return ZHandle                     ���������                 
	*/
	ZHandle GetHandle(){return m_hHandle;};

private:

	ZBOOL IsValid();
	ZVOID Close();

private:

	ZHandle m_hHandle;

#if defined(CP_OS_LINUX) || defined(CP_OS_CYGWIN)
	pthread_mutexattr_t m_mattr;
	pthread_mutex_t     m_mutex;
#endif

};

class CCPCriticalSection
{
public:
	CCPCriticalSection();
	~CCPCriticalSection();

	/**
	* ��ʼ��
	* @return ZINT32                      �ɹ�����SUCCESS��ʧ�ܷ���INVALID                       
	*/
	ZINT32 Init();

	/**
	* �����ٽ���
	* @return ZINT32                      �ɹ�����SUCCESS��ʧ�ܷ���INVALID                       
	*/
	ZINT32 Lock();

	/**
	* ���������ٽ���
	* @return ZINT32                      �ɹ�����SUCCESS��ʧ�ܷ���INVALID                       
	*/
	ZINT32 TryLock();

	/**
	* �˳��ٽ���
	* @return ZINT32                      �ɹ�����SUCCESS��ʧ�ܷ���INVALID                       
	*/
	ZINT32 UnLock();

private:

	ZVOID Close();

private:

	ZBOOL m_bInit;

#if defined(CP_OS_LINUX) || defined(CP_OS_CYGWIN)
	CCPMutex m_cs;
#else
	CRITICAL_SECTION m_cs;
#endif
   
};
