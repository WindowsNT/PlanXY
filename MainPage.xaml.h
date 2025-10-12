#pragma once

#include "MainPage.g.h"

extern std::wstring fil;
winrt::Windows::Foundation::IInspectable WindowFromPage(winrt::Windows::Foundation::IInspectable pg);
std::wstring TempFile();
std::vector<char> Fetch(const char* TheLink);

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


        void OnMSAOpen(IInspectable const&, IInspectable const&)
        {
            int what = 2025;
            bool local = 0;
            ystring d1;
#ifdef _DEBUG
            local = 1;
            d1.Format(L"r:\\msa%i.db",what);
            if (local)
            {
                if (GetFileAttributes(d1) != INVALID_FILE_ATTRIBUTES)
                {
                    int msa2fetmain(const wchar_t* dbname, const wchar_t* targetfet);
                    ystring tempfet;
                    tempfet.Format(L"r:\\msa%i.fet",what);
                    DeleteFile(tempfet.c_str());
                    msa2fetmain(d1, tempfet.c_str());
                    fil = tempfet.c_str();
                    winrt::WuiFET::MainWindow CreateWi();
                    CreateWi();
                    return;
                }
            }
#endif
            auto f1 = Fetch("https://www.msa-apps.com/fetrequest.php");
            bool AskText(HWND hh, const TCHAR * ti, const TCHAR * as, TCHAR * re, std::wstring * re2 = 0, int mxt = 1000);
            std::wstring res;
            if (AskText((HWND)0, L"MSA-Apps Key", L"MSA-Apps Key", 0, &res, 0))
            {
                ystring y;
                y.Format(L"https://www.msa-apps.com/fetrequest.php?what=%i&key=%s", what,res.c_str());
                f1 = Fetch(y.a_str());
                if (f1.size() > 10)
                {
                    auto dbx = TempFile();
                    dbx += L".db";
                    if (local)
                        dbx = d1;
                    PutFile(dbx.c_str(), f1, true);
                    int msa2fetmain(const wchar_t* dbname, const wchar_t* targetfet);
                    ystring tempfet = TempFile();
                    tempfet += L".fet";
                    if (local)
                    {
                        tempfet.Format(L"r:\\msa%i.fet", what);
                    }
                    DeleteFile(tempfet.c_str());
                    msa2fetmain(dbx.c_str(), tempfet.c_str());

                    // Open tempfet
//                    if (local == 0)
//                       DeleteFile(dbx.c_str());
                    fil = tempfet.c_str();
                    winrt::WuiFET::MainWindow CreateWi();
                    CreateWi();

                }
            }
        }


        void OnOpen(IInspectable const&, IInspectable const&)
        {
            OPENFILENAME of = { 0 };
            of.lStructSize = sizeof(of);
            of.hwndOwner = (HWND)0;
            ystring filt;
            filt.Format(L"%s\0*.fet\0\0", s(151));
            of.lpstrFilter = filt.c_str();
            std::vector<wchar_t> fnx(10000);
            of.lpstrFile = fnx.data();
            of.nMaxFile = 10000;
            of.lpstrDefExt = L"fet";
            of.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
            if (!GetOpenFileName(&of))
                return;

            // If we actually opene 2025.db
            if (wcsstr(fnx.data(), L".db"))
            {
                std::wstring TempFile();
                int msa2fetmain(const wchar_t* dbname, const wchar_t* targetfet);
                auto tempfet = TempFile();
				tempfet += L".fet";
				DeleteFile(tempfet.c_str());
				msa2fetmain(fnx.data(), tempfet.c_str());
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

        void OnAbout(IInspectable const&, IInspectable const&)
        {
            MessageBoxW(0, L"Plan XY - a WinUI, FET based timetable generator.\r\n\r\nFET - https://lalescu.ro/liviu/fet/\r\n\r\nPlan XY - https://www.turbo-play.com/planxy", ttitle, MB_OK | MB_ICONINFORMATION);
            return;
        }

        void OnSave(IInspectable const& a1, IInspectable const& a2)
        {
			auto wi = WindowFromPage(*this).try_as<winrt::WuiFET::MainWindow>();
            if (!wi)
                return;
            std::wstring fi = wi.file().c_str();
            if (fi.length() == 0)
            {
				OnSaveAs(a1,a2);
                return;
            }

            // Save it
			if (!project->x)
				return;
            XML3::XMLSerialization x1 = {};
            x1.NoContentIndent = 1;
            project->x->Save(fi.c_str(), &x1);
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
            auto wi = WindowFromPage(*this).try_as<winrt::WuiFET::MainWindow>();
            if (!wi)
                return;
            OPENFILENAME of = { 0 };
            of.lStructSize = sizeof(of);
            of.hwndOwner = (HWND)0;
            ystring filt;
            filt.Format(L"%s\0*.fet\0\0", s(151));
            of.lpstrFilter = filt.c_str();
            std::vector<wchar_t> fnx(10000);
            of.lpstrFile = fnx.data();
			wcscpy_s(fnx.data(), 10000, wi.file().c_str());
            of.nMaxFile = 10000;
            of.lpstrDefExt = L"fet";
            of.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
            if (!GetSaveFileName(&of))
                return;

            DeleteFile(fnx.data());
			wi.file(fnx.data());
			Refresh(L"Loaded_File");
			OnSave(*this, *this);
        }





    };
}

namespace winrt::WuiFET::factory_implementation
{
    struct MainPage : MainPageT<MainPage, implementation::MainPage>
    {
    };
}
