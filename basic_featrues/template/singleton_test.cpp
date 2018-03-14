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

	//����A���͵ĵ���
	Singleton<A>::Instance(str); //���Abb

	//����B���͵ĵ�������ʱ����str��move֮������ֵ��Ȼ����Ը����ƶ�������⸳ֵ
	Singleton<B>::Instance(std::move(str));  //���BBbb

	//����C���͵ĵ�����������������
	Singleton<C>::Instance(1, 3.14); //�������������Ĺ��캯��
	Singleton<C>::GetInstance()->Fun();

	Singleton<A>::DestoryInstance();
	Singleton<B>::DestoryInstance();
	Singleton<C>::DestoryInstance();

	system("pause");
}