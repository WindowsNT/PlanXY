#include "pch.h"
#pragma execution_character_set("utf-8")
#include "rest.h"
struct DAYANDHOUR
{
	int d = 0; // 1 based
	int h = 0; // 1 based
};





void DbClean(const char* xy)
{
	char x[1000] = {};
	if (1)
	{
		sqlite::sqlite sql(xy);

		sqlite::query q1(sql.h(), "SELECT name FROM sqlite_master WHERE type IN('table', 'view') AND name NOT LIKE 'sqlite_%' UNION ALL 	SELECT name FROM sqlite_temp_master		WHERE type IN('table', 'view')	ORDER BY 1");
		std::map<std::string, std::string> r;

		std::vector<std::string> torem;
		for (;;)
		{
			if (!q1.NextRow0(r))
				break;


			if (r["name"] == "SCHOOLS")
				continue;
			if (r["name"] == "AITH")
				continue;
			if (r["name"] == "LESSONSINAITH")
				continue;
			if (r["name"] == "PERSONS")
				continue;
			if (r["name"] == "STUDENTS")
				continue;
			if (r["name"] == "CLASSES")
				continue;
			if (r["name"] == "LESSONS")
				continue;
			if (r["name"] == "STUDENTSINCLASSES")
				continue;
			if (r["name"] == "TEACHERSINCLASSES")
				continue;
			if (r["name"] == "LESSONSINCLASSES")
				continue;
			if (r["name"] == "PERSONSINSCHOOL")
				continue;
			if (r["name"] == "PARENTSMYSCHOOL")
				continue;
			if (r["name"] == "BUILDINGS")
				continue;
			if (r["name"] == "TROOMPREF")
				continue;
			if (r["name"] == "TGAPPREF")
				continue;
			if (r["name"] == "TNOTAVAILABLE")
				continue;
			if (r["name"] == "EAE_PICK")
				continue;
			if (r["name"] == "TT_ATOM")
				continue;
			if (r["name"] == "TT_ATOM2")
				continue;
			if (r["name"] == "FET_RESULTS")
				continue;


			torem.push_back(r["name"]);
		}
		sql.One("BEGIN TRANSACTION");
		for (auto& t : torem)
		{
			sprintf_s(x, 1000, "DROP TABLE %s", t.c_str());
			sql.One(x);
		}
		sql.One("DELETE FROM FET_RESULTS");
		sql.One("COMMIT");
		sql.One("VACUUM");
	}
}



std::wstring StringFromYear(int y)
{
	if (y == 6) return L"ΛΓ";
	if (y == 5) return L"ΛΒ";
	if (y == 4) return L"ΛΑ";
	if (y == 3) return L"Γ";
	if (y == 2) return L"Β";
	if (y == 1) return L"Α";
	return L"";
}

int YearFromString(const wchar_t* s)
{
	if (wcsncmp(s, L"ΛΓ",2) == 0) return 6;
	if (wcsncmp(s, L"ΛΒ",2) == 0) return 5;
	if (wcsncmp(s, L"ΛΑ",2) == 0) return 4;
	if (wcsncmp(s, L"Γ",1) == 0) return 3;
	if (wcsncmp(s, L"Β",1) == 0) return 2;
	if (wcsncmp(s, L"Α",1) == 0) return 1;
	return 0;
}

int ClassIndexFromName(const wchar_t* n)
{
	if (!n)
		return 0;
	if (wcsstr(n, L"9")) return 9;
	if (wcsstr(n, L"8")) return 8;
	if (wcsstr(n, L"7")) return 7;
	if (wcsstr(n, L"6")) return 6;
	if (wcsstr(n, L"5")) return 5;
	if (wcsstr(n, L"4")) return 4;
	if (wcsstr(n, L"3")) return 3;
	if (wcsstr(n, L"2")) return 2;
	if (wcsstr(n, L"1")) return 1;
	return 0;
}

std::vector<int> ComboIndexFromName(const wchar_t* n)
{
	// Can be 1+3, be sure to check +
	std::vector<int> r;
	if (!n)
		return r;
	auto chr = wcschr(n, '+');
	if (!chr)
		return r;
	wchar_t t[100] = {};
	wcsncpy_s(t, 100, n, chr - n);
	int v = ClassIndexFromName(t);
	if (v != 0)
		r.push_back(v);
	chr++;
	v = ClassIndexFromName(chr);
	if (v != 0)
		r.push_back(v);
	return r;
}

