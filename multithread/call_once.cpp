#include <thread>
#include <mutex>
#include <iostream>

std::once_flag flag;

void do_once()
{
	//call_once保证在多线程环境下某个函数或lamda表达式只执行一次
	std::call_once(flag, []() { std::cout << "Called once" << std::endl; });  //执行一次

	std::cout << "Other Code" << std::endl;   //执行三次
}

int main(void)
{
	std::thread t1(do_once);
	std::thread t2(do_once);
	std::thread t3(do_once);

	t1.join();
	t2.join();
	t3.join();

	system("pause");
	return 0;
}