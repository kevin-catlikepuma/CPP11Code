#include <iostream>
#include <thread>
#include <utility>
#include <future>
#include <vector>

int func(int x)
{
	std::this_thread::sleep_for(std::chrono::seconds(3));
	return x + 2; 
}

void func_promise(std::promise<int>& p)
{
	std::this_thread::sleep_for(std::chrono::seconds(1));

	p.set_value_at_thread_exit(10);

	std::this_thread::sleep_for(std::chrono::seconds(1));
}

/*
thread���ṩ��future���������첽�����������Ϊһ���첽�����Ľ���������ϻ�ȡ��
ֻ����δ��ĳ��ʱ���ĳ���ط���ȡ������첽�����Ľ����һ��δ�����ڴ�ֵ�����Գ�Ϊfuture��
future�ṩ�˻�ȡ�첽���������ͨ����

����ͨ��ͬ���ȴ��ķ�ʽ����ȡ�����
����ͨ����ѯfuture״̬��ȡ�첽���������future_status��Deferred��δ��ʼ��,Ready����ɣ�,Timeout��������ʱ����
*/
/*
packaged_task��װ��һ���ɵ��õĶ���İ�װ�ࣨ�纯����������ʽ���������󣩣���������future���������Ա��첽����
*/
int main(void)
{
	std::packaged_task<int()> task([]() {return 10; });

	std::packaged_task<int(int)> tsk (func);
	std::future<int> ft = tsk.get_future();
	//std::thread(std::move(tsk), 2).detach();
	std::thread th(std::ref(tsk), 2); // 2Ϊ���ݸ��̺߳����Ĳ���
	th.detach();

	//auto ret = ft.get(); //�ȴ�task��ɲ���ȡ����ֵ����joinֻ�ǵȴ��߳���ɣ����ܻ�ȡ����ֵ

	//ѭ����ѯ����״̬�Ƿ����
	std::future_status status;
	do
	{
		status = ft.wait_for(std::chrono::seconds(2));
		if (status == std::future_status::deferred)
		{
			std::cout << "deferred" << std::endl;
		}
		else if (status == std::future_status::timeout)
		{
			std::cout << "timeout" << std::endl;
		}
		else if (status == std::future_status::ready)
		{
			std::cout << "ready" << std::endl;
		}
	} while (status != std::future_status::ready);

	auto ret = ft.get();

	std::cout << "Thread return result: " << ret << std::endl;


	//std::promise�����ݺ�future��������Ϊ��ȡ�̺߳����е�ĳ��ֵ�ṩ����
	//���̺߳�����Ϊ���洫������promise��ֵ�����̺߳���ִ�����֮��Ϳ���ͨ��promise��future��ȡ��ֵ�ˡ�
	std::promise<int> pr;
	//std::thread t([](std::promise<int>& p) {p.set_value_at_thread_exit(9); }, std::ref(pr));
	std::thread t(func_promise, std::ref(pr));
	t.detach(); //���ע�͵�����䣬�߳̿��ܻ������̺߳��������������������쳣

	std::future<int> ft1 = pr.get_future();
	auto ret1 = ft1.get();

	std::cout << "promise bind value: " << ret1 << std::endl;
	system("pause");
	return 0;
}