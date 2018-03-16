#include<iostream>
using namespace std;

/*
...作用：声明一个参数包，可以包含0到任意个参数；在模板定义右边可以将参数包展开成一个一个独立的参数
展开参数包：1. 通过递归的模板函数来将参数包展开；2.通过逗号表达式和初始化列表的方式展开参数包。
*/

//2.通过逗号表达式和初始化列表的方式展开参数包。

template<class T>
T print_args(T t)
{
	cout << t << endl;

	return t;
}

template<class... Args>
void Func(Args... args)
{
	//通过初始化列表来初始化一个变长数组，
	//{ (print_args(args), 0)... } 将会展开成 { (print_args(arg1), 0)，(print_args(arg2), 0)，(print_args(arg3), 0) etc... }
	//最终arr[]将被初始化为0， 长度为sizeof...(Args)
	//逗号表达式按顺序执行，所以都返回0

	//以下三种方法均可实现参数包展开
	//1
	int arr[] = { (print_args(args), 0)... }; //在构造arr数组时，将参数包展开，这个数组目的纯粹时为了在构造的过程展开参数包

	//2
	std::initializer_list<int> { (print_args(args), 0)... };

	//3
	std::initializer_list<int> { ([&] { cout << args << endl; }(), 0)... };

	for(auto i : arr)
	{ 
		cout << i << endl; //0
	}
	
}


int main()
{
	Func(4, 5, 6);
	system("pause");
}