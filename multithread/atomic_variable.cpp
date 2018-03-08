#include <thread>
#include <mutex>
#include <iostream>
#include <atomic>

struct Counter
{
	int value = 0;
	std::mutex mutex;

	void increment()
	{
		std::lock_guard<std::mutex> lock(mutex);
		++value;
		std::cout << value << std::endl;
	}

	void decrement()
	{
		std::lock_guard<std::mutex> lock(mutex);
		--value;
		std::cout << value << std::endl;
	}

	int get()
	{
		return value;
	}
};

struct AtomicCounter
{
	std::atomic<int> value = 0; //原子变量在多线程访问时不需要用互斥量来保护

	void increment()
	{
		++value;
		std::cout << value << std::endl;
	}

	void decrement()
	{
		--value;
		std::cout << value << std::endl;
	}

	int get()
	{
		return value.load();
	}
};

//Counter g_counter;
AtomicCounter g_counter;

void Increments()
{
	for (int i = 0; i < 10; ++i)
	{
		g_counter.increment();
		std::this_thread::sleep_for(std::chrono::microseconds(200));
	}
}

void Decrements()
{
	for (int i = 0; i < 5; ++i)
	{
		g_counter.decrement();
		std::this_thread::sleep_for(std::chrono::microseconds(150));
	}
}

int main(void)
{
	std::thread t1(Increments);
	std::thread t2(Decrements);

	t1.join();
	t2.join();

	system("pause");
	return 0;
}