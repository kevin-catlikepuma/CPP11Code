#include <ppltasks.h>
#include <iostream>
#include <string>
#include <array>

//https://msdn.microsoft.com/en-us/library/dd997692.aspx

using namespace concurrency;
using namespace std;

void eh_continuations()
{
	wcout << L"Running a task..." << endl;
	// Create a task that throws.
	auto t = create_task([]() -> int
	{
		throw exception();
		return 42;
	});

	//
	// Attach two continuations to the task. The first continuation is  
	// value-based; the second is task-based.

	// Value-based continuation.
	auto c1 = t.then([](int n)
	{
		// We don't expect to get here because the antecedent 
		// task always throws.
		wcout << L"Received " << n << L'.' << endl;
	});

	// Task-based continuation.
	auto c2 = t.then([](task<int> previousTask)  // schedule for task t
	{
		// We do expect to get here because task-based continuations
		// are scheduled even when the antecedent task throws.
		try
		{
			wcout << L"Received " << previousTask.get() << L'.' << endl;  // task t excute.
		}
		catch (const exception& e)
		{
			wcout << L"Caught exception from previous task." << endl;
		}
	});

	// Wait for the continuations to finish.
	try
	{
		wcout << L"Waiting for tasks to finish..." << endl;
		(c1 && c2).wait();
	}
	catch (const exception& e)
	{
		wcout << L"Caught exception while waiting for all tasks to finish." << endl;
	}
}
/* Output:
Running a task...
Waiting for tasks to finish...
Caught exception from previous task.
Caught exception while waiting for all tasks to finish.
*/

void eh_task_chain()
{
	int n = 1;
	create_task([n]
	{
		wcout << L"In first task. n = ";
		wcout << n << endl;

		return n * 2;

	}).then([](int n)
	{
		wcout << L"In second task. n = ";
		wcout << n << endl;

		return n * 2;

	}).then([](int n)
	{
		wcout << L"In third task. n = ";
		wcout << n << endl;

		// This task throws.
		throw exception();
		// Not reached.
		return n * 2;

	}).then([](int n)
	{
		// This continuation is not run because the previous task throws.
		wcout << L"In fourth task. n = ";
		wcout << n << endl;

		return n * 2;

	}).then([](task<int> previousTask)
	{
		// This continuation is run because it is value-based.
		try
		{
			// The call to task::get rethrows the exception.
			wcout << L"In final task. result = ";
			wcout << previousTask.get() << endl; 
		}
		catch (const exception&)
		{
			wcout << L"<exception>" << endl;
		}
	}).wait();
}
/* Output:
In first task. n = 1
In second task. n = 2
In third task. n = 4
In final task. result = <exception>
*/

int wmain()
{
	wcout << L"Running a task..." << endl;
	// Create a task that throws.
	auto t = create_task([]
	{
		throw exception();
	});

	// Create a continuation that prints its input value.
	auto continuation = t.then([]
	{
		// We do not expect this task to run because
		// the antecedent task threw.
		wcout << L"In continuation task..." << endl;
	});

	// Wait for the continuation to finish and handle any 
	// error that occurs.
	try
	{
		wcout << L"Waiting for tasks to finish..." << endl;
		continuation.wait();

		// Alternatively, call get() to produce the same result.
		//continuation.get();
	}
	catch (const exception& e)
	{
		wcout << L"Caught exception." << endl;
	}
}
/* Output:
Running a task...
Waiting for tasks to finish...
Caught exception.
*/