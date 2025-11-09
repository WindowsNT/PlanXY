#pragma once

#include "MsaApps.g.h"
winrt::Windows::Foundation::IInspectable WindowFromPage(winrt::Windows::Foundation::IInspectable pg);
std::wstring TempFile();
int msa2fetmain(const wchar_t* dbname, const wchar_t* targetfet, int stype);
#include "fetfile.hpp"
extern std::wstring fil;
winrt::Windows::Foundation::IInspectable WindowFromPage(winrt::Windows::Foundation::IInspectable pg);
std::wstring TempFile();
std::vector<char> Fetch(const char* TheLink);

namespace winrt::WuiFET::implementation
{
    using namespace winrt;
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Controls;
    using namespace Microsoft::UI::Xaml::Input;
    struct MsaApps : MsaAppsT<MsaApps>
    {
        MsaApps()
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
        }
        std::wstring _str1, _str2, _str3;
        void str1(winrt::hstring const& value)
        {
            _str1 = value.c_str();
            Refresh(L"str1");
        }
        winrt::hstring str1()
        {
            return _str1.c_str();
        }
        void str2(winrt::hstring const& value)
        {
            _str2 = value.c_str();
            Refresh(L"str2");
        }
        winrt::hstring str2()
        {
            return _str2.c_str();
        }
        void str3(winrt::hstring const& value)
        {
            _str3 = value.c_str();
            Refresh(L"str3");
        }
        winrt::hstring str3()
        {
            return _str3.c_str();
        }

        void Message([[maybe_unused]] const wchar_t* tit, [[maybe_unused]] const wchar_t* msg)
        {
#ifdef _DEBUG
            MessageBox(0, msg, tit, MB_OK);
#else
            auto t = tt();
            t.PreferredPlacement(TeachingTipPlacementMode::Center);
            str1(tit);
            str2(msg);
            t.IsOpen(true);
#endif
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
            m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{ L"" });
        }
        winrt::event_token PropertyChanged(winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler)
        {
            return m_propertyChanged.add(handler);
        }
        void PropertyChanged(winrt::event_token const& token) noexcept
        {
            m_propertyChanged.remove(token);
        }

        void OnMSAOpen(IInspectable const&, IInspectable const&)
        {
            int what = 2025;
            bool local = 0;
            ystring d1;
#ifdef _DEBUG
            local = 1;
            d1.Format(L"r:\\msa%i.db", what);
            if (local)
            {
                if (GetFileAttributes(d1) != INVALID_FILE_ATTRIBUTES)
                {
                    ystring tempfet;
                    tempfet.Format(L"r:\\msa%i.pxy", what);
                    DeleteFile(tempfet.c_str());
                    msa2fetmain(d1, tempfet.c_str(), 99);
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
                y.Format(L"https://www.msa-apps.com/fetrequest.php?what=%i&key=%s", what, res.c_str());
                f1 = Fetch(y.a_str());
                if (f1.size() > 10)
                {
                    auto dbx = TempFile();
                    dbx += L".db";
                    if (local)
                        dbx = d1;
                    PutFile(dbx.c_str(), f1, true);
                    ystring tempfet = TempFile();
                    tempfet += L".pxy";
                    if (local)
                    {
                        tempfet.Format(L"r:\\msa%i.pxy", what);
                    }
                    DeleteFile(tempfet.c_str());
                    msa2fetmain(dbx.c_str(), tempfet.c_str(), 99);

                    // Open tempfet
//                    if (local == 0)
//                       DeleteFile(dbx.c_str());
                    fil = tempfet.c_str();
                    winrt::WuiFET::MainWindow CreateWi();
                    CreateWi();

                }
            }
        }


    };
}

namespace winrt::WuiFET::factory_implementation
{
    struct MsaApps : MsaAppsT<MsaApps, implementation::MsaApps>
    {
    };
}
