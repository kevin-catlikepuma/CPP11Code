#include <thread>
#include <mutex>
#include <iostream>

using namespace std;

std::mutex g_lock;

void thread_func()
{
	g_lock.lock();  // 加锁

	std::cout << "Enter thread: " << this_thread::get_id() << std::endl;
	this_thread::sleep_for(chrono::seconds(1));
	std::cout << "Leave thread: " << this_thread::get_id() << std::endl;

	g_lock.unlock();
}


/*
lock_guard可以简化lock/unlock的写法，也更安全
因为lock_guard在构造时会自动锁定互斥量，而在退出作用域后进行析构时自动解锁，从而保证互斥量忘记unlock操作
*/
void thread_func1()
{
	std::lock_guard<std::mutex> locker(g_lock); // 出作用域后自动解锁

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

