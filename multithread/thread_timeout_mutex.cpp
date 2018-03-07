#include <thread>
#include <mutex>
#include <iostream>

std::timed_mutex g_mutex;

void work()
{
	std::chrono::milliseconds timeout(100);

	while (true)
	{
		if (g_mutex.try_lock_for(timeout))
		{
			std::cout << std::this_thread::get_id() << ": Get mutex and lock()" << std::endl;

			std::this_thread::sleep_for(std::chrono::milliseconds(250));

			g_mutex.unlock();

			std::this_thread::sleep_for(std::chrono::milliseconds(250));
		}
		else
		{
			std::cout << std::this_thread::get_id() << ": timeout to get mutex." << std::endl;

			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	}
}

int main(void)
{
	std::thread t1(work);
	std::thread t2(work);

	t1.join();
	t2.join();

	system("pause");
	return 0;
}