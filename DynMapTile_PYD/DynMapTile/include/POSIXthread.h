
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
	  * 创建线程
	  * @param  [in]    pThreadAttri	    线程属性    
	  * @param  [in]    dwStackSize         线程堆栈大小    
	  * @param  [in]    pStartAddress       线程函数地址
	  * @param  [in]    pParam              线程参数
	  * @param  [in]    dwCreationFlags     线程标识
	  * @return ZINT32                      INVALID表示出错，SUCCESS表示成功 
	  */
    ZINT32 CreateThread(ZPVOID pThreadAttri, ZDWORD dwStackSize, PTHREAD_START
                              pStartAddress, ZPVOID pParam, ZDWORD dwCreationFlags);
                              
    /**
    * 查询线程优先级
	* @return ZINT32                        线程优先级(目前只考虑返回预定义常数，不返回优先级数值)
	*/
    ZINT32 GetThreadPriority();
    
	/**
	* 设置线程优先级
	* @param  [in]    nPriority             线程的优先级
	* @return ZBOOL                         成功返回ZTRUE，失败返回ZFALSE
	*/
    ZBOOL SetThreadPriority(ZINT32 nPriority);

	/**
	* 获取当前线程ID
	* @return ZDWORD                        当前线程ID
	*/
	static ZDWORD GetCurThreadID();

	/**
	* 挂起线程
	* @return ZDWORD                        INVALID表示出错，其它值表示成功           
	*/
    ZDWORD SuspendThread();
    
	/**
	* 恢复线程
	* @return ZDWORD                        INVALID表示出错，其它值表示成功
	*/
    ZDWORD ResumeThread();
    
	/**
	* 等待线程终止
	* @return ZVOID                       
	*/
    ZVOID JoinThread();

	/**
	* 线程休眠
	* @param  [in]    iMillionSeconds       线程的休眠时间(单位：毫秒)
	* @return ZVOID                       
	*/
	static ZVOID Sleep(ZUINT32 iMillionSeconds);
    
	/**
	* 销毁线程
	* @return ZVOID                       
	*/
    ZVOID ExitThread();
    
private:

    ZHandle m_hThread;                      // 线程的Handle或Linux中线程的ID
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
	* 初始化
	* @param  [in]    bManualReset        重置方式(人工重置1,自动重置0)
	* @param  [in]    bInitialState       事件初始信号态
	* @param  [in]    pName               事件名称
	* @return ZINT32                      成功返回SUCCESS，失败返回INVALID                       
	*/
	ZINT32 Init(ZBOOL bManualReset, ZBOOL bInitialState, ZLPCTSTR pName);

	/**
	* 将事件置到信号态
	* @return ZINT32                      成功返回SUCCESS，失败返回INVALID                         
	*/
	ZINT32 SetEvent();
 
	/**
	* 将事件置到无信号态
	* @return ZINT32                      成功返回SUCCESS，失败返回INVALID                        
	*/
	ZINT32 ResetEvent();

	/**
	* 获取事件保护锁
	* @return CCPMutex*                   事件保护锁指针                     
	*/
	CCPMutex* GetEventMutex(){return m_pMutex;};

	/**
	* 获取事件句柄
	* @return ZHandle                     事件句柄                 
	*/
	ZHandle GetHandle(){return m_hHandle;};

	/**
	* 获取信号激活状态
	* @return ZBOOL                       信号激活状态                                  
	*/
	ZBOOL  GetSignalState(){ return m_bSignal;};

	/**
	* 设置信号激活状态
	* @param  [in]    bSignal             事件信号态激活状态
	* @return ZVOID                                                 
	*/
	ZVOID  SetSignalState(ZBOOL bSignal){m_bSignal = bSignal;};

	/**
	* 判断是否自动重置
	* @return ZBOOL                       ZTURE表示自动重置，ZFALSE表示人工重置                                 
	*/
	ZBOOL  IsAutoReset(){return m_bAutoReset;};

