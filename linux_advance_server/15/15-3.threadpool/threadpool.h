#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <list>
#include <cstdio>
#include <exception>
#include <pthread.h>
#include "locker.h"

template< typename T >
class threadpool
{
	public:
		threadpool( int thread_number = 8, int max_requests = 10000 );
		~threadpool();
		bool append( T* request );

	private:
		static void* worker( void* arg );
		void run();

	private:
		int m_thread_number;//线程池中线程的数量
		int m_max_requests;//请求队列中允许的最多等待处理的请求数量
		pthread_t* m_threads;//线程池数组
		std::list< T* > m_workqueue;//请求队列
		locker m_queuelocker;//请求队列线程mutex锁
		sem m_queuestat;//是否有任务需要处理
		bool m_stop;//是否结束线程
};

template< typename T >
threadpool< T >::threadpool( int thread_number, int max_requests ) :
	m_thread_number( thread_number ), m_max_requests( max_requests ), m_stop( false ), m_threads( NULL )
{
	if( ( thread_number <= 0 ) || ( max_requests <= 0 ) )
	{
		throw std::exception();
	}

	m_threads = new pthread_t[ m_thread_number ];
	if( ! m_threads )
	{
		throw std::exception();
	}

	for ( int i = 0; i < thread_number; ++i )
	{
		printf( "create the %dth thread\n", i );
		if( pthread_create( m_threads + i, NULL, worker, this ) != 0 )
		{
			delete [] m_threads;
			throw std::exception();
		}
		//设置子线程为脱离线程(觉得可以不用设置)
		if( pthread_detach( m_threads[i] ) )
		{
			delete [] m_threads;
			throw std::exception();
		}
	}
}

template< typename T >
threadpool< T >::~threadpool()
{
	delete [] m_threads;
	m_stop = true;
}

template< typename T >
bool threadpool< T >::append( T* request )
{
	m_queuelocker.lock();
	if ( m_workqueue.size() > m_max_requests )
	{
		m_queuelocker.unlock();
		return false;
	}
	//增加一用户请求到链表尾
	m_workqueue.push_back( request );
	m_queuelocker.unlock();
	m_queuestat.post();
	return true;
}

template< typename T >
void* threadpool< T >::worker( void* arg )
{
	threadpool* pool = ( threadpool* )arg;
	pool->run();
	return pool;
}

template< typename T >
void threadpool< T >::run()
{
	while ( ! m_stop )
	{
		m_queuestat.wait();//GDB子线程断点位置
		m_queuelocker.lock();
		if ( m_workqueue.empty() )
		{
			//这个代码写的不好是查询法，应该改成通知法
			//如果一直empty就会一直空转,应该改用mutex+cond 成为通知法
			m_queuelocker.unlock();
			continue;
		}
		//返回第一个用户请求的引用
		T* request = m_workqueue.front();
		//pop_back()删除链表尾的一个用户请求
		m_workqueue.pop_front();
		m_queuelocker.unlock();
		if ( ! request )
		{
			continue;
		}
		request->process();
	}
}

#endif
