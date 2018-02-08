#include <ppltasks.h>
#include <iostream>
#include <string>

//https://msdn.microsoft.com/en-us/library/dd492427.aspx

using namespace concurrency;
using namespace std;

task<wstring> write_to_string()
{
	// Create a shared pointer to a string that is 
	// assigned to and read by multiple tasks.
	// By using a shared pointer, the string outlives
	// the tasks, which can run in the background after
	// this function exits.
	auto s = make_shared<wstring>(L"Value 1");

	//task run on background. refer to "Using Lambda Expressions" in the up commented link
	return create_task([s]  // 捕获share_ptr，指针s不会释放，当s没有引用计数为0时，操作系统会释放
	{
		// Print the current value.
		wcout << L"Current value: " << *s << endl;
		// Assign to a new value.
		*s = L"Value 2";

	}).then([s]
	{
		this_thread::sleep_for(chrono::seconds(2));
		// Print the current value.
		wcout << L"Current value: " << *s << endl;
		// Assign to a new value and return the string.
		*s = L"Value 3";
		return *s;
	});
}

int wmain()
{
	// Create a chain of tasks that work with a string.
	auto t = write_to_string(); //返回及网下执行

	wcout << L"Function return. " << endl;

	// Wait for the tasks to finish and print the result.
	wcout << L"Final value: " << t.get() << endl;

	system("pause");

	/* Output:
	Function return. Current value: 
	Value 1
	Current value: Value 2
	Final value: Value 3
	*/
}
