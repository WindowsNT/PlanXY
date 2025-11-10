
//

enum class WHATVISIBLE
{
	TEACHERS,
	LESSONS,
	ROOMS,
	CLASSES,
	ACTIVITIES,
	TAGS,
	BUILDINGS,
	KIDS,
	PARENTS,
	ROOMFEATURES,
	UNKNOWNS,
	ERR,
};

struct DETACHDATA
{
	DWORD PID = 0;
	DWORD TID = 0;
	ystring workdir;
};


struct DETACHDATASESSION
{
	std::vector<DETACHDATA> Data;
};

struct PROJECT
{
	std::wstring file;
	std::shared_ptr<XML3::XML> x;
	bool Dirty = 0;

	int HowManyDays();
	int HowManyHours();
	std::vector<ystring> DaysNames();
	std::vector<ystring> HoursNames();

	std::optional<std::vector<DETACHDATASESSION>> HasActiveDetachData();


	HRESULT RemoveElement(const char* n, WHATVISIBLE type);
	void RemoveFrom(XML3::XMLElement* sroot, std::string SearchKey, std::string Search2, std::string n, WHATVISIBLE whatlook,std::string numbered = "");


	std::shared_ptr<XML3::XMLElement> ReloadRoomFeaturesElement();

	XML3::XMLElement* FindSubElementByName(XML3::XMLElement* root, const wchar_t* name,const char* src = "Name")
	{
		for (auto& j : *root)
		{
			auto j2 = j.FindElementZ(src);
			if (!j2)
				continue;
			if (trim(j2->GetContent()) == name)
				return &j;
		}
		return nullptr;
	}

	bool Save(bool As);

	// For Students
	XML3::XMLElement* FindSubElementByName2(XML3::XMLElement* root, const wchar_t* name)
	{
		for (auto& j : *root)
		{
			auto j2 = j.FindElementZ("Name");
			if (!j2)
				continue;
			if (trim(j2->GetContent()) == name)
				return &j;

			for (auto& jj : j)
			{
				if (jj.GetElementName() != "Group")
					continue;

				if (trim(jj.FindElementZ("Name")->GetContent()) == name)
					return &jj;

				for (auto& jjj : jj)
				{
					if (jjj.GetElementName() != "Subgroup")
						continue;
					if (trim(jjj.FindElementZ("Name")->GetContent()) == name)
						return &jjj;
				}
			}
		}
		return nullptr;
	}
};

