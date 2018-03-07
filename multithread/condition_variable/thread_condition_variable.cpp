#include <thread>
#include <mutex>
#include <iostream>
#include "SyncQueue.hpp"

SyncQueue<int> syncQueue(5);

void PutDatas()
{
	for (int i = 0; i < 20; ++i)
	{
		syncQueue.Put(888);
		//std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

void TakeDatas()
{
	int x = 0;

	for (int i = 0; i < 20; ++i)
	{
		syncQueue.Take(x);
		std::cout << x << std::endl;
		//std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

int main(void)
{
	
	std::thread t2(TakeDatas);
	std::thread t1(PutDatas);
	t2.join();
	t1.join();
	

	system("pause");
	return 0;
}