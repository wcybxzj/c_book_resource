#ifndef LOCKER_H
#define LOCKER_H

#include <exception>
#include <pthread.h>
#include <semaphore.h>

class sem
{
	public:
		sem()
		{
			if( sem_init( &m_sem, 0, 0 ) != 0 )
			{
				throw std::exception();
			}
		}
		~sem()
		{
			sem_destroy( &m_sem );
		}
		//以原子的方式信号量减1，如果当前信号量值是0则阻塞,
		//一直信号量变成非0才解除阻塞,然后减1
		bool wait()
		{
			return sem_wait( &m_sem ) == 0;
		}
		//以原子的方式将信号量增加
		bool post()
		{
			return sem_post( &m_sem ) == 0;
		}

	private:
		sem_t m_sem;
};

class locker
{
	public:
		locker()
		{
			if( pthread_mutex_init( &m_mutex, NULL ) != 0 )
			{
				throw std::exception();
			}
		}
		~locker()
		{
			pthread_mutex_destroy( &m_mutex );
		}
		bool lock()
		{
			return pthread_mutex_lock( &m_mutex ) == 0;
		}
		bool unlock()
		{
			return pthread_mutex_unlock( &m_mutex ) == 0;
		}

	private:
		pthread_mutex_t m_mutex;
};

class cond
{
	public:
		cond()
		{
			if( pthread_mutex_init( &m_mutex, NULL ) != 0 )
			{
				throw std::exception();
			}
			if ( pthread_cond_init( &m_cond, NULL ) != 0 )
			{
				pthread_mutex_destroy( &m_mutex );
				throw std::exception();
			}
		}
		~cond()
		{
			pthread_mutex_destroy( &m_mutex );
			pthread_cond_destroy( &m_cond );
		}
		bool wait()
		{
			int ret = 0;
			pthread_mutex_lock( &m_mutex );
			ret = pthread_cond_wait( &m_cond, &m_mutex );
			pthread_mutex_unlock( &m_mutex );
			return ret == 0;
		}
		bool signal()
		{
			return pthread_cond_signal( &m_cond ) == 0;
		}

	private:
		pthread_mutex_t m_mutex;
		pthread_cond_t m_cond;
};

#endif
