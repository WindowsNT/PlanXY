//

struct PROJECT
{
	std::wstring file;
	std::shared_ptr<XML3::XML> x;
	bool Dirty = 0;

	XML3::XMLElement* FindSubElementByName(XML3::XMLElement* root, const wchar_t* name)
	{
		for (auto& j : *root)
		{
			if (trim(j.FindElementZ("Name")->GetContent()) == name)
				return &j;
		}
		return nullptr;
	}

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

