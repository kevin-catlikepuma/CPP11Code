#include <cpprest\json.h>
using namespace web;

//https://github.com/Microsoft/cpprestsdk/wiki/JSON

int wmain()
{
	//construct a JSON value
	json::value v0 = json::value::null();
	json::value v1 = json::value::number(17);
	json::value v2 = json::value::number(3.1415);
	json::value v3 = json::value::boolean(true);
	json::value v4 = json::value::string(U("Hello Again!"));

	//Accessing Data
	int i = v1.as_integer();
	double d = v2.as_double();
	bool b = v3.as_bool();
	utility::string_t s = v4.as_string(); //将value绑定的值，转换为字符串


	json::value v5 = json::value::object(); //object对象可以看做一个map，可以直接健值对赋值,对应一个{}
	
	//array
	json::value v6 = json::value::array(); //对应一个[]
	json::value a6 = json::value::object();
	a6[U("A60Key")] = v4;
	v6[0] = a6; // 此处可以使用循环来做，将通过索引方式json::value加到array对象中

	json::value a7 = json::value::object();
	a7[U("A61Key")] = v3;
	v6[1] = a7;
	

	v5[U("V2Key")] = v2; //直接对object对象v5，赋值，健值对，类似map
	v5[U("V3Key")] = v3;
	v5[U("V4Key")] = v4;

	//将array对象，添加到object中。
	v5[U("array")] = v6; // \"array\":[{\"A60Key\":\"Hello Again!\"},{\"A61Key\":true}]

	//序列化，将V5对象，转换成字符串形式的json表达
	utility::string_t sjson = v5.serialize(); 
	//L"{\"V2Key\":3.1415000000000002,\"V3Key\":true,\"V4Key\":\"Hello Again!\",\"array\":[{\"A60Key\":\"Hello Again!\"},{\"A61Key\":true}]}"


	//json字符串解析


	json::value obj_json = json::value::parse(sjson);
	auto & v2key = obj_json.at(U("V2Key")); //此处为引用，下面的赋值才能改变“V2Key”的值
	auto v2value_old = v2key.as_number().to_double();
	v2key = json::value::number(5.6); //改变了“V2Key”的值为5.6
	auto v2value = v2key.as_number().to_double();

	auto v4key = obj_json.at(U("V4Key"));
	auto v4value = v4key.as_string();

	bool bV4key = obj_json.has_field(U("V4Key"));
	bool bV61key = obj_json.has_field(U("A61Key"));

	bool barray = obj_json.has_field(U("array"));
	if (barray)
	{
		json::value& arr_obj = obj_json.at(U("array")); //想改变原有的值，必须是引用
		json::value& ar1 = arr_obj[0];
		bool bar1 = ar1.has_field(U("A60Key"));
		if (bar1)
		{
			auto wstrv4 = ar1[U("A60Key")].as_string();

			ar1[U("A60Key")] = json::value::string(U("New Hello Again!"));
		}
	}

	utility::string_t snewjson = obj_json.serialize();
	//L"{\"V2Key\":5.5999999999999996,\"V3Key\":true,\"V4Key\":\"Hello Again!\",\"array\":[{\"A60Key\":\"New Hello Again!\"},{\"A61Key\":true}]}"

	system("pause.");
	return 0;
}