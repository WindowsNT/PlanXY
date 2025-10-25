#pragma once
#include "Item.g.h"

namespace winrt::WuiFET::implementation
{
    struct Item : ItemT<Item>
    {
        static winrt::hstring txt(long jx)
        {
            return s(jx);
        }

        Item()
        {

        }
        std::wstring _n1;
        std::wstring _n2;
        std::wstring _src;
        hstring Name1()
        {
            return _n1.c_str();
        }
        hstring Name2()
        {
            return _n2.c_str();
        }
        hstring Source()
        {
            return _src.c_str();
        }

        void Name1(hstring n)
        {
            _n1 = n.c_str();
        }

        void Name2(hstring n)
        {
            _n2 = n.c_str();
        }
        void Source(hstring n)
        {
            _src = n.c_str();
        }

        bool _sele = 0;
        bool Sel()
        {
            return _sele;
        }
        void Sel(bool v)
        {
			_sele = v;
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

        long long _ptr2 = 0;
        long long _ptr3 = 0;

        long long ptr2() const
        {
            return _ptr2;
        }
        void ptr2(long long value)
        {
            _ptr2 = value;
        }
        long long ptr3() const
        {
            return _ptr3;
        }
        void ptr3(long long value)
        {
            _ptr3 = value;
        }

        long long _ptr4 = 0;
        long long _ptr5 = 0;
        void Ptr4(long long value)
        {
            _ptr4 = value;
        }
        long long Ptr4() const
        {
            return _ptr4;
        }
        void Ptr5(long long value)
        {
            _ptr5 = value;
        }
        long long Ptr5() const
        {
            return _ptr5;
        }

        IInspectable _pi1, _pi2, _pi3;
        IInspectable pi1() const { return _pi1; }
        void pi1(IInspectable const& v) { _pi1 = v; }
        IInspectable pi2() const { return _pi2; }
        void pi2(IInspectable const& v) { _pi2 = v; }
        IInspectable pi3() const { return _pi3; }
        void pi3(IInspectable const& v) { _pi3 = v; }

		int _whatx = 0;
        int WhatX() const
        {
            return _whatx;
        }
        void WhatX(int value)
        {
            if (_whatx != value)
            {
                _whatx = value;
                m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{ L"WhatX" });
            }
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

        winrt::Microsoft::UI::Xaml::Media::SolidColorBrush sb = nullptr;
        void ColorX(winrt::Microsoft::UI::Xaml::Media::Brush br)
        {
			auto sbr = br.try_as<winrt::Microsoft::UI::Xaml::Media::SolidColorBrush>();
            if (sbr)
                sb = sbr;
        }

        winrt::Microsoft::UI::Xaml::Media::Brush ColorX()
        {
            if (sb)
				return sb;
            auto col = winrt::Windows::UI::Color();
            col.A = 255;
            col.R = 0;
            col.G = 0;
            col.B = 0;
            auto ct = SettingsX->GetRootElement().vv("Theme").GetValueInt(0);
            if (ct == 2)
            {
                col.R = 255;
				col.G = 255;
				col.B = 255;
            }

            if (Sel())
            {
                if (ct == 2)
                {
                    col.R = 100;
                    col.G = 100;
                    col.B = 255;
                }
                else
                {
                    col.R = 200;
                    col.G = 0;
                    col.B = 0;
                }

            }


            sb = winrt::Microsoft::UI::Xaml::Media::SolidColorBrush();
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
    struct Item : ItemT<Item, implementation::Item>
    {
    };
}
