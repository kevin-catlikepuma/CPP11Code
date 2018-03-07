#include <thread>
#include <mutex>
#include <iostream>

struct Complex
{
	std::recursive_mutex mutex;
	int i;

	Complex() : i(0) {}

	void mul(int x)
	{
		std::lock_guard<std::recursive_mutex> lock(mutex);
		i *= x;
	}

	void div(int x)
	{
		std::lock_guard<std::recursive_mutex> lock(mutex);
		i /= x;
	}

	void both(int x, int y)
	{
		std::lock_guard<std::recursive_mutex> lock(mutex);
		mul(x);  //改函数中也会请求获取互斥锁，由于是在同一个线程中，所以不会发生死锁
		div(y);
	}
};


/*
递归锁（std::recursive_mutex）允许同一个线程多次获得同一个互斥锁，可以用来解决同一线程需要多次获取互斥量的死锁问题。
尽量不要使用递归锁，原因如下：
1.需要用到递归锁定的多线程互斥往往本身是可以简化的，递归锁很容易产生复杂的逻辑，不易理解
2.递归锁比非递归锁效率会低一些
3.同一线程获取递归锁的最大数并未说明，一旦超过一定次数可能会引发系统错误

*/

int main(void)
{
	Complex complex;

	complex.both(32, 23);  //因为同一线程可以多次获取同一互斥量，不会发生死锁

	system("pause");
	return 0;
}