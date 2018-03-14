#include <iostream>

void PrintT(int& t)
{
	std::cout << "lvalue" << std::endl;
}

template <typename T>
void PrintT(T&& t)
{
	std::cout << "rvalue" << std::endl;
}

template <typename T>
void TestForward(T&& v)
{
	PrintT(v);  //v 被转换成左值
	PrintT(std::forward<T>(v)); //按照原来的类型转发到PrintT
	PrintT(std::move(v));  //v将变成右值
}

void Test()
{
	TestForward(1);//1为右值
	int x = 1;
	TestForward(x);//auto推到v为未定的引用类型T&&,被左值初始化变成了左值引用
	TestForward(std::forward<int>(x));
}

int main(void)
{
	Test();

	system("pause");
	return 0;
}