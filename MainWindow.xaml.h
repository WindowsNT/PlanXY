#pragma once

#include "MainWindow.g.h"
#include "DataPage.h"
#include "fetfile.hpp"
extern std::map<HWND, winrt::Windows::Foundation::IInspectable> windows;
std::vector<char> Fetch(const char* TheLink);
extern std::wstring fil;
std::wstring TempFile();
extern int _ConstraintsMode;
int msa2fetmain(const wchar_t* dbname, const wchar_t* targetfet,int stype);

namespace winrt::WuiFET::implementation
{
	using namespace winrt;
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Controls;

	struct MainWindow : MainWindowT<MainWindow>
    {
		std::shared_ptr<PROJECT> project;

		std::stack<std::string> undos;
		std::stack<std::string> redos;

		bool IsGreek()
		{
			if (lang == -1)
				s(1);
			return lang == LANG_GREEK;
		}

		void Dirty()
		{
			if (project)
				project->Dirty = true;
		}


		void PushUndo()
		{
			if (!project->x)
				return;
			std::string t;
			t = project->x->Serialize();
			undos.push(t);
			Dirty();
		}
		void PushRedo()
		{
			if (!project->x)
				return;
			std::string t;
			t = project->x->Serialize();
			redos.push(t);
		}
		void Undo()
		{
			if (undos.empty())
				return;
			auto top = undos.top();
			undos.pop();
			if (project->x)
			{
				PushRedo();
				project->x->Parse(top.c_str(), top.length());
			}
		}
		void Redo()
		{
			if (redos.empty())
				return;
			auto top = redos.top();
			redos.pop();
			if (project->x)
			{
				PushUndo();
				project->x->Parse(top.c_str(), top.length());
			}
		}

		bool Closing()
		{
			if (!project)
				return true;
			if (project->Dirty == false)
				return true;
			int res = MessageBoxW((HWND)wnd(), s(190), ttitle, MB_YESNOCANCEL | MB_ICONQUESTION);
			if (res == IDCANCEL)
				return false;

			if (res == IDYES)
				{
				if (!project->Save(false))
					return false;
				}
			return true;
		}

        MainWindow()
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
			project = std::make_shared<PROJECT>();


        }

