
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
	ERR,
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


	HRESULT RemoveElement(const char* n, WHATVISIBLE type);
	void RemoveFrom(XML3::XMLElement* sroot, std::string SearchKey, std::string Search2, std::string n, WHATVISIBLE whatlook,std::string numbered = "");


	XML3::XMLElement* FindSubElementByName(XML3::XMLElement* root, const wchar_t* name,const char* src = "Name")
	{
		for (auto& j : *root)
		{
			if (trim(j.FindElementZ(src)->GetContent()) == name)
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
			if (trim(j.FindElementZ("Name")->GetContent()) == name)
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

