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
	std::wstring wstrTimeStamp;
	std::unordered_map<std::wstring, std::wstring> map_info;
	TaskInfo()
	{
		wstrTaskId.clear();
		wstrTimeStamp.clear();
		map_info = {};
	}
};