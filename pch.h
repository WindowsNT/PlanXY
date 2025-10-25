#pragma once
#define DISABLE_XAML_GENERATED_MAIN
#define _SILENCE_CXX17_RESULT_OF_DEPRECATION_WARNING
#include <ws2tcpip.h>
#include <winsock2.h>
#include <windows.h>
#include <unknwn.h>
#include <restrictederrorinfo.h>
#include <hstring.h>
#include <queue>
#include <any>
#include <optional>
#include <stack>
#include <mutex>
#include <vector>
#include <set>
#include <functional>
#include <memory>
#include <limits>
#include <vector>
#include <shlobj.h>
#include <powrprof.h>
#include <cmath>
#include <future>
#include <cstdlib>
#include <sstream>
#include <filesystem>
#include <ctime>
#include <fstream>
#include <algorithm>
#include <random>
#include <dwmapi.h>
#include <uxtheme.h>
#include <atlbase.h>
#include <wincodec.h>
#include <wininet.h>
#include <Rdpencomapi.h>
#include <userenv.h>
#include <winsqlite/winsqlite3.h>
#include <windows.h>
#include <comdef.h>
#include <exdisp.h>
#include <oledlg.h>
#include <comutil.h>

// Undefine GetCurrentTime macro to prevent
// conflict with Storyboard::GetCurrentTime
#undef GetCurrentTime
#undef min
#undef max

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Storage.Streams.h>
#include <winrt/Windows.ApplicationModel.Activation.h>
#include <winrt/Microsoft.UI.Composition.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Input.h>
#include <winrt/Microsoft.UI.Xaml.Input.h>
#include <winrt/Microsoft.UI.Input.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Controls.Primitives.h>
#include <winrt/Microsoft.UI.Xaml.Data.h>
#include <winrt/Microsoft.UI.Xaml.Interop.h>
#include <winrt/Windows.UI.Xaml.Interop.h>
#include <microsoft.ui.xaml.window.h>
#include <winrt/Microsoft.UI.Xaml.Markup.h>
#include <microsoft.ui.xaml.media.dxinterop.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <winrt/Microsoft.Windows.AppNotifications.h>
#include <winrt/Microsoft.Windows.AppNotifications.Builder.h>
#include <winrt/Microsoft.UI.Xaml.Navigation.h>
#include <winrt/Microsoft.UI.Xaml.Shapes.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <winrt/Microsoft.UI.Xaml.Media.Imaging.h>
#include <winrt/Microsoft.UI.Dispatching.h>
#include <wil/cppwinrt_helpers.h>
#include <appmodel.h>
#include <filesystem>
#include "resizer.h"
#include "xml3all.h"
#include "CLI11.hpp"

extern const wchar_t* ttitle;
extern std::shared_ptr<XML3::XML> SettingsX;
extern std::wstring datafolder;

const wchar_t* s(size_t);



inline std::random_device rd;
inline std::mt19937 gen(rd());

// ystring class, wstring <-> string wrapper
class ystring : public std::wstring
{
private:
	mutable std::string asc_str_st;
public:

	// Constructors
	ystring(HWND hh) : std::wstring()
	{
		AssignFromHWND(hh);
	}
	ystring(HWND hh, int ID) : std::wstring()
	{
		AssignFromHWND(GetDlgItem(hh, ID));
	}
	ystring() : std::wstring()
	{
	}
	ystring(const char* v, int CP = CP_UTF8)
	{
		EqChar(v, CP);
	}
	ystring(const std::string& v, int CP = CP_UTF8)
	{
		EqChar(v.c_str(), CP);
	}
	ystring(const std::wstring& f)
	{
		assign(f.c_str());
	}
	ystring(const wchar_t* f)
	{
		if (!f)
			return;
		assign(f);
	}

	ystring& Format(const wchar_t* f, ...)
	{
		va_list args;
		va_start(args, f);

		int len = _vscwprintf(f, args) + 100;
		if (len < 8192)
			len = 8192;
		std::vector<wchar_t> b(len);
		vswprintf_s(b.data(), len, f, args);
		assign(b.data());
		va_end(args);
		return *this;
	}

	// operator =
	void operator=(const char* v)
	{
		EqChar(v);
	}
	void operator=(const wchar_t* v)
	{
		assign(v);
	}
	void operator=(const std::wstring& v)
	{
		assign(v.c_str());
	}
	void operator=(const ystring& v)
	{
		assign(v.c_str());
	}
	void operator=(const std::string& v)
	{
		EqChar(v.c_str());
	}
	CLSID ToCLSID()
	{
		CLSID a;
		CLSIDFromString(c_str(), &a);
		return a;
	}
	void operator=(CLSID cid)
	{
		wchar_t ad[100] = { 0 };
		StringFromGUID2(cid, ad, 100);
		assign(ad);
	}

