#pragma once

#include "ResultsClasses.g.h"
ystring trim(ystring s);

namespace winrt::WuiFET::implementation
{
    struct ResultsClasses : ResultsClassesT<ResultsClasses>
    {
        ResultsClasses()
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


        double _Resizer_V1 = 0;
        double _Resizer_X1 = 0;
        double Resizer_V1()
        {
            return _Resizer_V1;
        }
        void Resizer_V1(double value)
        {
            if (_Resizer_V1 != value)
            {
                _Resizer_V1 = value;
                m_propertyChanged(*this, winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"Resizer_V1"));
                m_propertyChanged(*this, winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"Resizer_VV1"));
            }
        }
        Microsoft::UI::Xaml::GridLength Resizer_VV1()
        {
            // Default is "Star"
            if (_Resizer_V1 == 0)
                return Microsoft::UI::Xaml::GridLength{ 1, Microsoft::UI::Xaml::GridUnitType::Star };
            else
            {
                // Get actual length first
                auto top = Content();
                auto grid = top.as<Microsoft::UI::Xaml::FrameworkElement>().FindName(L"TC_SingleClass").as<Microsoft::UI::Xaml::Controls::Grid>();
                if (!grid)
                    return Microsoft::UI::Xaml::GridLength{ 1, Microsoft::UI::Xaml::GridUnitType::Star };

                // Get col 1 actual width
                if (_Resizer_X1 == 0)
                {
                    auto row = grid.ColumnDefinitions().GetAt(0);
                    auto aw = row.ActualWidth();
                    if (aw == 0)
                        return Microsoft::UI::Xaml::GridLength{ 1, Microsoft::UI::Xaml::GridUnitType::Star };
                    _Resizer_X1 = aw;
                }

                return Microsoft::UI::Xaml::GridLength{ _Resizer_X1 + _Resizer_V1, Microsoft::UI::Xaml::GridUnitType::Pixel };
            }
        }


        // WhatX 1: Class
        winrt::Windows::Foundation::Collections::IObservableVector<winrt::WuiFET::Item> Classes_List()
        {
            auto items = winrt::single_threaded_observable_vector<winrt::WuiFET::Item>();
            auto x = _ResultP->x;
            if (!x)
                return items;

            auto& r = x->GetRootElement()["Students_List"];
            for (auto& rr : r)
            {
                std::shared_ptr<XML3::XMLElement> ee = rr.FindElementZ("Name", true);
                ystring name = trim(ee->GetContent());
                winrt::WuiFET::Item it;
                it.Name1(name.c_str());
                it.ptr((long long)&rr);
                items.Append(it);
                for (auto& rrr : rr)
                {
                    if (rrr.GetElementName() != "Group")
						continue;

                    std::shared_ptr<XML3::XMLElement> ee2 = rrr.FindElementZ("Name", true);
                    ystring name2 = trim(ee2->GetContent());
                    winrt::WuiFET::Item it2;
                    it2.Name1(name2.c_str());
                    it2.ptr((long long)&rrr);
                    items.Append(it2);

                    for (auto& rrrr : rrr)
                    {
                        if (rrrr.GetElementName() != "Subgroup")
                            continue;

                        std::shared_ptr<XML3::XMLElement> ee3 = rrrr.FindElementZ("Name", true);
                        ystring name3 = trim(ee3->GetContent());
                        winrt::WuiFET::Item it3;
                        it3.Name1(name3.c_str());
                        it3.ptr((long long)&rrrr);
                        items.Append(it3);
                    }

                }

            }
            return items;
        }
        XML3::XMLElement* _SelectedClass = 0;
        void ClassChanged(IInspectable, IInspectable)
        {
            auto lv = Classes_ListView();
            auto sel = lv.SelectedItem().as<winrt::WuiFET::Item>();
            if (sel && sel.ptr())
                _SelectedClass = (XML3::XMLElement*)sel.ptr();
            Refresh(L"gr1_is");
        }


        // WhatY 1
        winrt::Windows::Foundation::Collections::IObservableVector<winrt::WuiFET::DGHeaderModel> gr1_ch()
        {
            auto headers = winrt::single_threaded_observable_vector<winrt::WuiFET::DGHeaderModel>();
            if (!_ResultP->x)
                return headers;
            auto x = _ResultP->x;
            XML3::XMLElement* root = &x->GetRootElement();
            auto days = root->FindElementZ("Days_List", true);
            winrt::WuiFET::DGHeaderModel it0;
            it0.MinimumWidth(200);
            headers.Append(it0); // empty first 
            for (auto& j : *days)
            {
                if (j.GetElementName() != "Day")
                    continue;
                std::shared_ptr<XML3::XMLElement> ee = j.FindElementZ("Name", true);
                ystring name = trim(ee->GetContent());
                winrt::WuiFET::DGHeaderModel it;
                it.Name(name.c_str());
                it.MinimumWidth(200);
                headers.Append(it);
            }
            return headers;
        }


        winrt::Windows::Foundation::Collections::IObservableVector<DGRowModel> gr1_is()
        {
            auto rows = winrt::single_threaded_observable_vector<DGRowModel>();
            if (!_SelectedClass)
                return rows;

            ystring SelectedClassName = trim(_SelectedClass->FindElementZ("Name", true)->GetContent());
            if (!_ResultP->x)
                return rows;
            auto x = _ResultP->x;
            XML3::XMLElement* root = &x->GetRootElement();
            auto days = root->FindElementZ("Days_List", true);
            auto hours = root->FindElementZ("Hours_List", true);

            std::vector<std::wstring> days2;
            for (auto& j : *days)
            {
                if (j.GetElementName() != "Day")
                    continue;
                days2.push_back(trim(j.FindElementZ("Name", true)->GetContent()).c_str());
            }

            std::vector<std::wstring> hours2;
            for (auto& hou : *hours)
            {
                if (hou.GetElementName() != "Hour")
                    continue;
                hours2.push_back(trim(hou.FindElementZ("Name", true)->GetContent()).c_str());
            }

            for (int row = 0; row < (int)hours2.size(); row++)
            {
                auto cells = winrt::single_threaded_observable_vector<DGCellModel>();

                for (int column = 0; column <= (int)days2.size(); column++)
                {
                    if (column == 0)
                    {
                        DGCellModel c1(row, column, DGCellType::Block, hours2[row]);
                        c1.MinimumWidth(200);
                        cells.Append(c1);
                    }
                    else
                    {
                        ystring y;
                        auto r1 = _ResultX->FindElementZ("Students_Timetable", true);
                        for (auto& rr1 : *r1)
                        {
                            // check name
                            if (rr1.vv("name").GetWideValue() != SelectedClassName)
                                continue;

                            // Check "Day"
                            for (auto& rr2 : rr1)
                            {
                                if (rr2.vv("name").GetWideValue() != days2[column - 1])
                                    continue;

                                // Check "Hour"
                                for (auto& rr3 : rr2)
                                {
                                    if (rr3.vv("name").GetWideValue() != hours2[row])
                                        continue;

                                    // This is it
                                    for (auto& rr4 : rr3)
                                    {
                                        if (rr4.GetElementName() == "Subject")
                                        {
                                            y += trim(rr4.vv("name").GetWideValue());
                                            y += L" ";
                                        }
                                        if (rr4.GetElementName() == "Teacher")
                                        {
                                            y += trim(rr4.vv("name").GetWideValue());
                                            y += L" ";
                                        }

                                    }

                                }
                            }

                        }

                        DGCellModel c1(row, column, DGCellType::Block, y);
                        c1.MinimumWidth(200);
                        cells.Append(c1);

                    }
                }


                DGRowModel rr(row, cells);
                rows.Append(rr);

            }


            return rows;
        }


    };
}

namespace winrt::WuiFET::factory_implementation
{
    struct ResultsClasses : ResultsClassesT<ResultsClasses, implementation::ResultsClasses>
    {
    };
}
