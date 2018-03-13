#include <iostream>
#include <algorithm>
#include <vector>

class MyClass
{
public:
	std::wstring wstrManifest;
	MyClass();
	~MyClass();

private:

};

MyClass::MyClass()
{
	wstrManifest = L"";
}

MyClass::~MyClass()
{
}

bool MyClass_greater(MyClass* pClass1, MyClass* pClass2)
{
	return pClass1->wstrManifest >= pClass2->wstrManifest;
}

bool MyClass_less(MyClass* pClass1, MyClass* pClass2)
{
	return pClass1->wstrManifest <= pClass2->wstrManifest;
}

int main(void)
{
	std::vector<MyClass*> vecClass;
	MyClass* pClass1 = new MyClass();
	pClass1->wstrManifest = L"H";
	vecClass.push_back(pClass1);

	MyClass* pClass2 = new MyClass();
	pClass2->wstrManifest = L"C";
	vecClass.push_back(pClass2);

	MyClass* pClass3 = new MyClass();
	pClass3->wstrManifest = L"J";
	vecClass.push_back(pClass3);

	MyClass* pClass4 = new MyClass();
	pClass4->wstrManifest = L"Z";
	vecClass.push_back(pClass4);

	MyClass* pClass5 = new MyClass();
	pClass5->wstrManifest = L"Y";
	vecClass.push_back(pClass5);

	MyClass* pClass6 = new MyClass();
	pClass6->wstrManifest = L"B";
	vecClass.push_back(pClass6);

	MyClass* pClass7 = new MyClass();
	pClass7->wstrManifest = L"A";
	vecClass.push_back(pClass7);

	int nSize = vecClass.size();
	for (size_t i = 0; i < nSize; i++)
	{
		std::wcout << vecClass[i]->wstrManifest.c_str() << std::endl;
	}

	std::sort(vecClass.begin(), vecClass.end(), MyClass_less);

	std::wcout << L"Output after sorted." << std::endl;
	nSize = vecClass.size();
	for (size_t i = 0; i < nSize; i++)
	{
		std::wcout << vecClass[i]->wstrManifest.c_str() << std::endl;
	}

	system("pause");
	return 0;
}