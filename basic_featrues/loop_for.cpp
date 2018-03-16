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

	//3 自动推到的是vec里值得类型
	for (auto n : arr) // for (auto& n : arr) 需要在遍历中修改容器中的值，需要使用引用
	{
		cout << "The range-based for statement: " << n << endl;
	}

	//有序map
	std::map<std::string, int> mm = { {"kevin", 123},{ "tom", 123 },{ "andy", 123 } };
	for (auto& mV : mm) //mV的类型是std::pair，需要使用.first,.second来取值
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
		arr.push_back(10); //扩大容器：在迭代时修改容器可能会引起迭代器失效，导致意料之外的结果
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
	for (auto val : get_range())  // 在基于范围的for循环而言，冒号后面的表达式只被执行一次
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