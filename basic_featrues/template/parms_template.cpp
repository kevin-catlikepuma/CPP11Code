#include<iostream>
using namespace std;

/*
...���ã�����һ�������������԰���0���������������ģ�嶨���ұ߿��Խ�������չ����һ��һ�������Ĳ���
չ����������1. ͨ���ݹ��ģ�庯������������չ����2.ͨ�����ű��ʽ�ͳ�ʼ���б�ķ�ʽչ����������
*/

//�ݹ���ֹ����
void Func()
{
	cout << "empty" << endl;
}

/* or
//�ݹ���ֹ����
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