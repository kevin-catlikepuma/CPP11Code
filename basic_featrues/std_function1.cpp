#include <iostream>
#include <functional>

//std::function作为函数参数
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

//std::function可以取代函数指针的作用。它可以保持函数延迟执行，所以比较适合作为回调函数
class A
{
	std::function<void()> callback_;

public:
	A(const std::function<void()>& f) : callback_(f) {}

	void notify(void)
	{
		callback_();
	}
};

class Foo
{
public:
	void operator()(void)
	{
		std::cout << __FUNCTION__ << std::endl;
	}
};

int main(void)
{
	for (int i = 0; i < 10; ++i)
	{
		call_when_even(i, output);
	}

	std::cout << std::endl;

	Foo foo;
	A a(foo);
	a.notify();

	system("pause");
	return 0;
}