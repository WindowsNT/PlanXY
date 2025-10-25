#pragma once
#include "DGRowModel.g.h"
#include "DGCellModel.g.h"

namespace winrt::WuiFET::implementation
{
    struct DGRowModel : DGRowModelT<DGRowModel>
    {
        DGRowModel(int row,winrt::Windows::Foundation::Collections::IObservableVector<DGCellModel> const& cells)
        {
			m_row = row;
            m_cells = cells;
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
            m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{ L"" });
        }
        void Refresh2(hstring const& str)
        {
            m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{ str });
        }


        winrt::Windows::Foundation::Collections::IObservableVector<DGCellModel> Cells() const
        {
            return m_cells;
        }
    private:
        winrt::Windows::Foundation::Collections::IObservableVector<DGCellModel> m_cells{ nullptr };
		int m_row = 0;
    };
};

namespace winrt::WuiFET::factory_implementation
{
    struct DGRowModel : DGRowModelT<DGRowModel, implementation::DGRowModel>
    {
    };
}
