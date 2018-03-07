#include <thread>
#include <iostream>

using namespace std;

void thread_func()
{
	std::cout << "this method is running." << std::endl;
	this_thread::sleep_for(chrono::seconds(3)); //���߳�����3s
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

//��wmain�����У��̶߳���th���������̺߳���thread_func������������������������쳣��
//Ӧ�ñ�֤�̶߳���������������̺߳���ִ����ʱ��Ȼ���ڣ�����ͨ�����£�
//1.ͨ��join�����������̣߳��ȴ��̺߳���ִ�����
//2.ͨ��detach�������߳��ں�ִ̨��
//3.ͨ�����̶߳��󱣴浽һ�������У��Ա�֤�̶߳������������
int wmain()
{
	std::cout << std::thread::hardware_concurrency() << std::endl; // ��ȡcpu����
	std::thread th(thread_func);  //�����̶߳���th
	std::cout << th.get_id() << std::endl;
	//th.join(); // join�������������̣߳��ȴ��߳�ִ�н���
	th.detach();  //����detach���������������̣߳����̺߳��̶߳������
	

	//�����߳�
	std::thread th_p(thread_func_params, 1, 2.4, "test");
	std::cout << th_p.get_id() << std::endl;
	th_p.join();

	system("pause");

	return 0;
}

