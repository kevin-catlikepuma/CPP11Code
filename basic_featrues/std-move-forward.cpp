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
	PrintT(v);  //v ��ת������ֵ
	PrintT(std::forward<T>(v)); //����ԭ��������ת����PrintT
	PrintT(std::move(v));  //v�������ֵ
}

void Test()
{
	TestForward(1);//1Ϊ��ֵ
	int x = 1;
	TestForward(x);//auto�Ƶ�vΪδ������������T&&,����ֵ��ʼ���������ֵ����
	TestForward(std::forward<int>(x));
}

int main(void)
{
	Test();

	system("pause");
	return 0;
}