std::map<std::string, std::wstring> ClassRowFromID(sqlite::sqlite& sql, int cid)
{
	char t[1000] = {};
	sprintf_s(t, 1000, "SELECT * FROM CLASSES WHERE ID = %i", cid);
	sqlite::query q1(sql.h(), t);
	std::map<std::string, std::wstring> r;
	q1.NextRow(r);
	return r;
}

std::map<std::string, std::wstring> LessonRowFromID(sqlite::sqlite& sql, int cid)
{
	char t[1000] = {};
	sprintf_s(t, 1000, "SELECT * FROM LESSONS WHERE ID = %i", cid);
	sqlite::query q1(sql.h(), t);
	std::map<std::string, std::wstring> r;
	q1.NextRow(r);
	return r;
}

std::map<std::string, std::wstring> PersonRowFromID(sqlite::sqlite& sql, int cid)
{
	char t[1000] = {};
	sprintf_s(t, 1000, "SELECT * FROM PERSONS WHERE ID = %i", cid);
	sqlite::query q1(sql.h(), t);
	std::map<std::string, std::wstring> r;
	q1.NextRow(r);
	return r;
}

std::map<std::string, std::wstring> PISRowFromID(sqlite::sqlite& sql, int cid)
{
	char t[1000] = {};
	sprintf_s(t, 1000, "SELECT * FROM PERSONSINSCHOOL WHERE UID = %i", cid);
	sqlite::query q1(sql.h(), t);
	std::map<std::string, std::wstring> r;
	q1.NextRow(r);
	return r;
}


std::map<std::string, std::wstring> StudentRowFromID(sqlite::sqlite& sql, int cid)
{
	char t[1000] = {};
	sprintf_s(t, 1000, "SELECT * FROM STUDENTS WHERE ID = %i", cid);
	sqlite::query q1(sql.h(), t);
	std::map<std::string, std::wstring> r;
	q1.NextRow(r);
	return r;
}

std::map<std::string, std::wstring> SMainClassRowFromStudent(sqlite::sqlite& sql, int kid)
{
	char t[1000] = {};
	sprintf_s(t, 1000, "SELECT * FROM STUDENTSINCLASSES WHERE KID = %i", kid);
	sqlite::query q1(sql.h(), t);
	std::map<std::string, std::wstring> r;
	while (q1.NextRow(r))
	{
		auto clr = ClassRowFromID(sql, _wtoi(r["CID"].c_str()));
		if (clr["MSSECTION"] == L"ΓΠ-ΓΥΜΝ" || clr["MSSECTION"] == L"ΓΠ-ΛΥΚ" || clr["MSSECTION"] == L"ΓΠ-ΕΠΑΛ")
			return clr;
	}
	return {};
}


int FindSubFromMain(sqlite::sqlite& sql, int mid)
{
	auto mcr = ClassRowFromID(sql, mid);
	auto idx = ClassIndexFromName(mcr["NAME"].c_str());
	char t[1000] = {};
	sprintf_s(t, 1000, "SELECT * FROM CLASSES WHERE LEVEL = %i AND MUSICMAIN = 1", _wtoi(mcr["LEVEL"].c_str()));
	sqlite::query q1(sql.h(), t);
	std::map<std::string, std::wstring> r;
	while (q1.NextRow(r))
	{
		auto idx2 = ClassIndexFromName(r["NAME"].c_str());
		if (idx2 == idx)
			return _wtoi(r["ID"].c_str());
	}
	return 0;
}






std::map<std::string, std::wstring> SMainClassRowFromStudent(sqlite::sqlite& sql, int kid);
std::map<std::string, std::wstring> ClassRowFromID(sqlite::sqlite& sql, int cid);
std::map<std::string, std::wstring> PersonRowFromID(sqlite::sqlite& sql, int lid);
std::map<std::string, std::wstring> LessonRowFromID(sqlite::sqlite& sql, int lid);
std::map<std::string, std::wstring> PersonRowFromID(sqlite::sqlite& sql, int lid);
std::map<std::string, std::wstring> StudentRowFromID(sqlite::sqlite& sql, int cid);

int ClassIndexFromName(const wchar_t* n);


