#pragma once

#include "ViewResult.g.h"
#include "timetable.hpp"

namespace winrt::WuiFET::implementation
{
    using namespace winrt;
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Controls;

    struct ViewResult : ViewResultT<ViewResult>
    {

        ViewResult()
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
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


        void OnLoad(IInspectable, IInspectable)
        {
            auto topnv = Content().as<NavigationView>();
            if (topnv)
            {
                Frame fr = topnv.FindName(L"contentFrame").as<Frame>();
                if (fr)
                {
					PROJECT_AND_RESULT* par = new PROJECT_AND_RESULT();
					par->project = _ResultP;
					par->result = _ResultX;
                    fr.Navigate(winrt::xaml_typename<winrt::WuiFET::ResultsSummary>(), winrt::box_value<long long>((long long)par));
                }
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
            auto it = ar.InvokedItemContainer().as<NavigationViewItem>();
            if (!it)
                return;
            auto n = it.Name();

            PROJECT_AND_RESULT* par = new PROJECT_AND_RESULT();
            par->result = _ResultX;
            par->project = _ResultP;

            if (n == L"Summary")
                fr.Navigate(winrt::xaml_typename<winrt::WuiFET::ResultsSummary>(), winrt::box_value<long long>((long long)par));
            if (n == L"Teachers")
                fr.Navigate(winrt::xaml_typename<winrt::WuiFET::ResultsTeachers>(), winrt::box_value<long long>((long long)par));
			if (n == L"Classes")
				fr.Navigate(winrt::xaml_typename<winrt::WuiFET::ResultsClasses>(), winrt::box_value<long long>((long long)par));
			if (n == L"Kids")
				fr.Navigate(winrt::xaml_typename<winrt::WuiFET::ResultsKids>(), winrt::box_value<long long>((long long)par));
			if (n == L"Parents")
				fr.Navigate(winrt::xaml_typename<winrt::WuiFET::ResultsParents>(), winrt::box_value<long long>((long long)par));
			if (n == L"Rooms")
				fr.Navigate(winrt::xaml_typename<winrt::WuiFET::ResultRooms>(), winrt::box_value<long long>((long long)par));
            if (n == L"Statistics")
				fr.Navigate(winrt::xaml_typename<winrt::WuiFET::Statistics>(), winrt::box_value<long long>((long long)par));

        }

    };
}

namespace winrt::WuiFET::factory_implementation
{
    struct ViewResult : ViewResultT<ViewResult, implementation::ViewResult>
    {
    };
}
