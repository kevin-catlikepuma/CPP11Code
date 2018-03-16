#include <iostream>
#include <functional>

//std::function��Ϊ��������
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

//std::function����ȡ������ָ������á������Ա��ֺ����ӳ�ִ�У����ԱȽ��ʺ���Ϊ�ص�����
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