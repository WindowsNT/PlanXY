#pragma once

#include "MainPage.g.h"
extern wchar_t MainDirectory[MAX_PATH];
#include "fetfile.hpp"
extern std::wstring fil;
winrt::Windows::Foundation::IInspectable WindowFromPage(winrt::Windows::Foundation::IInspectable pg);
std::wstring TempFile();
std::vector<char> Fetch(const char* TheLink);
namespace fs = std::filesystem;
#include "version.h"
namespace winrt::WuiFET::implementation
{
    using namespace winrt;
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Controls;

    struct MainPage : MainPageT<MainPage>
    {
        MainPage()
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
        }

        PROJECT* project = 0;
        void OnNavigatedTo(winrt::Microsoft::UI::Xaml::Navigation::NavigationEventArgs const& e)
        {
            if (e.Parameter())
            {
                auto param = e.Parameter().as<long long>();
				project = (PROJECT*)param;
            }
        }

        winrt::hstring TheTitle()
        {
            ystring y;
			y.Format(L"%s - %s", s(1),ystring(VERSION).c_str());
			return winrt::hstring(y.c_str());
        }
        winrt::hstring CurrentVersion()
        {
			return winrt::hstring(ystring(VERSION).c_str());
        }

        bool IsGreek()
        {
            return lang == LANG_GREEK;
        }

        winrt::event_token PropertyChanged(winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler)
        {
            return m_propertyChanged.add(handler);
        }
        void PropertyChanged(winrt::event_token const& token) noexcept
        {
            m_propertyChanged.remove(token);
        }
        static winrt::hstring txt(long jx)
        {
            return s(jx);
        }

        winrt::event<Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> m_propertyChanged;
        