		winrt::event_token PropertyChanged(winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler)
		{
			return m_propertyChanged.add(handler);
		}
		void PropertyChanged(winrt::event_token const& token) noexcept
		{
			m_propertyChanged.remove(token);
		}
		winrt::event<Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> m_propertyChanged;
		void Refresh(const wchar_t* s)
		{
			m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{ s });
		}
		void Refresh()
		{
			m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{ L"" });
		}

        static winrt::hstring txt(long jx)
        {
            return s(jx);
        }


        long long _wnd = 0;
        long long wnd()
        {
            return _wnd;
        }
        void wnd(long long value)
        {
            _wnd = value;
        }

		long long ptrx()
		{
			return (long long)project.get();
		}

        winrt::hstring file()
        {
			return project->file.c_str();
		}
        void file(winrt::hstring const& value)
        {
			project->file = value.c_str();
			if (GetFileAttributes(project->file.c_str()) == INVALID_FILE_ATTRIBUTES)
			{
				if (!project->x)
				{
					std::vector<char> ee;
					if (lang != LANG_GREEK)
						ExtractResource(GetModuleHandle(0), L"DEFAULTFET", L"DATA", ee);
					project->x = std::make_shared<XML3::XML>();
					project->x->Parse(ee.data(), ee.size());

				}
				return;
			}

			if (!project->file.empty())
			{
				// Is this .db ?
				if (_wcsicmp(project->file.c_str() + project->file.length() - 3, L".db") == 0)
				{
					// Convert to fet
					ystring tempfet;
					tempfet.Format(L"r:\\msa2025.pxy");
					DeleteFile(tempfet.c_str());
					msa2fetmain(project->file.c_str(), tempfet.c_str(),99);
					fil = tempfet.c_str();
					project->file = fil;
				}
				project->x = std::make_shared<XML3::XML>(project->file.c_str());
				void MigrateFetFile(XML3::XML * x);
				MigrateFetFile(project->x.get());
				project->HasActiveDetachData();
			}
			else
			{
				// Create the defauklt
				std::vector<char> ee;
				ExtractResource(GetModuleHandle(0),L"DEFAULTFET", L"DATA", ee);
				project->x = std::make_shared<XML3::XML>();
				project->x->Parse(ee.data(), ee.size());
			}
		}






		void ItemInvoked(IInspectable, NavigationViewItemInvokedEventArgs ar)
		{
			auto topnv = Content().as<NavigationView>();
			if (!topnv)
				return;
			Frame fr = topnv.FindName(L"contentFrame").as<Frame>();
			if (!fr)
				return;
			if (ar.IsSettingsInvoked())
			{
				fr.Navigate(winrt::xaml_typename<winrt::WuiFET::Settings>());
				return;
			}
			auto it = ar.InvokedItemContainer().as<NavigationViewItem>();
			if (!it)
				return;
			auto n = it.Name();
			if (n == L"ViewMain")
				fr.Navigate(winrt::xaml_typename<winrt::WuiFET::MainPage>(),winrt::box_value<long long>((long long)project.get()));
			if (n == L"ViewData")
			{
				if (project && project->HasActiveDetachData())
					MessageBox((HWND)wnd(), s(236), ttitle, MB_ICONWARNING);
				else
				fr.Navigate(winrt::xaml_typename<winrt::WuiFET::DataPage>(), winrt::box_value<long long>((long long)project.get()));
			}
			if (n == L"MySchool")
			{
				if (project && project->HasActiveDetachData())
					MessageBox((HWND)wnd(), s(236), ttitle, MB_ICONWARNING);
				else
					fr.Navigate(winrt::xaml_typename<winrt::WuiFET::MySchool>(), winrt::box_value<long long>((long long)project.get()));
			}
			if (n == L"MsaApps")
			{
				if (project && project->HasActiveDetachData())
					MessageBox((HWND)wnd(), s(236), ttitle, MB_ICONWARNING);
				else
					fr.Navigate(winrt::xaml_typename<winrt::WuiFET::MsaApps>(), winrt::box_value<long long>((long long)project.get()));
			}
			_ConstraintsMode = 0;
			if (n == L"ViewTime")
			{
				if (project && project->HasActiveDetachData())
					MessageBox((HWND)wnd(), s(236), ttitle, MB_ICONWARNING);
				else
					fr.Navigate(winrt::xaml_typename<winrt::WuiFET::Constraints>(), winrt::box_value<long long>((long long)project.get()));
			}
			if (n == L"ViewSpace")
			{
				if (project && project->HasActiveDetachData())
					MessageBox((HWND)wnd(), s(236), ttitle, MB_ICONWARNING);
				else
				{
					_ConstraintsMode = 1;
					fr.Navigate(winrt::xaml_typename<winrt::WuiFET::Constraints>(), winrt::box_value<long long>((long long)project.get()));
				}
			}
			if (n == L"ViewExtra")
			{
				if (project && project->HasActiveDetachData())
					MessageBox((HWND)wnd(), s(236), ttitle, MB_ICONWARNING);
				else
				{
					_ConstraintsMode = 2;
					fr.Navigate(winrt::xaml_typename<winrt::WuiFET::Constraints>(), winrt::box_value<long long>((long long)project.get()));
				}
			}
			if (n == L"ViewStatistics")
			{
				PROJECT_AND_RESULT* par = new PROJECT_AND_RESULT();
				par->result = 0;
				par->project = project.get();
				fr.Navigate(winrt::xaml_typename<winrt::WuiFET::Statistics>(), winrt::box_value<long long>((long long)par));
			}
			if (n == L"ViewGenerate")
				fr.Navigate(winrt::xaml_typename<winrt::WuiFET::GeneratorPage>(), winrt::box_value<long long>((long long)project.get()));
			if (n == L"ResultList")
				fr.Navigate(winrt::xaml_typename<winrt::WuiFET::ResultList>(), winrt::box_value<long long>((long long)project.get()));
		}




		void NVI_ChangeLightTapped(IInspectable const&, IInspectable const&)
		{
			auto ct = SettingsX->GetRootElement().vv("Theme").GetValueInt(0);
			if (ct == 1 || ct == 0)
				SettingsX->GetRootElement().vv("Theme").SetValueInt(2);
			else
				SettingsX->GetRootElement().vv("Theme").SetValueInt(1);
			SettingsX->Save();
			void PostThemeChange();
			PostThemeChange();
		}


		void OnLoad(IInspectable, IInspectable)
		{
			auto topnv = Content().as<NavigationView>();
			if (topnv)
			{

				Frame fr = topnv.FindName(L"contentFrame").as<Frame>();
				if (fr)
				{
					if ((project && project->file.length() && !project->HasActiveDetachData()) || windows.size() > 1)
					{
						fr.Navigate(winrt::xaml_typename<winrt::WuiFET::DataPage>(), winrt::box_value<long long>((long long)project.get()));
					}
					else
					{
						fr.Navigate(winrt::xaml_typename<winrt::WuiFET::MainPage>(), winrt::box_value<long long>((long long)project.get()));
					}
				}
				/*
				topnv.KeyDown([this](IInspectable const&, Microsoft::UI::Xaml::Input::KeyRoutedEventArgs const& a)
					{
						auto topnv = Content().as<NavigationView>();
						if (topnv)
						{
							Frame fr = topnv.FindName(L"contentFrame").as<Frame>();
							if (fr)
							{
								auto co = fr.Content().try_as<winrt::VisualDML::MLGraph>();
								if (co)
								{
									co.Key((long long)a.Key(), false);
								}
							}
						}
					});
					*/

			}
		}

    };
}

namespace winrt::WuiFET::factory_implementation
{
    struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow>
    {
    };
}