// All kids load
struct K
{
	int id = 0;
	ystring name;
	int level = 0;
	int cidx = 0; // 1,2,3
	int ab = 0; // 1 A, 2 B
	int eap = 0; // 1 A 2 P english
};

std::vector<K> AllKidLoaded;


#include "config.h"
#include "days.h"
#include "buildingsandrooms.h"
#include "classes.h"
#include "lessons.h"
#include "teachers.h"
#include "kids.h"
#include "parents.h"
#include "tag.h"
#include "activities.h"
#include "timeconstraints.h"
#include "spaceconstraints.h"


#include "ttgapgen.hpp"

/*
		2024
			// Sal-Boufake anath (remove)
			// Skarimba Kwtitou 6 hours kat
			// Metallinos P Erase fake Jazz Band
			// Karypidou must set AB and Proxenias for A3
			// Needs "PARAM" set in TEACHERSINCLASSES to 1 FOR SOLFEGE because we can't deduct from hours (its both 2)
*/

#include <regex>
void BuildNewVersion(const char* dbf, [[maybe_unused]]  std::string groups1, std::string teachers, [[maybe_unused]] std::string activities1)
{
	sqlite::sqlite sql(dbf);
	//	XML3::XML xGroups(groups.c_str());


	std::vector<char> d1;
	d1.resize(teachers.size() + 1); memcpy(d1.data(), teachers.c_str(), teachers.size());
	std::wstring t1 = XML3::XMLU(d1.data()).wc();
	t1 = std::regex_replace(t1, std::wregex(L"\\&apos;"), L"'");
	std::string ct1 = XML3::XMLU(t1.c_str()).bc();
	d1.resize(ct1.size() + 1);
	memcpy(d1.data(), ct1.c_str(), ct1.size());
	d1[d1.size() - 1] = 0;

	//	teachers.replace(teachers.find("$name"), sizeof("$name") - 1, "Somename");

	XML3::XML xTeachers(d1.data(), d1.size());
	//	XML3::XML xActivities(activities.c_str());
	//	XML3::XML xFinalFet(finalfet.c_str());

	wchar_t wt[1000] = {};
	// Load all teachers from database
	std::map<std::wstring, int> teacher_to_tid;
	std::map<std::wstring, int> room_to_aid;
	std::map<std::wstring, int> kids_to_kid;
	std::map<std::wstring, int> lesson_to_lid;
	std::map<std::wstring, int> class_to_cid;
	if (1)
	{
		sqlite::query q(sql.h(), "SELECT * FROM PERSONS");
		std::map<std::string, std::wstring> r;
		while (q.NextRow(r))
		{
			std::wstring n = r["LASTNAME"].c_str();
			n += L" ";
			n += r["FIRSTNAME"].c_str();
			teacher_to_tid[n] = _wtoi(r["ID"].c_str());
		}
	}
	if (1)
	{
		sqlite::query q(sql.h(), "SELECT * FROM LESSONS");
		std::map<std::string, std::wstring> r;
		while (q.NextRow(r))
		{
			std::wstring n = r["NAME"].c_str();
			swprintf_s(wt, 1000, L"L%s|%s", r["LEVEL"].c_str(), n.c_str());
			lesson_to_lid[wt] = _wtoi(r["ID"].c_str());
		}
	}
	if (1)
	{
		sqlite::query q(sql.h(), "SELECT * FROM CLASSES");
		std::map<std::string, std::wstring> r;
		while (q.NextRow(r))
		{
			std::wstring n = r["NAME"].c_str();
			swprintf_s(wt, 1000, L"L%s|%s", r["LEVEL"].c_str(), n.c_str());
			class_to_cid[wt] = _wtoi(r["ID"].c_str());

			if (r["MAIN"] == L"1")
			{
				swprintf_s(wt, 1000, L"L%s|%s|Α", r["LEVEL"].c_str(), r["NAME"].c_str());
				class_to_cid[wt] = _wtoi(r["ID"].c_str());
				swprintf_s(wt, 1000, L"L%s|%s|Β", r["LEVEL"].c_str(), r["NAME"].c_str());
				class_to_cid[wt] = _wtoi(r["ID"].c_str());
			}

		}
	}
	if (1)
	{
		sqlite::query q(sql.h(), "SELECT * FROM AITH");
		std::map<std::string, std::wstring> r;
		while (q.NextRow(r))
		{
			std::wstring n = r["NAME"].c_str();
			room_to_aid[n] = _wtoi(r["ID"].c_str());
		}
	}
	if (1)
	{
		sqlite::query q(sql.h(), "SELECT * FROM STUDENTS");
		std::map<std::string, std::wstring> r;
		while (q.NextRow(r))
		{
			std::wstring n = r["LASTNAME"].c_str();
			n += L" ";
			n += r["FIRSTNAME"].c_str();

			auto cidx = ClassIndexFromName(SMainClassRowFromStudent(sql, _wtoi(r["ID"].c_str()))["NAME"].c_str());

			swprintf_s(wt, 1000, L"T|%s|%i|%s|%s", r["LEVEL"].c_str(), cidx, r["ID"].c_str(), n.c_str());
			kids_to_kid[wt] = _wtoi(r["ID"].c_str());
		}
	}


	std::vector<std::string> sql_statements;
	char t[1000] = {};

	// Parse teachers
	// Name, Room, Subject with Lx, Students with L|y|AB or T|
	std::map<int, std::map<int, std::map<int, bool>>> has_teacher;
	for (auto& xt : xTeachers.GetRootElement())
	{
		int tid = 0;
		auto whatn = xt.vv("name").GetWideValue();
		tid = teacher_to_tid[whatn];
		if (tid == 0)
			continue;

		for (auto& day : xt)
		{
			int dd = 0;
			if (day.vv("name").GetWideValue() == L"Δευτέρα") dd = 1;
			if (day.vv("name").GetWideValue() == L"Τρίτη") dd = 2;
			if (day.vv("name").GetWideValue() == L"Τετάρτη") dd = 3;
			if (day.vv("name").GetWideValue() == L"Πέμπτη") dd = 4;
			if (day.vv("name").GetWideValue() == L"Παρασκευή") dd = 5;

			for (auto& hour : day)
			{
				if (hour.GetChildrenNum() == 0)
					continue;
				int hh = _wtoi(hour.vv("name").GetWideValue().c_str());
				if (hh == 7 && dd == 1 && tid == 4)
				{

				}
				int aid = 0, cid = 0, lid = 0, kid = 0;
				for (int pass = 0; pass < 2; pass++)
				{
					for (auto& sub : hour)
					{
						if (sub.GetElementName() == "Room" && aid == 0)
						{
							auto whatr = sub.vv("name").GetWideValue();
							aid = room_to_aid[whatr];
							if (aid == 0)
							{

							}
						}

						if (sub.GetElementName() == "Subject" && lid == 0)
						{
							auto whatl = sub.vv("name").GetWideValue();
							lid = lesson_to_lid[whatl];
							if (lid == 0)
							{
								sprintf_s(t, 1000, "SELECT * FROM TEACHERSINCLASSES WHERE TID = %i AND CID = %i", tid, cid);
								sqlite::query q(sql.h(), t);
								std::map<std::string, std::wstring> r;
								while (q.NextRow(r))
								{
									lid = _wtoi(r["LID"].c_str());
								}

							}
						}


						if (sub.GetElementName() == "Students" && kid == 0 && cid == 0)
						{
							auto whatk = sub.vv("name").GetWideValue();
							kid = kids_to_kid[whatk];
							if (kid == 0)
								cid = class_to_cid[whatk];
							if (cid == 0)
							{
								sprintf_s(t, 1000, "SELECT * FROM TEACHERSINCLASSES WHERE TID = %i AND LID = %i", tid, lid);
								sqlite::query q(sql.h(), t);
								std::map<std::string, std::wstring> r;
								while (q.NextRow(r))
								{
									cid = _wtoi(r["CID"].c_str());
								}
							}
							if (cid == 0 && wcsncmp(whatk.c_str(), L"L7|", 3) == 0)
							{
								sprintf_s(t, 1000, "SELECT * FROM CLASSES WHERE NAME = '%s'", XML3::XMLU(whatk.c_str() + 3).bc());
								sqlite::query q(sql.h(), t);
								std::map<std::string, std::wstring> r;
								while (q.NextRow(r))
								{
									cid = _wtoi(r["ID"].c_str());
								}
							}
						}
					}
				}

				if ((dd == 1 || dd == 3) && hh >= 8)
				{
					aid = 82;
				}

				if (tid == 0 || lid == 0 || cid == 0)
					continue;


				if (aid == 0)
					aid = 82;

				if (has_teacher[tid][hh][dd] == 0)
				{
					has_teacher[tid][hh][dd] = 1;
					if (kid == 0)
						sprintf_s(t, 1000, "INSERT INTO FET_RESULTS (SID,TID,LID,CID,AID,DD,HH) VALUES (%i,%i,%i,%i,%i,%i,%i)", 1, tid, lid, cid, aid, dd, hh);
					else
						sprintf_s(t, 1000, "INSERT INTO FET_RESULTS (SID,TID,KID,LID,CID,AID,DD,HH) VALUES (%i,%i,%i,%i,%i,%i,%i,%i)", 1, tid, kid, lid, cid, aid, dd, hh);
					sql_statements.push_back(t);
				}

			}
		}


	}


	// fet_results
	// SID,TID,KID,LID,CID,AID,DD,HH,KIDS 

	std::string d0 = "";
	for (auto& s : sql_statements)
	{
		d0 += s;
		d0 += ";\r\n";
		sql.One(s.c_str());
	}
	//	d0 += "COMMIT;";
	sql.One("COMMIT");

	if (d0.size() > 10000)
	{
		std::vector<char> Fetch(const char* TheLink);

		auto f1 = Fetch("https://www.msa-apps.com/fetresults.php");
		bool AskText(HWND hh, const TCHAR* ti, const TCHAR* as, TCHAR* re, std::wstring* re2 = 0, int mxt = 1000);
		std::wstring res;
		if (AskText((HWND)0, L"MSA-Apps Key", L"MSA-Apps Key", 0, &res, 0))
		{
			RESTAPI::REST r;
			r.Connect(L"www.msa-apps.com", true);
			std::wstring k = L"X-key: ";
			k += res;
			auto i3 = r.RequestWithBuffer(L"fetresults.php", L"POST", { k }, d0.data(), d0.size());
			ShellExecute(0, L"open", L"https://www.msa-apps.com/fet/?real=1", 0, 0, SW_SHOWNORMAL);
		}
	}


}

