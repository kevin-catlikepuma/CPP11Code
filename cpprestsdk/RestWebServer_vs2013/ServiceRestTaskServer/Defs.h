#pragma once

enum TaskMark
{
	errOP = 0, //
	addTask,
	delTask,
	chgTask,
};

struct SubscribeTask
{
	std::wstring wstrTaskId;
	TaskMark mark;

	SubscribeTask() { wstrTaskId.clear(); mark = errOP; }
	SubscribeTask(const wchar_t* pwstrTaskId, TaskMark mk)
	{
		wstrTaskId = pwstrTaskId;
		mark = mk;
	}
};

struct TaskInfo
{
	std::wstring wstrTaskId;
	std::unordered_map<std::wstring, std::wstring> map_info;
	TaskInfo()
	{
		wstrTaskId.clear();
		map_info.clear();
	}

	web::json::value AsJSON() const
	{
		web::json::value res = web::json::value::object();
		res[U("taskID")] = web::json::value::string(wstrTaskId);

		web::json::value taskInfo = web::json::value::array(map_info.size());
		size_t idx = 0;
		for (auto iter = map_info.begin(); iter != map_info.end(); ++iter)
		{
			web::json::value tinfo = web::json::value::object();
			tinfo[U("key")] = web::json::value::string(iter->first);
			tinfo[U("value")] = web::json::value::string(iter->second);
			taskInfo[idx++] = tinfo;
		}

		res[U("taskInfo")] = taskInfo;

		return res;
	}
};

struct TaskItem
{
	std::wstring wstrKey;
	std::wstring wstrValuePath;

	TaskItem(const wchar_t* pwstrKey, const wchar_t* pwstrPath)
	{
		wstrKey = pwstrKey;
		wstrValuePath = pwstrPath;
	}
};