	operator const wchar_t* ()
	{
		return c_str();
	}

	// asc_str() and a_str() and operator const char*() 
	const std::string& asc_str(int CP = CP_UTF8) const
	{
		const wchar_t* s = c_str();
		int sz = WideCharToMultiByte(CP, 0, s, -1, 0, 0, 0, 0);
		std::vector<char> d(sz + 100);
		WideCharToMultiByte(CP, 0, s, -1, d.data(), sz + 100, 0, 0);
		asc_str_st = d.data();
		return asc_str_st;
	}
	operator const char* () const
	{
		return a_str();
	}
	const char* a_str(int CP = CP_UTF8) const
	{
		asc_str(CP);
		return asc_str_st.c_str();
	}

	long long ll() const
	{
		return atoll(a_str());
	}

	// Internal Convertor
	void EqChar(const char* v, int CP = CP_UTF8)
	{
		clear();
		if (!v)
			return;
		int sz = MultiByteToWideChar(CP, 0, v, -1, 0, 0);
		std::vector<wchar_t> d(sz + 100);
		MultiByteToWideChar(CP, 0, v, -1, d.data(), sz + 100);
		assign(d.data());
	}

	// From HWND
	void AssignFromHWND(HWND hh)
	{
		int zl = GetWindowTextLength(hh);
		std::vector<wchar_t> n(zl + 100);
		GetWindowTextW(hh, n.data(), zl + 100);
		assign(n.data());
	}
};