ystring dbimportfile;


void LoadParametersMSA()
{
	XML3::XML x("f:\\wuitools\\wuifet\\msa.xml");
	auto& root = x.GetRootElement();
	ystring s;
	for (int i = 0; i < 6; i++)
	{
		auto& y = root["y"];
		s.Format(L"n%i", i + 1);
		active_years[i] = y.vv(s).GetValueInt(1);
	}

/*	for (int i = 0; i < 3; i++)
	{
		auto& y = root["cidx"];
		s.Format(L"n%i", i + 1);
		active_cidx[i] = y.vv(s).GetValueInt(1);
	}
	*/

	if (1)
	{
		auto& y = root["atom"];
		MaxAtomEPGL = y.vv("MaxAtomEPGL").GetValueInt(5);
		MaxAtomEPGLPiano = y.vv("MaxAtomEPGLPiano").GetValueInt(3);
	}

	if (1)
	{
		auto& y = root["time"];
		Teacher_Time_Constraints = y.vv("Teacher_Time_Constraints").GetValueInt(1);
		MaxHoursPerDay = y.vv("MaxHoursPerDay").GetValueInt(5);
		MaxSpanHours = y.vv("MaxSpanHours").GetValueInt(7);
		MaxContinuousHoursGen = y.vv("MaxContinuousHoursGen").GetValueInt(5);
		MaxContinuousHoursAt = y.vv("MaxContinuousHoursAt").GetValueInt(6);
		G_MaxGapsWeek = y.vv("G_MaxGapsWeek").GetValueInt(4);
		A_MaxGapsWeek = y.vv("A_MaxGapsWeek").GetValueInt(6);
	}

	if (1)
	{
		auto& y = root["space"];
		SpaceConstraints = y.vv("SpaceConstraints").GetValueInt(1);
	}

	if (1)
	{
		auto& y = root["activities"];
		CreateA_Syn = y.vv("Syn").GetValueInt(1);
		CreateA_Ind = y.vv("Independent").GetValueInt(1);
		CreateA_Aggl = y.vv("Aggl").GetValueInt(1);
		CreateA_2XGL = y.vv("XGL").GetValueInt(1);
		CreateA_Combo1 = y.vv("Combo1").GetValueInt(1);
		CreateA_Combo2 = y.vv("Combo2").GetValueInt(1);
		CreateA_Combo3 = y.vv("Combo3").GetValueInt(1);
		CreateA_Combo4 = y.vv("Combo4").GetValueInt(1);
		CreateA_MousEp4 = y.vv("MousEp4").GetValueInt(1);
		CreateA_MousEp6 = y.vv("MousEp6").GetValueInt(1);
		CreateA_Kat5 = y.vv("Kat5").GetValueInt(1);
		CreateA_Kat6 = y.vv("Kat6").GetValueInt(1);
		CreateA_CGP = y.vv("CGP").GetValueInt(1);
		CreateA_EAE = y.vv("EAE").GetValueInt(1);

	}

}

