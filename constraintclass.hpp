#pragma once

struct CONSTRAINT_PARAM
{
	std::string FETXMLEntry; // e.g. Maximum_Hours_Continuously, Minimum_Days_Per_Week
	std::wstring Description; // display name
	int type = 0; // 0 number,1 bool, 2 single list, 3 multiple list, 4 string
 	int from = 0;
	int val = 0; // and value
	int to = 1;
	ystring sval; // for type 4
	std::vector<std::wstring> lst; // for type 2 and 3
	std::vector<size_t> lst_selected;
};



class A_CONSTRAINT
{
public:
	std::wstring d0; // display name in the menu
	std::string x0; // e.g. ConstraintTeacherNotAvailableTimes
	int Type = 0; // 0 time 1 space
	int SubType = 0; // 0 All, 1 Teacher 2 Class 3 Room 4 Subject 5 Activity
	int HasTeacher = 0; // 0 1 or 2 for many
	int HasClass = 0;
	int HasRoom = 0;
	int HasSubject = 0;
	int w_from = 0;
	int w_to = 100;
	bool SupportsMultiple = 0;
	std::vector<CONSTRAINT_PARAM> params;
};

inline void BuildConstraintList(std::vector<A_CONSTRAINT>& constraints)
{
	constraints.clear();
}


