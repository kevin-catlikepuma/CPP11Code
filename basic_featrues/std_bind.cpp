#include <iostream>
#include <functional>

void call_when_even(int x, const std::function<void(int)>& f)
{
	if (!(x & 1))  //x % 2 == 0
	{
		f(x);
	}
}

void output(int x)
{
	std::cout << x << " ";
}

void output1(int x, int y)
{
	std::cout << x << " " << y << std::endl;
}

void output_mul_2(int x)
{
	std::cout << x * 2 << " ";
}

int output_add_2(int x)
{
	std::cout << x + 2 << " ";
	return x + 2;
}

//std::bind�������ɵ��ö����������һ��󶨡��󶨺�Ľ������ʹ��std::function���б��棬���ӳٵ��õ��κ�������Ҫ��ʱ��
int main(void)
{
	//std::bind���˲�ͬ�ĺ���������������ִ�н��
	//std::placeholders::_1��һ��ռλ�����������λ�ý��ں�������ʱ��������ĵ�һ������
	{
		//std::bind���ص���һ���º���������ֱ�Ӹ��Ƹ�һ��std::function����Ϊ�������ǲ�������������������
		auto fr = std::bind(output, std::placeholders::_1);
		for (int i = 0; i < 10; ++i)
		{
			call_when_even(i, fr);
		}

		std::cout << std::endl;
	}
	{
		//std::function<void(int)> fr = std::bind(output_mul_2, std::placeholders::_1);
		auto fr = std::bind(output_mul_2, std::placeholders::_1);
		for (int i = 0; i < 10; ++i)
		{
			call_when_even(i, fr);
		}

		std::cout << std::endl;
	}

	{
		//std::function<int(int)> fadd = std::bind(output_add_2, std::placeholders::_1);
		auto fadd = std::bind(output_add_2, std::placeholders::_1);
		auto retvalue = fadd(5);
		std::cout << retvalue << std::endl;
		std::cout << std::endl;
	}

	//std::bindռλ��
	{
		std::bind(output1, 1, 2)();
		std::bind(output1, std::placeholders::_1, 2)(1);
		std::bind(output1, 2, std::placeholders::_1)(1);

		//std::bind(output1, 2, std::placeholders::_2)(1);  //error:����ʱû�еڶ�������

		std::bind(output1, 2, std::placeholders::_2)(1, 2);  //��� 2 2   ����ʱ��һ���������̵���

		std::bind(output1, std::placeholders::_1, std::placeholders::_2)(1, 2);  //��� 1 2
		std::bind(output1, std::placeholders::_2, std::placeholders::_1)(1, 2); //��� 2 1
	}

	system("pause");
	return 0;
}