int LevelFromName(ystring LevelName, bool OnlyLyk = 0)
{
	int Level = 0;
	if (LevelName == L"Α") Level = 1;
	if (LevelName == L"Β") Level = 2;
	if (LevelName == L"Γ") Level = 3;
	if (LevelName == L"Α-ΛΥΚ") Level = 4;
	if (LevelName == L"Β-ΛΥΚ") Level = 5;
	if (LevelName == L"Γ-ΛΥΚ") Level = 6;
	if (OnlyLyk && Level < 4)
		Level += 3;
	return Level;
}



int msa2fetmain(const wchar_t* dbname,const wchar_t* targetfet,int stype)
{
//	ttgapgen(argc, argv);
//	if (1)
//		return 0;
	if (stype == 99)
	{
		DbClean(ystring(dbname));
	}
	else
	{
		TotalHours = 7;
	}
	dbimportfile = dbname;
	sqlite::sqlite sql(ystring(dbname).a_str());
	DeleteFile(targetfet);
	XML3::XML x(targetfet);


	auto& root = x.GetRootElement();
	root.SetElementName("fet");
	root.vv("version").SetValue("7.5.6");
	if (stype == 99)
	{
		LoadParametersMSA();
		root["Institution_Name"].SetContent(XML3::XMLU(L"Μουσικό Σχολείο Αλίμου"));
	}
	else
		root["Institution_Name"].SetContent(XML3::XMLU(L"Σχολείο"));

	// Statics
	CreateDays(x,stype);
	CreateBuildings(x, sql);
	CreateMainClasses(sql);
	CreateClasses3(x, sql,stype);
	CreateLessons(x,sql,stype);
	CreateTeachers(x, sql);
	CreateKids(x, sql);
	CreateParents(x, sql,stype);

	if (stype == 99)
	{
		FindTeachersWithSynola(x, sql);
		if (CreateA_Syn)
			CreateActivitiesForSynola3(sql);
		if (CreateA_Ind)
			CreateActivitiesForIndependentLessons(sql,stype);
		if (CreateA_Aggl)
			CreateActivitiesForAggl3(sql);
		if (CreateA_2XGL)
			CreateActivitiesFor2XGL(sql);
		CreateActivitiesForComboLessons3(sql);
		if (CreateA_MousEp4)
			CreateActivitiesForMousEp(sql, 4);
		if (CreateA_Kat5)
			CreateActivitiesForKat(sql, 5);
		if (CreateA_MousEp6)
			CreateActivitiesForMousEp(sql, 6);
		if (CreateA_Kat6)
			CreateActivitiesForKat(sql, 6);
		if (CreateA_CGP)
			CreateActivitiesForCGP(sql);
		if (CreateA_EAE)
		{
			CombineActivitesForEae(sql);
			CombineActivitesForEae3(sql);
		}
	}
	else
	{
		// Other Schools
		CreateActivitiesForIndependentLessons(sql,stype);
#ifdef _DEBUG
		CreateActivitiesForXLGeneral(sql, 0); // aggl
		CreateActivitiesForXLGeneral(sql,1); // gal germ
#endif
		CreateActivitiesForKat(sql, 5);
		CreateActivitiesForKat(sql, 6);
		CreateActivitiesForCGP(sql);
	}

	CreateActivities3(sql, x);
	CreateTimeConstraints3(sql, x,stype);
	CreateSpaceConstraints2(sql,x,stype);

	CreateTags(x);

	RemoveNonExistingActivities(x);
	CheckActivitiesWithoutStudents(x);

	x.Save(targetfet);


	return 0;
}