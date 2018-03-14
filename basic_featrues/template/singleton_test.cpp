#include <iostream>
#include <string>
#include "Singleton.hpp"

using namespace std;

struct A
{
	A(const string& str) { cout << "A" << str << endl; }
	A(string&& x) { cout << "AA" << endl; }
};

struct B
{
	B(const string& str) { cout << "B" << str << endl; }
	B(string&& x) { cout << "BB" << x << endl; }
};

struct C
{
	C(int x, double y) { cout << x << "," << y << endl; }
	void Fun() { cout << "test" << endl; }
};

int main()
{
	string str = "bb";

	//创建A类型的单例
	Singleton<A>::Instance(str); //输出Abb

	//创建B类型的单例，临时变量str被move之后变成右值，然后可以根据移动语义避免赋值
	Singleton<B>::Instance(std::move(str));  //输出BBbb

	//创建C类型的单例，含有两个参数
	Singleton<C>::Instance(1, 3.14); //调用两个参数的构造函数
	Singleton<C>::GetInstance()->Fun();

	Singleton<A>::DestoryInstance();
	Singleton<B>::DestoryInstance();
	Singleton<C>::DestoryInstance();

	system("pause");
}