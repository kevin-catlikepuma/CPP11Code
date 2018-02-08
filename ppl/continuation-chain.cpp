#include <ppltasks.h>
#include <iostream>

//https://msdn.microsoft.com/en-us/library/dd492427(v=vs.140).aspx

using namespace concurrency;
using namespace std;

void basic_chain()
{
	auto t = create_task([]() -> int
	{
		return 42;
	});

	int ret = t.then([](int result)
	{
		wcout << result << endl;
		return result + 10;
	}).get();

	wcout << ret << endl;
}
/* Output:
42
52
*/

int wmain()
{
	basic_chain();

	auto t = create_task([]() -> int
	{
		return 0;
	});

	// Create a lambda that increments its input value.
	auto increment = [](int n) { return n + 1; };

	// Run a chain of continuations and print the result.
	int result = t.then(increment).then(increment).then(increment).get();
	wcout << result << endl;

	system("pause");
}

/* Output:
3
*/