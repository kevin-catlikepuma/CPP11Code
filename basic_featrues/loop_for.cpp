#include<iostream>
#include<vector>
#include<algorithm>
#include<map>
#include<unordered_map>

using namespace std;

void do_cout(int nValue)
{
	cout << "do_cout: " << nValue << endl;
}

void Iterator_container()
{
	std::vector<int> arr = { 1, 2, 3 };
	
	//1
	for (auto it = arr.begin(); it != arr.end(); ++it)
	{
		cout << "For: " << *it << endl;
	}

	//2 
	std::for_each(arr.begin(), arr.end(), do_cout);

	//3 �Զ��Ƶ�����vec��ֵ������
	for (auto n : arr) // for (auto& n : arr) ��Ҫ�ڱ������޸������е�ֵ����Ҫʹ������
	{
		cout << "The range-based for statement: " << n << endl;
	}

	//����map
	std::map<std::string, int> mm = { {"kevin", 123},{ "tom", 123 },{ "andy", 123 } };
	for (auto& mV : mm) //mV��������std::pair����Ҫʹ��.first,.second��ȡֵ
	{
		cout << mV.first.c_str() << " --> " << mV.second << endl;
	}

	std::unordered_map<std::string, int> uM = { { "kevin", 123 },{ "tom", 123 },{ "andy", 123 } };
	for (auto& mV : uM)
	{
		cout << mV.first.c_str() << " --> " << mV.second << endl;
	}
}

void Error_loop()
{
	std::vector<int> arr = { 1, 2, 3 };
	for (auto val : arr)
	{
		cout << val << endl;
		arr.push_back(10); //�����������ڵ���ʱ�޸��������ܻ����������ʧЧ����������֮��Ľ��
	}
}


std::vector<int> g_Vec = { 1,2,3,4,5 };
std::vector<int> & get_range(void)
{
	cout << "get_range ->: " << endl;
	return g_Vec;
}

void BaseRangeFor()
{
	for (auto val : get_range())  // �ڻ��ڷ�Χ��forѭ�����ԣ�ð�ź���ı��ʽֻ��ִ��һ��
	{
		cout << "BaseRangeFor:" << val << endl;
	}
}

int main()
{
	BaseRangeFor();

	Iterator_container();

	//Error_loop();


	system("pause");
}