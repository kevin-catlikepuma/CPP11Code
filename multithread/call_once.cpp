#include <thread>
#include <mutex>
#include <iostream>

std::once_flag flag;

void do_once()
{
	//call_once��֤�ڶ��̻߳�����ĳ��������lamda���ʽִֻ��һ��
	std::call_once(flag, []() { std::cout << "Called once" << std::endl; });  //ִ��һ��

	std::cout << "Other Code" << std::endl;   //ִ������
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