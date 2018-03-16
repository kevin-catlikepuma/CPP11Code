#include<iostream>
using namespace std;

/*
...���ã�����һ�������������԰���0���������������ģ�嶨���ұ߿��Խ�������չ����һ��һ�������Ĳ���
չ����������1. ͨ���ݹ��ģ�庯������������չ����2.ͨ�����ű��ʽ�ͳ�ʼ���б�ķ�ʽչ����������
*/

//2.ͨ�����ű��ʽ�ͳ�ʼ���б�ķ�ʽչ����������

template<class T>
T print_args(T t)
{
	cout << t << endl;

	return t;
}

template<class... Args>
void Func(Args... args)
{
	//ͨ����ʼ���б�����ʼ��һ���䳤���飬
	//{ (print_args(args), 0)... } ����չ���� { (print_args(arg1), 0)��(print_args(arg2), 0)��(print_args(arg3), 0) etc... }
	//����arr[]������ʼ��Ϊ0�� ����Ϊsizeof...(Args)
	//���ű��ʽ��˳��ִ�У����Զ�����0

	//�������ַ�������ʵ�ֲ�����չ��
	//1
	int arr[] = { (print_args(args), 0)... }; //�ڹ���arr����ʱ����������չ�����������Ŀ�Ĵ���ʱΪ���ڹ���Ĺ���չ��������

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