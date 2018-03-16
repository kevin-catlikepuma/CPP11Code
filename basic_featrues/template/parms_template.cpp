#include<iostream>
using namespace std;

/*
...作用：声明一个参数包，可以包含0到任意个参数；在模板定义右边可以将参数包展开成一个一个独立的参数
展开参数包：1. 通过递归的模板函数来将参数包展开；2.通过逗号表达式和初始化列表的方式展开参数包。
*/

//递归终止函数
void Func()
{
	cout << "empty" << endl;
}

/* or
//递归终止函数
template<class T>
void Func(T t)
{
	cout << t << endl;
}
*/

template<class T, class... Args>
void Func(T head, Args... args)
{
	cout << "parameter " << head << endl;

	Func(args...);
}


int main()
{
	Func(4, 5, 6, 7, 8);
	system("pause");
}