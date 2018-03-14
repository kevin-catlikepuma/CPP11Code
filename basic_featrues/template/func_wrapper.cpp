#include <iostream>

using namespace std;

template<class Function, class... Args>
inline auto FuncWrapper(Function && f, Args && ... args) -> decltype(f(std::forward<Args>(args)...))
{
	return f(std::forward<Args>(args)...);
}

void test0()
{
	cout << "void" << endl;
}

int test1()
{
	return 1;
}

int test2(int x)
{
	return x;
}

string test3(string s1, string s2)
{
	return s1 + s2;
}

int main()
{
	FuncWrapper(test0);

	auto x = FuncWrapper(test1);
	cout << "test1 return value: " << x << endl;

	auto y = FuncWrapper(test2, 5);
	cout << "test2 return value: " << y << endl;

	auto z= FuncWrapper(test3, "aa", "bb");
	cout << "test3 return value: " << z.c_str() << endl;

	system("pause");
}