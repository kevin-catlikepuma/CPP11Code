#pragma once

#include<list>
#include<mutex>
#include<thread>
#include<condition_variable>
#include <iostream>

template<typename T>
class SyncQueue
{
private:
	bool IsFull() const
	{
		return m_queue.size() == m_maxSize;
	}

	bool IsEmpty() const
	{
		return m_queue.empty();
	}

public:
	SyncQueue(int maxSize) : m_maxSize(maxSize)
	{
	}

	void Put(const T& x)
	{
		std::lock_guard<std::mutex> locker(m_mutex);

		while (IsFull())
		{
			std::cout << "���������ˣ���Ҫ�ȴ�..." << std::endl;
			m_notFull.wait(m_mutex); //�����̣߳�ֱ���յ�notify_one֪ͨ
		}
		/*
		wait��������һ�����صķ��������Խ���һ������
		����while�����Ը���Ϊ��
		m_notFull.wait(locker, [this] { return !IsFull();});
		*/

		m_queue.push_back(x);
		m_Empty.notify_one();
	}

	void Take(T& x)
	{
		std::lock_guard<std::mutex> locker(m_mutex);

		while (IsEmpty())
		{
			std::cout << "���������ˣ���Ҫ�ȴ�..." << std::endl;
			m_Empty.wait(m_mutex);
		}
		/*
		m_Empty.wait(locker, [this] { return !IsEmpty();});
		*/

		x = m_queue.front();
		m_queue.pop_front();
		m_notFull.notify_one();
	}

	bool Empty()
	{
		std::lock_guard<std::mutex> locker(m_mutex);
		return m_queue.empty();
	}

	bool Full()
	{
		std::lock_guard<std::mutex> locker(m_mutex);
		return m_queue.size() == m_maxSize;
	}

	size_t Size()
	{
		std::lock_guard<std::mutex> locker(m_mutex);
		return m_queue.size();
	}

	int Count()
	{
		return m_queue.size();
	}

private:
	std::list<T>	m_queue;                  //������
	std::mutex		m_mutex;                    //�����������������������ʹ��
	std::condition_variable_any m_Empty;//��Ϊ�յ���������
	std::condition_variable_any m_notFull; //û��������������
	int		m_maxSize;                         //ͬ����������size
};