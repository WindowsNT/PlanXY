#pragma once

#include "Settings.g.h"
extern std::map<HWND, winrt::Windows::Foundation::IInspectable> windows;

namespace winrt::WuiFET::implementation
{
    struct Settings : SettingsT<Settings>
    {
        Settings()
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
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

        static winrt::hstring txt(long jx)
        {
            return s(jx);
        }

        void Refresh();


        int WhatLanguage()
        {
            auto cb = LangCB();
            for (unsigned int j = 0; j < cb.Items().Size(); j++)
            {
                auto it = cb.Items().GetAt(j);
                auto item = it.as<winrt::Microsoft::UI::Xaml::Controls::ComboBoxItem>();
                if (!item)
                    continue;
                auto Tag = item.Tag();
                if (!Tag)
                    continue;
                auto tv = _wtoi(unbox_value_or<winrt::hstring>(Tag, L"9").c_str());
                if (tv == SettingsX->GetRootElement().vv("lang").GetValueInt(LANG_ENGLISH))
                {
                    return j;
                }
            }
            return 0;
        }

        void WhatLanguage(int value)
        {
            // Get tag of ComboBoxItem
            auto cb = LangCB();
            auto it = cb.Items().GetAt(value).as< winrt::Microsoft::UI::Xaml::Controls::ComboBoxItem>();
            auto tv = _wtoi(unbox_value_or<winrt::hstring>(it.Tag(), L"9").c_str());
            SettingsX->GetRootElement().vv("lang").SetValueInt(tv);
            SettingsX->Save();

            lang = -1;


            // All windows refresh
            for (auto& w : windows)
            {
                auto w2 = w.second.as<winrt::WuiFET::MainWindow>();
                w2.Refresh();
            }


        }


    };
}

namespace winrt::WuiFET::factory_implementation
{
    struct Settings : SettingsT<Settings, implementation::Settings>
    {
    };
}
