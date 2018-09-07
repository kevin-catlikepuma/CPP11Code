#include <string>
#include "json\json.h"
#include <fstream>
#include "basic_use.h"

/*
{
    "encoding" : "UTF-8",
    "plug-ins" : [
        "python",
        "c++",
        "ruby"
        ],
    "indent" : { "length" : 3, "use_space": true }
	"tab-length":[],
    "tab":null
}
*/

//使用Json::Reader对Json文件进行解析：
//bool parse (const std::string &document, Value &root, bool collectComments=true)
//bool parse (std::istream &is, Value &root, bool collectComments=true)

void parser_jsonfile()
{
	Json::Reader reader;
	Json::Value	root;

	std::ifstream ifs("E:\\FS_Projects\\demo.json", std::ios::binary); //open file example.json

	if(reader.parse(ifs, root))
	{
		//success
		std::string value1 = root["encoding"].asString();
		Json::Value inObj = root["indent"];
		int nValue = inObj["length"].asInt();

	}
	else
	{
		//fail to parse
		ifs.close();
	}


}

/*
值得强调的是，Json::Value和C++中的map有一个共同的特点，就是当你尝试访问一个不存在的 key 时，会自动生成这样一个key-value默认为null的值对
总结就是要判断是否含有key，使用isMember成员函数，value是否为null使用isNull成员函数，value是否为空可以用empty() 和 size()成员函数。
*/

void parser_jsonstr()
{
	std::string json_str = "{ \"encoding\": \"UTF-8\", \"tab-length\": [], \"plug-ins\": [\"python\", \"C++\", \"ruby\", \"perl\"], \"indent\": { \"length\": 3, \"use_space\": true}, \"objectlist\": [ {\"int\": 123}, {\"string\": \"kevin\"}, {\"bool\": false}]}";
	Json::Reader reader;
	Json::Value	root;

	if(reader.parse(json_str.c_str(), root))
	{
		std::string value1 = root["encoding"].asString();
		Json::Value inObj = root["indent"];
		if(inObj.type() == Json::objectValue)
		{
			int nValue = inObj["length"].asInt();
			bool bValue = inObj["use_space"].asBool();
		}

		bool bMember = root.isMember("tab"); //root对象里不存在一个key为"tab"的元素，此时返回false
		bool bNull = root["tab"].isNull();  //访问一个不存在的 key 时，会自动生成这样一个key-value默认为null的值对，此时返回true
		bMember = root.isMember("tab");  //此时再访问key为"tab"的元素是否存在，则返回true
		bNull = root["tab"].isNull();  //判断key为"tab"的元素值是否为null，仍然返回true

		if(root.isMember("tab-length")) // 判断是否存在key为tab-length的元素
		{
			if(root["tab-length"].isNull()) //判断key为tab-length的元素值是否为null，注意不是空
			{
				;
			}
			else
			{
				bool bEmpty = root["tab-length"].empty();  //不为空，注意不是null
				int nSize = root["tab-length"].size();
				int k = 1000;
			}
		}

		Json::Value arrayObj = root["plug-ins"];
		Json::ValueType type = arrayObj.type();
		if(type == Json::arrayValue)
		{
			for(int i = 0; i < arrayObj.size(); i++)
			{
				Json::Value obj = arrayObj[i];  // 取出列表中的对象
				if(obj.type() == Json::stringValue)  // 判断列表中的对象是否为Json::stringValue
				{
					std::string strValue = obj.asString();
					int k = 1000;
				}
			}
		}

		Json::Value listObj = root["objectlist"];
		if(listObj.type() == Json::arrayValue)
		{
			for(int i = 0; i < listObj.size(); i++)
			{
				Json::Value obj = listObj[i];
				if(obj.isMember("int"))  //判断json对象里是否含有key为"int"的元素
				{
					int nValue = obj["int"].asInt();
					int k = 1000;
				}
				
				if(obj.isMember("bool"))
				{
					bool bValue = obj["bool"].asBool();
					int k = 1000;
				}

				if(obj.isMember("string"))
				{
					std::string strValue = obj["string"].asString();
					int k = 1000;
				}
			}
		}


		//写入文件
		Json::FastWriter fwriter;
		Json::StyledWriter swriter;


		std::string str = fwriter.write(root);
		std::ofstream ofs("example_fast_writer.json");
		ofs.write(str.c_str(), str.length());
		ofs.close();

		str = swriter.write(root);
		ofs.open("example_styled_writer.json");
		ofs.write(str.c_str(), str.length());
		ofs.close();
	}

}
