#include <thread>
#include <iostream>

using namespace std;

void thread_func()
{
	std::cout << "this method is running." << std::endl;
	this_thread::sleep_for(chrono::seconds(3)); //将线程休眠3s
	std::cout << this_thread::get_id() << std::endl;
	int i = 0;
	while (i < 10)
	{
		std::cout << i << ", times" << std::endl;

		i++;
	}
}

void thread_func_params(int i, double d, std::string s)
{
	std::cout << i << "," << d << "," << s.c_str() << std::endl;
	std::cout << this_thread::get_id() << std::endl;
}

//在wmain函数中，线程对象th可能先于线程函数thread_func结束（发生这种情况会引发异常）
//应该保证线程对象的生命周期在线程函数执行完时仍然存在，可以通过如下：
//1.通过join函数来阻塞线程，等待线程函数执行完成
//2.通过detach函数让线程在后台执行
//3.通过将线程对象保存到一个容器中，以保证线程对象的生命周期
int wmain()
{
	std::cout << std::thread::hardware_concurrency() << std::endl; // 获取cpu核数
	std::thread th(thread_func);  //创建线程对象th
	std::cout << th.get_id() << std::endl;
	//th.join(); // join函数将会阻塞线程，等待线程执行结束
	th.detach();  //调用detach函数将不会阻塞线程，将线程和线程对象分离
	

	//参数线程
	std::thread th_p(thread_func_params, 1, 2.4, "test");
	std::cout << th_p.get_id() << std::endl;
	th_p.join();

	system("pause");

	return 0;
}

