#include <iostream>
#include <string>

using namespace std;

//C++11引入了新的类型，右值引用：就是对一个右值进行引用的类型。声明时必须立即进行初始化（同左值引用一样）
//左值：指表达式结束后依然存在的持久对象
//右值：指表达式结束后就不在存在的临时对象
//区分方法：看能不能对表达式取地址，如果能则为左值；否则为右值。所有的具名变量或对象都是左值，右值不具名。

//在C++11中，右值概念构成：
//1.将亡值 (与右值引用相关的表达式，如将要被移动的对象，T&&函数返回值，std::move返回值和转换为T&&的类型的转换函数的返回值。）
//2.纯右值（非引用返回的临时变量，运算表达式产生的临时变量，原始字面量和lambda表达式等都是纯右值）。

//示例

int g_constructCount = 0;
int g_copyConstructCount = 0;
int g_destructCount = 0;

struct A
{
	A() { cout << "construct: " << ++g_constructCount << endl; }
	A(const A& a){ cout << "copy construct: " << ++g_copyConstructCount << endl; }

	~A() { cout << "destruct: " << ++g_destructCount << endl; }
};

A GetA() { return A(); }

int main()
{
	{
		A a = GetA();
		//A& a = GetA();  //错误：非常量左值引用不能接受右值，只能接受左值
	}


	{
		//这里a并不占内存，只是临时对象的一个别名
		A&& a = GetA(); //右值引用，可以减少一次构造和一次析构，原因是右值引用绑定了右值，让临时右值的生命周期延长了。这样就避免了临时对象的拷贝构造和析构
	}

	system("pause");

	return 0;
}