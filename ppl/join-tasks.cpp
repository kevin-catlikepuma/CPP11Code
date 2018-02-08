#include <ppltasks.h>
#include <iostream>
#include <array>
#include<numeric>

//https://msdn.microsoft.com/en-us/library/dd492427(v=vs.140).aspx

using namespace concurrency;
using namespace std;

void tasks_when_any()
{
	// Start multiple tasks.
	array<task<int>, 3> tasks = {
		create_task([]() -> int { return 88; }),
		create_task([]() -> int { return 42; }),
		create_task([]() -> int { return 99; })
	};

	// Select the first to finish.
	when_any(begin(tasks), end(tasks)).then([](pair<int, size_t> result) // you can also specify task<pair<int, size_t>> to produce a task-based continuation.
	{
		wcout << "First task to finish returns "
			<< result.first
			<< L" and has index "
			<< result.second
			<< L'.' << endl;
	}).wait();

	/* Sample output:
	First task to finish returns 42 and has index 1.
	*/
}

int wmain()
{
	// Start multiple tasks.
	array<task<void>, 3> tasks =
	{
		create_task([] { wcout << L"Hello from taskA." << endl; }),
		create_task([] { wcout << L"Hello from taskB." << endl; }),
		create_task([] { wcout << L"Hello from taskC." << endl; })
	};

	auto joinTask = when_all(begin(tasks), end(tasks));

	// Print a message from the joining thread.
	wcout << L"Hello from the joining thread." << endl;

	// Wait for the tasks to finish.
	joinTask.wait();
	/* Sample output:
	Hello from the joining thread. 异步执行，顺序每次执行可能不一样
	Hello from taskA.
	Hello from taskC.
	Hello from taskB.
	*/

	array<task<int>, 3> tasks1 =
	{
		create_task([]() -> int { return 88; }),
		create_task([]() -> int { return 42; }),
		create_task([]() -> int { return 99; })
	};

	auto joinTaskAdd = when_all(begin(tasks1), end(tasks1)).then([](vector<int> results)
	{
		wcout << L"The sum is "
			<< accumulate(begin(results), end(results), 0)
			<< L'.' << endl;
	});

	// Print a message from the joining thread.
	wcout << L"Hello from the joining thread." << endl;

	// Wait for the tasks to finish.
	joinTaskAdd.wait();

	/* Output:
	Hello from the joining thread.
	The sum is 229.
	*/
	system("pause");
}

