#pragma once

#include <thread>
#include <condition_variable>
#include <mutex>
#include <list>
#include <iostream>

template<typename T>
class SimpleSyncQueue
{
public:
	SimpleSyncQueue() {}

	void Put(const T& x)
	{
		std::lock_guard<std::mutex> locker(m_mutex);

		m_queue.push_back(x);
		m_notEmpty.notify_one();
	}

	void Take(T& x)
	{
		//unique_lock与lock_guard不同之处在于可以随时释放互斥锁，而lock_guard只有在出了作用域之后才会释放互斥锁
		std::unique_lock<std::mutex> locker(m_mutex);
		
		//条件变量m_notEmpty会先检查判断式是否满足
		//!m_queue.empty() 满足（队列不为空），则获取互斥锁，wait结束
		//!m_queue.empty() 不满足（队列为空），则释放互斥锁，线程等待，直到收到m_notEmpty.notify_one()通知，再获取互斥锁
		m_notEmpty.wait(locker, [this] { return !m_queue.empty(); });

		x = m_queue.front();
		m_queue.pop_front();
	}

	bool Empty()
	{
		std::lock_guard<std::mutex> locker(m_mutex);
		return m_queue.empty();
	}

	size_t Size()
	{
		std::lock_guard<std::mutex> locker(m_mutex);
		return m_queue.size();
	}

private:
	std::list<T>	m_queue;
	std::mutex		m_mutex;
	std::condition_variable	m_notEmpty;
};