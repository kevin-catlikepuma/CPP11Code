#include <ppltasks.h>
#include <iostream>
#include <string>
#include <array>

//https://msdn.microsoft.com/en-us/library/dd492427.aspx

using namespace concurrency;
using namespace std;

void create_identity_matrix2()
{
	task<array<array<int, 10>, 10>> create_identity_matrix = create_task([]()  // 返回值可用auto代指
	{
		array<array<int, 10>, 10> matrix;
		int row = 0;
		for_each(begin(matrix), end(matrix), [&row](array<int, 10>& matrixRow)
		{
			fill(begin(matrixRow), end(matrixRow), 0);
			matrixRow[row] = 1;
			row++;
		});
		return matrix;
	});

	auto print_matrix = create_identity_matrix.then([](array<array<int, 10>, 10> matrix) // 此处参数必须跟create_identity_matrix正则表达式返回值一致
	{
		for_each(begin(matrix), end(matrix), [](array<int, 10>& matrixRow)
		{
			wstring comma;
			for_each(begin(matrixRow), end(matrixRow), [&comma](int n)
			{
				wcout << comma << n;
				comma = L", ";
			});
			wcout << endl;
		});
	});

	print_matrix.wait();
}

int wmain()
{
	task<array<array<int, 10>, 10>> create_identity_matrix([]
	{
		array<array<int, 10>, 10> matrix;
		int row = 0;
		for_each(begin(matrix), end(matrix), [&row](array<int, 10>& matrixRow)
		{
			fill(begin(matrixRow), end(matrixRow), 0);
			matrixRow[row] = 1;
			row++;
		});
		return matrix;
	});

	auto print_matrix = create_identity_matrix.then([](array<array<int, 10>, 10> matrix) // 此处参数必须跟create_identity_matrix正则表达式返回值一致
	{
		for_each(begin(matrix), end(matrix), [](array<int, 10>& matrixRow)
		{
			wstring comma;
			for_each(begin(matrixRow), end(matrixRow), [&comma](int n)
			{
				wcout << comma << n;
				comma = L", ";
			});
			wcout << endl;
		});
	});

	print_matrix.wait();

	system("pause");
}
/* Output:
1, 0, 0, 0, 0, 0, 0, 0, 0, 0
0, 1, 0, 0, 0, 0, 0, 0, 0, 0
0, 0, 1, 0, 0, 0, 0, 0, 0, 0
0, 0, 0, 1, 0, 0, 0, 0, 0, 0
0, 0, 0, 0, 1, 0, 0, 0, 0, 0
0, 0, 0, 0, 0, 1, 0, 0, 0, 0
0, 0, 0, 0, 0, 0, 1, 0, 0, 0
0, 0, 0, 0, 0, 0, 0, 1, 0, 0
0, 0, 0, 0, 0, 0, 0, 0, 1, 0
0, 0, 0, 0, 0, 0, 0, 0, 0, 1
*/