#include <thread>
#include <mutex>
#include <iostream>

struct Complex
{
	std::recursive_mutex mutex;
	int i;

	Complex() : i(0) {}

	void mul(int x)
	{
		std::lock_guard<std::recursive_mutex> lock(mutex);
		i *= x;
	}

	void div(int x)
	{
		std::lock_guard<std::recursive_mutex> lock(mutex);
		i /= x;
	}

	void both(int x, int y)
	{
		std::lock_guard<std::recursive_mutex> lock(mutex);
		mul(x);  //�ĺ�����Ҳ�������ȡ����������������ͬһ���߳��У����Բ��ᷢ������
		div(y);
	}
};


/*
�ݹ�����std::recursive_mutex������ͬһ���̶߳�λ��ͬһ���������������������ͬһ�߳���Ҫ��λ�ȡ���������������⡣
������Ҫʹ�õݹ�����ԭ�����£�
1.��Ҫ�õ��ݹ������Ķ��̻߳������������ǿ��Լ򻯵ģ��ݹ��������ײ������ӵ��߼����������
2.�ݹ����ȷǵݹ���Ч�ʻ��һЩ
3.ͬһ�̻߳�ȡ�ݹ������������δ˵����һ������һ���������ܻ�����ϵͳ����

*/

int main(void)
{
	Complex complex;

	complex.both(32, 23);  //��Ϊͬһ�߳̿��Զ�λ�ȡͬһ�����������ᷢ������

	system("pause");
	return 0;
}