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

//std::bind用来将可调用对象与其参数一起绑定。绑定后的结果可以使用std::function进行保存，并延迟调用到任何我们需要的时候
int main(void)
{
	//std::bind绑定了不同的函数，控制了最后的执行结果
	//std::placeholders::_1是一个占位符，代表这个位置将在函数调用时，被传入的第一个参数
	{
		//std::bind返回的是一个仿函数，可以直接复制给一个std::function。因为这里我们并不关心真正返回类型
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

	//std::bind占位符
	{
		std::bind(output1, 1, 2)();
		std::bind(output1, std::placeholders::_1, 2)(1);
		std::bind(output1, 2, std::placeholders::_1)(1);

		//std::bind(output1, 2, std::placeholders::_2)(1);  //error:调用时没有第二个参数

		std::bind(output1, 2, std::placeholders::_2)(1, 2);  //输出 2 2   调用时第一个参数被吞掉了

		std::bind(output1, std::placeholders::_1, std::placeholders::_2)(1, 2);  //输出 1 2
		std::bind(output1, std::placeholders::_2, std::placeholders::_1)(1, 2); //输出 2 1
	}

	system("pause");
	return 0;
}