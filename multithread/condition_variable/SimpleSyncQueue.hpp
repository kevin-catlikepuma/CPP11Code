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
		//unique_lock��lock_guard��֮ͬ�����ڿ�����ʱ�ͷŻ���������lock_guardֻ���ڳ���������֮��Ż��ͷŻ�����
		std::unique_lock<std::mutex> locker(m_mutex);
		
		//��������m_notEmpty���ȼ���ж�ʽ�Ƿ�����
		//!m_queue.empty() ���㣨���в�Ϊ�գ������ȡ��������wait����
		//!m_queue.empty() �����㣨����Ϊ�գ������ͷŻ��������̵߳ȴ���ֱ���յ�m_notEmpty.notify_one()֪ͨ���ٻ�ȡ������
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