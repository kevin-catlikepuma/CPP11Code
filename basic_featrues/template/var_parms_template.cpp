#include<iostream>
using namespace std;

/*
...作用：声明一个参数包，可以包含0到任意个参数；在模板定义右边可以将参数包展开成一个一个独立的参数

*/
template<class... T>
void f(T... args)
{
	cout << sizeof...(args) << endl;
}


int main()
{
	f(1);
	f(1, 2, 3);
	f(4, 5, 6, 7, 8);
	system("pause");
}