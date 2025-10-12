#pragma once

#include "DGDataGrid.g.h"
#include "DGHeaderModel.h"

namespace winrt::WuiFET::implementation
{
    struct DGDataGrid : DGDataGridT<DGDataGrid>
    {
        DGDataGrid()
        {
            m_headers = winrt::single_threaded_observable_vector<winrt::WuiFET::DGHeaderModel>();
            m_rows = winrt::single_threaded_observable_vector<DGRowModel>();

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
        void Refresh()
        {
            m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{ L""});
		}
        void Refresh2(hstring const& str)
        {
            m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{ str });
		}



        winrt::Windows::Foundation::Collections::IObservableVector<winrt::WuiFET::DGHeaderModel> ColumnHeaders()
        {
            return m_headers;
        }
        winrt::Windows::Foundation::Collections::IObservableVector<DGRowModel> ItemsSource()
        {
            return m_rows;
        }

        void ColumnHeaders(winrt::Windows::Foundation::Collections::IObservableVector<winrt::WuiFET::DGHeaderModel> const& value)
        {
            m_headers = value;
            HeaderRepeater().ItemsSource(m_headers);
        }

        void ItemsSource(winrt::Windows::Foundation::Collections::IObservableVector<DGRowModel> const& value)
        {
            m_rows = value;
            RowRepeater().ItemsSource(m_rows);
        }

        void ButtonClick1(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
        {

        }

		

        private:
        winrt::Windows::Foundation::Collections::IObservableVector<winrt::WuiFET::DGHeaderModel> m_headers{ nullptr };
        winrt::Windows::Foundation::Collections::IObservableVector<DGRowModel> m_rows{ nullptr };
    };
}

namespace winrt::WuiFET::factory_implementation
{
    struct DGDataGrid : DGDataGridT<DGDataGrid, implementation::DGDataGrid>
    {
    };
}
