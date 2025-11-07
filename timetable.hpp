#pragma once
struct FINISH_ITEM
{
	ystring name;
	ystring dispname;
	XML3::XMLElement* RootInProject = 0;

	// Add operator < for set
	bool operator <(const FINISH_ITEM& b) const
	{
		return name < b.name;
	}
};

struct FINISH_ENTRY
{
	ystring day;
	ystring hour;
	std::set<FINISH_ITEM> teachers;
	std::set<FINISH_ITEM> subjects;
	std::set<FINISH_ITEM> classes;
	std::set<FINISH_ITEM> rooms;
	std::set<FINISH_ITEM> activities;
};

struct FINISH_X
{
	ystring name;
	ystring dispname;
	std::vector<FINISH_ENTRY> entries;
	XML3::XMLElement* RootInProject = 0;

	void SortEntries(PROJECT* project_root);
};
class FINISHEDTT
{
public:
	std::map<std::wstring, FINISH_X> teachers;
	std::map<std::wstring, FINISH_X> rooms;
	std::map<std::wstring, FINISH_X> classes;

	bool Empty()
	{
		if (teachers.empty())
			return true;
		return false;
	}

	bool BuildTimeTableResult(PROJECT* project_root, XML3::XMLElement* result);
	void TeachersToRooms();
	void TeachersToClasses();
};


