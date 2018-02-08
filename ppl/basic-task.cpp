#include <ppltasks.h>
#include <iostream>
#include <string>

//https://msdn.microsoft.com/en-us/library/dd492427.aspx

using namespace concurrency;
using namespace std;

int wmain()
{
	// Create a task.
	task<int> t([]()
	{
		this_thread::sleep_for(chrono::seconds(3));
		return 42;
	});

	t.is_done(); //  Determines if the task is completed.
	t.wait(); //wait() is not necessary, because the call to get() also waits for the result.

	wcout << t.get() << endl;

	task<wstring> t1([]()
	{
		this_thread::sleep_for(chrono::seconds(2));
		return L"Kevin wei";
	});

	wcout << t1.get() << endl;
	
	task<wstring> ct([]()
	{
		this_thread::sleep_for(chrono::seconds(2));
		return L"Kevin wei";
	});

	auto t4 = ct.then([](wstring s2)
	{
		this_thread::sleep_for(chrono::seconds(2));
		return s2 + L" is your name.";
	});

	wcout << t4.get() << endl;

	auto t2 = create_task([]()
	{
		return L"Kevin2 wei";
	});

	auto t3 = t2.then([](wstring s1) // 此处必须有参数，必须跟create_task返回的参数吻合,此处s1的值为Kevin2 wei
	{
		return s1 + L"Kevin3 wei";
	});

	wcout << t2.get() << endl;
	wcout << t3.get() << endl;


	

	system("pause");
}
