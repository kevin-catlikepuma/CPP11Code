#include <thread>
#include <mutex>
#include <iostream>

using namespace std;

std::mutex g_lock;

void thread_func()
{
	g_lock.lock();  // ����

	std::cout << "Enter thread: " << this_thread::get_id() << std::endl;
	this_thread::sleep_for(chrono::seconds(1));
	std::cout << "Leave thread: " << this_thread::get_id() << std::endl;

	g_lock.unlock();
}


/*
lock_guard���Լ�lock/unlock��д����Ҳ����ȫ
��Ϊlock_guard�ڹ���ʱ���Զ������������������˳���������������ʱ�Զ��������Ӷ���֤����������unlock����
*/
void thread_func1()
{
	std::lock_guard<std::mutex> locker(g_lock); // ����������Զ�����

	std::cout << "Enter thread: " << this_thread::get_id() << std::endl;
	this_thread::sleep_for(chrono::seconds(1));
	std::cout << "Leave thread: " << this_thread::get_id() << std::endl;
}

int wmain()
{
	std::cout << "CPU counts: " << std::thread::hardware_concurrency() << std::endl;

	std::thread th1(thread_func1);
	std::thread th2(thread_func1);
	std::thread th3(thread_func1);

	th1.join();
	th2.join();
	th3.join();

	system("pause");

	return 0;
}