template <typename T = char>
inline bool LoadFile(const wchar_t* f, std::vector<T>& d)
{
	HANDLE hX = CreateFile(f, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
	if (hX == INVALID_HANDLE_VALUE)
		return false;
	LARGE_INTEGER sz = { 0 };
	GetFileSizeEx(hX, &sz);
	d.resize((size_t)(sz.QuadPart / sizeof(T)));
	DWORD A = 0;
	ReadFile(hX, d.data(), (DWORD)sz.QuadPart, &A, 0);
	CloseHandle(hX);
	if (A != sz.QuadPart)
		return false;
	return true;
}

template <typename T = char>
inline bool PutFile(const wchar_t* f, std::vector<T>& d, bool Fw = true)
{
	HANDLE hX = CreateFile(f, GENERIC_WRITE, 0, 0, Fw ? CREATE_ALWAYS : CREATE_NEW, 0, 0);
	if (hX == INVALID_HANDLE_VALUE)
		return false;
	DWORD A = 0;
	WriteFile(hX, d.data(), (DWORD)(d.size() * sizeof(T)), &A, 0);
	CloseHandle(hX);
	if (A != d.size()*sizeof(T))
		return false;
	return true;
}

inline void Locate(const wchar_t* fi)
{
	ystring y;
	y.Format(L"/select,\"%s\"", fi);
	ShellExecute(0, L"open", L"explorer.exe", y.c_str(), 0, SW_SHOWMAXIMIZED);
}
ystring trim(ystring s);


#include "project.hpp"

#define MY_FET_TAG "PlanXY"

struct PROJECT_AND_RESULT
{
	PROJECT* project = 0;
	XML3::XMLElement* result = 0;
};


namespace sqlite
{
	class query
	{
	private:

		sqlite3* db = 0;
		sqlite3_stmt* stmt = 0;
		std::string q;

	public:

		int Count()
		{
			if (!stmt)
				return 0;
			int n = 0;
			for (;;)
			{
				int st = sqlite3_step(stmt);
				if (st == SQLITE_DONE)
					break;
				if (st == SQLITE_ROW)
				{
					n = sqlite3_column_int(stmt, 0);
				}
			}
			return n;
		}

		bool R()
		{
			if (!stmt)
				return false;
			int st = sqlite3_step(stmt);
			if (st != SQLITE_ROW)
				return false;
			return true;
		}

		int BindText(int idx, const char* t, size_t st)
		{
			if (st == 0)
				st = strlen(t);
			return sqlite3_bind_text(stmt, idx, t, (int)st, 0);
		}


		bool NextRow()
		{
			if (!stmt)
				return false;
			int st = sqlite3_step(stmt);
			if (st != SQLITE_ROW)
				return false;
			return true;
		}

		bool NextRow(std::vector<std::string>& r)
		{
			if (!stmt)
				return false;
			r.clear();
			int st = sqlite3_step(stmt);
			if (st != SQLITE_ROW)
				return false;

			int num_cols = sqlite3_column_count(stmt);
			for (int i = 0; i < num_cols; i++)
			{
				const char* qr = (const char*)sqlite3_column_text(stmt, i);
				if (qr)
					r.push_back(qr);
				else
					r.push_back("");
			}
			return true;
		}

		bool NextRow0(std::map<std::string, std::string>& r)
		{
			if (!stmt)
				return false;
			r.clear();
			int st = sqlite3_step(stmt);
			if (st != SQLITE_ROW)
				return false;

			int num_cols = sqlite3_column_count(stmt);
			for (int i = 0; i < num_cols; i++)
			{
				const char* qr = (const char*)sqlite3_column_text(stmt, i);
				const char* qn = (const char*)sqlite3_column_name(stmt, i);
				if (!qn)
					qn = "";
				if (!qr)
					qr = "";
				r[qn] = qr;
			}
			return true;
		}

		bool NextRow(std::map<std::string, std::wstring>& r)
		{
			if (!stmt)
				return false;
			r.clear();
			int st = sqlite3_step(stmt);
			if (st != SQLITE_ROW)
				return false;

			int num_cols = sqlite3_column_count(stmt);
			for (int i = 0; i < num_cols; i++)
			{
				const char* qr = (const char*)sqlite3_column_text(stmt, i);
				const char* qn = (const char*)sqlite3_column_name(stmt, i);
				if (!qn)
					qn = "";
				if (!qr)
					qr = "";
				r[qn] = XML3::XMLU(qr);
			}
			return true;
		}
		query(sqlite3* d, const char* qq)
		{
			if (qq == 0 || d == 0)
				return;
			db = d;
			q = qq;
			sqlite3_prepare_v2(db, qq, -1, &stmt, NULL);
		}
		~query()
		{
			if (stmt)
				sqlite3_finalize(stmt);
			stmt = 0;
		}

		query(const query& qu)
		{
			q = qu.q;
			db = qu.db;
			sqlite3_prepare_v2(db, q.c_str(), -1, &stmt, NULL);
		}

		operator bool()
		{
			if (stmt)
				return true;
			return false;
		}

	};

	class sqlite
	{
	private:
		sqlite3* db = 0;

	public:


		sqlite3* h() { return db; }

		sqlite(const sqlite&) = delete;
		sqlite& operator=(const sqlite&) = delete;


		auto last()
		{
			return sqlite3_last_insert_rowid(db);
		}

		sqlite(const char* fn)
		{
			if (fn)
				sqlite3_open(fn, &db);
		}

		~sqlite()
		{
			if (db)
				sqlite3_close(db);
			db = 0;
		}

		void One(const char* q)
		{
			query a1(h(), q);
			a1.NextRow();
		}
		void One(const char* q, std::map<std::string, std::wstring>& r)
		{
			query a1(h(), q);
			a1.NextRow(r);
		}

		operator bool()
		{
			if (db)
				return true;
			return false;
		}
	};



};

inline bool ReadOnly = false;

winrt::Windows::Foundation::Collections::IObservableVector<winrt::WuiFET::Item> GetList(XML3::XMLElement& r, const wchar_t* filter = 0, std::vector<std::wstring>* sel = 0, std::function<bool(XML3::XMLElement*)> TestFunction = nullptr,bool GoGroup = false,PROJECT* Act = 0);
void SortList(XML3::XMLElement& r);
ystring trim(ystring s);
ystring uniquestring();
ystring DisplayName(XML3::XMLElement& e,int HTML = 0);
ystring DisplayNameForActivity(PROJECT* p, XML3::XMLElement& ee, unsigned long WhatAdd = 0xFFFFFFFF, int HTML = 0);
winrt::Windows::UI::Color EstimateFGFromBG(winrt::Windows::UI::Color bg);
ystring ColorToString(winrt::Windows::UI::Color br);
winrt::Windows::UI::Color StringToColor(const wchar_t* s);

inline int _ConstraintsMode = 0;
inline int _NewPutGlobalPercNext = 100;

HWND HWNDFromPage(winrt::Windows::Foundation::IInspectable pg);


#pragma warning(disable:4334)
#pragma warning(disable:4127)
#pragma warning(disable:4244)
#pragma warning(disable:4267)
#include "zip.h"

#pragma warning(default:4334)
#pragma warning(default:4244)
#pragma warning(default:4267)
#pragma warning(default:4127)
