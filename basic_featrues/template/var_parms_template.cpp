#include<iostream>
using namespace std;

/*
...���ã�����һ�������������԰���0���������������ģ�嶨���ұ߿��Խ�������չ����һ��һ�������Ĳ���

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