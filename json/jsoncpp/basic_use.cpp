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

void ParserJsonFromFile()
{
	Json::Reader reader;
	Json::Value	root;

	std::ifstream ifs("example_styled_writer.json", std::ios::binary); //open file .json

	if(reader.parse(ifs, root))
	{
		//success
		std::string value1 = root["encoding"].asString();
		Json::Value inObj = root["indent"];
		if(inObj.type() == Json::objectValue)
		{
			int nValue = inObj["length"].asInt();

			Json::Value intObj = inObj["length"];
			if(intObj.type() == Json::intValue)
			{
				int nValue1 = intObj.asInt();
			}

			int k = 1000;
		}
	}
	
	ifs.close();
}

/*
ֵ��ǿ�����ǣ�Json::Value��C++�е�map��һ����ͬ���ص㣬���ǵ��㳢�Է���һ�������ڵ� key ʱ�����Զ���������һ��key-valueĬ��Ϊnull��ֵ��
�ܽ����Ҫ�ж��Ƿ���key��ʹ��isMember��Ա������value�Ƿ�Ϊnullʹ��isNull��Ա������value�Ƿ�Ϊ�տ�����empty() �� size()��Ա������
*/

void ParserJsonFromString()
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
			for(int i = 0; i < static_cast<int>(arrayObj.size()); i++)
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
			for(int i = 0; i < static_cast<int>(listObj.size()); i++)
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

/*
Json::Writer �� Json::Reader�෴���ǰ�Json::Value����д��string�����С�
Json::Writer�Ǹ������࣬����������Json::FastWriter��Json::StyledWriter�̳С� 
FastWriter�����޸�ʽ��д�룬������Json����������û�и�ʽ��
StyledWriter���Ǵ��и�ʽ��д�룬��������Ƚ��Ѻá�
*/
void InsertRomveJsonIntoFile()
{
	Json::Reader reader;
	Json::StyledWriter swriter;

	std::ifstream ifs;
	std::ofstream ofs;

	ifs.open("example_styled_writer.json", std::ios::binary);
	if(!ifs.is_open())
	{
		return;
	}

	Json::Value root;
	if(reader.parse(ifs, root))
	{
		Json::Value::Members memkeys = root.getMemberNames();

		//bool Value::removeIndex(ArrayIndex index, Value* removed)
		Json::Value removeObj1;
		bool bReturn = root.removeIndex(0, &removeObj1);
		if(bReturn)
		{
			if(removeObj1.type() == Json::stringValue)
			{
				std::string strValue = removeObj1.asString();
				int k = 1000;
			}
		}


		Json::Value removeObj2 = root.removeMember("objectlist");
		if(removeObj2.type() == Json::arrayValue)
		{
			for(int i = 0; i < static_cast<int>(removeObj2.size()); i++)
			{
				Json::Value obj = removeObj2[i];
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

		memkeys = root.getMemberNames();

		Json::Value insertObj1;
		Json::Value insertObj2;
		Json::Value insertArrayObj;

		insertObj1["sex"] = "male";
		insertObj2["contry"] = "China";
		insertArrayObj.append(insertObj1);
		insertArrayObj.append(insertObj2);

		root["sex"] = "male";
		root["contry"] = "China";
		root["info"] = insertArrayObj;

		std::string strJson = root.toStyledString();
		std::string strWriter = swriter.write(root);
		ofs.open("insert_writer.json");
		if(ofs.is_open())
		{
			//ofs.write(strWriter.c_str(), strWriter.length());
			ofs << strWriter;
		}
		
		ofs.close();
	}

	ifs.close();
}