private:

	ZBOOL IsValid();
	ZVOID Close();

private:

    CCPMutex* m_pMutex;                  // 该条件变量的互斥锁，若为空则在函数内部自创建
	ZHandle   m_hHandle;                 // 事件句柄
	ZBOOL     m_bAutoReset;              // 标识是否自动重置，默认为ZTURE
	ZBOOL     m_bSignal;                 // 条件变量是否激活，当激活事件发生时，该值为ZTRUE，用于等待事件时循环检测等待条件

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
	* 无条件等待事件
	* @param  [in]    pEvent              等待的对象指针
	* @return ZINT32                      成功返回ZWAIT_OBJECT_0，失败返回ZWAIT_FAILED                       
	*/
	static ZINT32 WaitEvent(CCPEvent* pEvent);

	/**
	* 时间等待事件
	* @param  [in]    pEvent              等待的对象指针
	* @param  [in]    dwMilliSeconds      等待的超时时间(单位:毫秒)
	* @return ZINT32                      成功返回ZWAIT_OBJECT_0，失败返回ZWAIT_FAILED                       
	*/
	static ZINT32 TimeWaitEvent(CCPEvent* pEvent, ZDWORD dwMilliSeconds);

};

class CCPSema
{
public:

	CCPSema();
	~CCPSema();

	/**
	* 初始化
	* @param  [in]    nInitialCount       信号量初始计数值，该值必须大于或等于0
	* @param  [in]    nMaxCount           信号量允许的最大计数值，该值必须大于0
	* @param  [in]    pName               信号量名称
	* @return ZINT32                      成功返回SUCCESS，失败返回INVALID                       
	*/
	ZINT32 Init(ZUINT32 nInitialCount, ZUINT32 nMaxCount, ZLPCTSTR pName);

	/**
	* 锁定信号量
	* @return ZINT32                      成功返回SUCCESS，失败返回INVALID                       
	*/
	ZINT32 Lock();

	/**
	* 尝试锁定信号量
	* @param  [in]    dwMilliseconds      尝试锁定的时间
	* @return ZINT32                      成功返回SUCCESS，失败返回INVALID                       
	*/
	ZINT32 TryLock(ZDWORD dwMilliseconds);

	/**
	* 释放信号量
	* @return ZINT32                      成功返回SUCCESS，失败返回INVALID                       
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
	* 初始化
	* @param  [in]    bInitalOwner        标识指定调用线程时是否立刻拥有该互斥量所有权
	* @param  [in]    pName               互斥量名称
	* @return ZINT32                      成功返回SUCCESS，失败返回INVALID                       
	*/
	ZINT32 Init(ZBOOL bInitalOwner, ZLPCTSTR pName);

	/**
	* 锁定互斥量
	* @return ZINT32                      成功返回SUCCESS，失败返回INVALID                       
	*/
	ZINT32 Lock();

	/**
	* 尝试锁定互斥量
	* @param  [in]    dwMilliseconds      尝试锁定的时间
	* @return ZINT32                      成功返回SUCCESS，失败返回INVALID                       
	*/
	ZINT32 TryLock(ZDWORD dwMilliseconds);

	/**
	* 解除锁定互斥量
	* @return ZINT32                      成功返回SUCCESS，失败返回INVALID                       
	*/
	ZINT32 UnLock();

	/**
	* 获取互斥量句柄
	* @return ZHandle                     互斥量句柄                 
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
	* 初始化
	* @return ZINT32                      成功返回SUCCESS，失败返回INVALID                       
	*/
	ZINT32 Init();

	/**
	* 进入临界区
	* @return ZINT32                      成功返回SUCCESS，失败返回INVALID                       
	*/
	ZINT32 Lock();

	/**
	* 尝试锁定临界区
	* @return ZINT32                      成功返回SUCCESS，失败返回INVALID                       
	*/
	ZINT32 TryLock();

	/**
	* 退出临界区
	* @return ZINT32                      成功返回SUCCESS，失败返回INVALID                       
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
