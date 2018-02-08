#include <iostream>
#include "msgtypes.h"
using namespace std;

std::wstring TestJson()
{
	BJTable table(234567, 1000);
	web::json::value& jsonValue = table.AsJSON();
	return jsonValue.serialize();

	//web::json::value result = web::json::value::object();
	//result[ID] = web::json::value::number((double)5.6);
	//result[CAPACITY] = web::json::value::number((double)2.3);

	//return result.serialize();

}

int wmain()
{
	std::wstring wstrJson = TestJson();
	ucout << wstrJson.c_str() << endl;
	system("pause.");
	return 0;
}