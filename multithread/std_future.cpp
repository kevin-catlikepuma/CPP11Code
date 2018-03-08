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
thread库提供了future用来访问异步操作结果，因为一个异步操作的结果不能马上获取，
只能在未来某个时候从某个地方获取，这个异步操作的结果是一个未来的期待值，所以称为future。
future提供了获取异步操作结果的通道。

可以通过同步等待的方式来获取结果，
可以通过查询future状态获取异步操作结果（future_status：Deferred（未开始）,Ready（完成）,Timeout（操作超时））
*/
/*
packaged_task包装了一个可调用的对象的包装类（如函数，正则表达式，函数对象），将函数和future绑定起来，以便异步调用
*/
int main(void)
{
	std::packaged_task<int()> task([]() {return 10; });

	std::packaged_task<int(int)> tsk (func);
	std::future<int> ft = tsk.get_future();
	//std::thread(std::move(tsk), 2).detach();
	std::thread th(std::ref(tsk), 2); // 2为传递给线程函数的参数
	th.detach();

	//auto ret = ft.get(); //等待task完成并获取返回值，而join只是等待线程完成，不能获取返回值

	//循环查询任务状态是否结束
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


	//std::promise将数据和future绑定起来，为获取线程函数中的某个值提供便利
	//在线程函数中为外面传进来的promise赋值，在线程函数执行完成之后就可以通过promise的future获取该值了。
	std::promise<int> pr;
	//std::thread t([](std::promise<int>& p) {p.set_value_at_thread_exit(9); }, std::ref(pr));
	std::thread t(func_promise, std::ref(pr));
	t.detach(); //如果注释掉该语句，线程可能会先于线程函数结束，会引发程序异常

	std::future<int> ft1 = pr.get_future();
	auto ret1 = ft1.get();

	std::cout << "promise bind value: " << ret1 << std::endl;
	system("pause");
	return 0;
}