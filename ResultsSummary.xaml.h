#pragma once

#include "ResultsSummary.g.h"

namespace winrt::WuiFET::implementation
{
    struct ResultsSummary : ResultsSummaryT<ResultsSummary>
    {
        ResultsSummary()
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
        }
        void OnNavigatedTo(winrt::Microsoft::UI::Xaml::Navigation::NavigationEventArgs const& e)
        {
            if (e.Parameter())
            {
                PROJECT_AND_RESULT* param = (PROJECT_AND_RESULT*)e.Parameter().as<long long>();
                _ResultX = param->result;
				_ResultP = param->project;
                delete param;
            }
        }

        static winrt::hstring txt(long jx)
        {
            return s(jx);
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

        XML3::XMLElement* _ResultX = 0;
        long long ResultX()
        {
            return (long long)_ResultX;
        }
        void ResultX(long long x)
        {
            _ResultX = (XML3::XMLElement*)x;
        }
        PROJECT* _ResultP = 0;
        long long ResultP()
        {
            return (long long)_ResultP;
        }
        void ResultP(long long x)
        {
            _ResultP = (PROJECT*)x;
        }

        void OpenFolder(IInspectable const&, IInspectable const&)
        {
//            ShellExecute(0, L"open", _folder.c_str(), 0, 0, SW_SHOW);
		}

        winrt::hstring str1()
        {
			auto& rr = *_ResultX;
            ULARGE_INTEGER ul = {};
            ul.QuadPart = rr.vv("ts").GetValueULongLong();
            SYSTEMTIME st = {};
            FILETIME ft = {};
            ft.dwLowDateTime = ul.LowPart;
            ft.dwHighDateTime = ul.HighPart;
            FileTimeToSystemTime(&ft, &st);
            wchar_t t[1000] = {};
            swprintf_s(t, 1000, L"%04d-%02d-%02d %02d:%02d:%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond
            );
			return winrt::hstring(t);
        }
        winrt::hstring str2()
        {
            auto& rr = *_ResultX;
            long long seconds_finished = rr.vv("finishseconds").GetValueLongLong();
           
            wchar_t t[1000] = {};
            swprintf_s(t, 1000, L"%02zi:%02zi:%02zi", 
                seconds_finished / 3600, (seconds_finished % 3600) / 60, (seconds_finished % 60)
            );
            return winrt::hstring(t);
        }

        winrt::hstring str3()
        {
            auto& rr = *_ResultX;
            wchar_t t[1000] = {};
            swprintf_s(t, 1000, L"%llu %llu %llu %llu %llu %llu",
                rr.vv("seed10").GetValueULongLong(),
                rr.vv("seed11").GetValueULongLong(),
                rr.vv("seed12").GetValueULongLong(),
                rr.vv("seed20").GetValueULongLong(),
                rr.vv("seed21").GetValueULongLong(),
                rr.vv("seed22").GetValueULongLong()
            );
            return winrt::hstring(t);
        }
    };
}

namespace winrt::WuiFET::factory_implementation
{
    struct ResultsSummary : ResultsSummaryT<ResultsSummary, implementation::ResultsSummary>
    {
    };
}
