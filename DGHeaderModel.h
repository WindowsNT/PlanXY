#pragma once
#include "DGHeaderModel.g.h"

namespace winrt::WuiFET::implementation
{
    struct DGHeaderModel : DGHeaderModelT<DGHeaderModel>
    {
        static winrt::hstring txt(long jx)
        {
            return s(jx);
        }

        DGHeaderModel()
        {

        }
        std::wstring _n1;
        hstring Name()
        {
            return _n1.c_str();
        }

        void Name(hstring n)
        {
            _n1 = n.c_str();
        }

        bool Sel()
        {
            return 0;
        }
        void Sel(bool)
        {
        }

        long long _ptr = 0;
        long long ptr()
        {
            return _ptr;
		}
        void ptr(long long p)
        {
            _ptr = p;
		}


        int _minwidth = 150;

        int MinimumWidth()
        {
            return _minwidth;
        }
        void MinimumWidth(int value)
        {
            if (_minwidth != value)
            {
                _minwidth = value;
                m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{ L"MinimumWidth" });
            }
        }

        winrt::Microsoft::UI::Xaml::Media::Brush ColorX()
        {
            auto col = winrt::Windows::UI::Color();
            col.A = 100;
            col.R = 255;
            if (Sel())
                col.A = 255;
            auto sb = winrt::Microsoft::UI::Xaml::Media::SolidColorBrush();
            sb.Color(col);
            return sb;
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


        void Refresh(winrt::hstring s)
        {
            m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{ s });
		}

    };
}
namespace winrt::WuiFET::factory_implementation
{
    struct DGHeaderModel : DGHeaderModelT<DGHeaderModel, implementation::DGHeaderModel>
    {
    };
}
