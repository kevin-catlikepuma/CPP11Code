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

//ʹ��Json::Reader��Json�ļ����н�����
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
ֵ��ǿ�����ǣ�Json::Value��C++�е�map��һ����ͬ���ص㣬���ǵ��㳢�Է���һ�������ڵ� key ʱ�����Զ���������һ��key-valueĬ��Ϊnull��ֵ��
�ܽ����Ҫ�ж��Ƿ���key��ʹ��isMember��Ա������value�Ƿ�Ϊnullʹ��isNull��Ա������value�Ƿ�Ϊ�տ�����empty() �� size()��Ա������
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

		bool bMember = root.isMember("tab"); //root�����ﲻ����һ��keyΪ"tab"��Ԫ�أ���ʱ����false
		bool bNull = root["tab"].isNull();  //����һ�������ڵ� key ʱ�����Զ���������һ��key-valueĬ��Ϊnull��ֵ�ԣ���ʱ����true
		bMember = root.isMember("tab");  //��ʱ�ٷ���keyΪ"tab"��Ԫ���Ƿ���ڣ��򷵻�true
		bNull = root["tab"].isNull();  //�ж�keyΪ"tab"��Ԫ��ֵ�Ƿ�Ϊnull����Ȼ����true

		if(root.isMember("tab-length")) // �ж��Ƿ����keyΪtab-length��Ԫ��
		{
			if(root["tab-length"].isNull()) //�ж�keyΪtab-length��Ԫ��ֵ�Ƿ�Ϊnull��ע�ⲻ�ǿ�
			{
				;
			}
			else
			{
				bool bEmpty = root["tab-length"].empty();  //��Ϊ�գ�ע�ⲻ��null
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
				Json::Value obj = arrayObj[i];  // ȡ���б��еĶ���
				if(obj.type() == Json::stringValue)  // �ж��б��еĶ����Ƿ�ΪJson::stringValue
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
				if(obj.isMember("int"))  //�ж�json�������Ƿ���keyΪ"int"��Ԫ��
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


		//д���ļ�
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