        void Refresh(const wchar_t* s)
        {
            m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{ s });
        }

        void Refresh()
        {
            m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{ L""});
        }


        void OnNew(IInspectable const&, IInspectable const&)
        {
            winrt::WuiFET::MainWindow CreateWi();
            CreateWi();
        }




        void OnOpen(IInspectable const&, IInspectable const&)
        {
            OPENFILENAME of = { 0 };
            of.lStructSize = sizeof(of);
            of.hwndOwner = (HWND)0;
            wchar_t filt[100] = {};
            swprintf_s(filt, 100, L"%s\0*.pxy;*.fet;*.db\0\0", s(127));
            of.lpstrFilter = filt;
            std::vector<wchar_t> fnx(10000);
            of.lpstrFile = fnx.data();
            of.nMaxFile = 10000;
            of.lpstrDefExt = L"pxy";
            of.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
            if (!GetOpenFileName(&of))
                return;

            // If we actually opene 2025.db
            if (wcsstr(fnx.data(), L".db"))
            {
                std::wstring TempFile();
                auto tempfet = TempFile();
				tempfet += L".pxy";
				DeleteFile(tempfet.c_str());
				msa2fetmain(fnx.data(), tempfet.c_str(),99,0);
				wcscpy_s(fnx.data(), 10000, tempfet.c_str());
			}


            if (project->file.empty())
            {
                auto wi = WindowFromPage(*this).try_as<winrt::WuiFET::MainWindow>();
                if (!wi)
                    return;
				wi.file(fnx.data());
                // Navigate to Data
				auto topnv = wi.Content().as<NavigationView>();
                if (topnv)
                {
                    auto fr = topnv.FindName(L"contentFrame").as<winrt::Microsoft::UI::Xaml::Controls::Frame>();
                    fr.Navigate(winrt::xaml_typename<winrt::WuiFET::DataPage>(), winrt::box_value<long long>((long long)project));
                }
            }
            else
            {
                fil = fnx.data();
                winrt::WuiFET::MainWindow CreateWi();
                CreateWi();
            }
        }

        winrt::hstring Loaded_File()
        {
            auto wi = WindowFromPage(*this).try_as<winrt::WuiFET::MainWindow>();
            if (!wi)
                return L"";
            return wi.file();
		}

        void OnShare(IInspectable const&, IInspectable const&)
        {
//            void ShareScreen(int f);
//			std::thread thr(ShareScreen, 1);
//			thr.detach();
            std::wstring hmnexe = MainDirectory;
            hmnexe += L"\\hmn.exe";
            if (GetFileAttributes(hmnexe.c_str()) != INVALID_FILE_ATTRIBUTES)
            {
                ShellExecute(0, L"open", hmnexe.c_str(), L"-q -s 7001 --upnp", 0, SW_SHOWNORMAL);
                return;
            }
		}

        void OnAbout(IInspectable const&, IInspectable const&)
        {
            MessageBoxW(HWNDFromPage(*this), L"Plan XY - a WinUI, FET based timetable generator.\r\n\r\nFET - https://lalescu.ro/liviu/fet/\r\n\r\nPlan XY - https://www.turbo-play.com/planxy", ttitle, MB_OK | MB_ICONINFORMATION);
            return;
        }

        void OnSave(IInspectable const& , IInspectable const& )
        {
            project->Save(false);
            Refresh(L"Loaded_File");
        }
        void OnExit(IInspectable const&, IInspectable const&)
        {
            auto wi = WindowFromPage(*this).try_as<winrt::WuiFET::MainWindow>();
            if (!wi)
                return;
            wi.Close();
        }
        void OnSaveAs(IInspectable const&, IInspectable const&)
        {
            project->Save(true);
			Refresh(L"Loaded_File");
        }
        void OnExportFET(IInspectable const&, IInspectable const&)
        {
            OPENFILENAME of = { 0 };
            of.lStructSize = sizeof(of);
            of.hwndOwner = (HWND)0;
            wchar_t filt[100] = {};
            swprintf_s(filt, 100, L"FET\0*.fet\0\0");
            of.lpstrFilter = filt;
            std::vector<wchar_t> fnx(10000);
            of.lpstrFile = fnx.data();
            of.nMaxFile = 10000;
            of.lpstrDefExt = L"fet";
            of.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
            if (!GetSaveFileName(&of))
                return;

			DeleteFile(fnx.data());
            XML3::XML x4;
			x4.SetRootElement(project->x->GetRootElement());
            void MigrateToFET(XML3::XML * x);
            MigrateToFET(&x4);
            x4.Save(fnx.data());
			Locate(fnx.data());
        }



        void PageLoaded(IInspectable const&, RoutedEventArgs const&)
        {
            auto mr = ExampleMenuRoot();
            std::wstring GetExamplesDirectory();
            auto LoopsDir = GetExamplesDirectory();


            // Load the examples
            try
            {
				std::map<std::wstring, MenuFlyoutSubItem> subs;
                for (const auto& entry : fs::recursive_directory_iterator(LoopsDir))
                {
                    auto& f = entry.path();

                    if (f.extension() != L".fet" && f.extension() != L".pxy")
						continue;

					// Remove base dir
                    std::wstring display = f.wstring().substr(LoopsDir.length() + 1);


                    // split to 
                    auto j = split(display.c_str(), L'\\');
                    if (j.size() != 2)
                        continue;

                    if (subs.find(j[0]) == subs.end())
                    {
                        MenuFlyoutSubItem sub;
						sub.Text(j[0].c_str());
						subs[j[0]] = sub;
						mr.Items().Append(sub);
                    }

                    MenuFlyoutItem item;
                    // remove .fet extension from j[1]
					j[1] = j[1].substr(0, j[1].length() - 4);
                    item.Text(j[1].c_str());
                    item.Tag(winrt::box_value(f.wstring().c_str()));
                    item.Click([&](IInspectable const& it3, RoutedEventArgs const&)
                        {
                            auto it = it3.as<MenuFlyoutItem>().Tag();
                            std::wstring fn = winrt::unbox_value<winrt::hstring>(it).c_str();
                            fil = fn.c_str();
                            winrt::WuiFET::MainWindow CreateWi();

                            if (project->file.empty())
                            {
                                auto wi = WindowFromPage(*this).try_as<winrt::WuiFET::MainWindow>();
                                if (!wi)
                                    return;
                                wi.file(fn.c_str());
                                // Navigate to Data
                                auto topnv = wi.Content().as<NavigationView>();
                                if (topnv)
                                {
                                    auto fr = topnv.FindName(L"contentFrame").as<winrt::Microsoft::UI::Xaml::Controls::Frame>();
                                    fr.Navigate(winrt::xaml_typename<winrt::WuiFET::DataPage>(), winrt::box_value<long long>((long long)project));
                                }
                            }
                            else
                                CreateWi();
                        });
					subs[j[0]].Items().Append(item);
                }
            }
            catch (...)
            {
            }
		}   


    };
}

namespace winrt::WuiFET::factory_implementation
{
    struct MainPage : MainPageT<MainPage, implementation::MainPage>
    {
    };
}
