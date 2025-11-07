#pragma once

#include "ResultRooms.g.h"
#include "timetable.hpp"
ystring trim(ystring s);
std::wstring TempFile();

namespace winrt::WuiFET::implementation
{
    struct ResultRooms : ResultRoomsT<ResultRooms>
    {
        ResultRooms()
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
                auto grid = top.as<Microsoft::UI::Xaml::FrameworkElement>().FindName(L"TC_SingleRoom").as<Microsoft::UI::Xaml::Controls::Grid>();
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


       
        FINISHEDTT ftt;

        // WhatX 1: Room
        ystring LeftFilter;
        void OnLeftFilter(IInspectable const&, IInspectable const&)
        {
            LeftFilter = fb11().Text().c_str();
            Refresh(L"Rooms_List");
        }
        winrt::Windows::Foundation::Collections::IObservableVector<winrt::WuiFET::Item> Rooms_List()
        {
            return GetList((_ResultP->x->GetRootElement()["Rooms_List"]), LeftFilter.c_str());
        }
        XML3::XMLElement* _SelectedRoom = 0;
        void RoomChanged(IInspectable, IInspectable)
        {
            auto lv = Rooms_ListView();
            auto sel = lv.SelectedItem().as<winrt::WuiFET::Item>();
            if (sel && sel.ptr())
                _SelectedRoom = (XML3::XMLElement*)sel.ptr();
            Refresh(L"gr1_is");
        }


        // WhatY 1: NotAvailableTimes
        winrt::Windows::Foundation::Collections::IObservableVector<winrt::WuiFET::DGHeaderModel> gr1_ch()
        {
            auto headers = winrt::single_threaded_observable_vector<winrt::WuiFET::DGHeaderModel>();
            if (!_ResultP->x)
                return headers;
            auto x = _ResultP->x;
            XML3::XMLElement* root = &x->GetRootElement();
            auto days = root->FindElementZ("Days_List", true);
            winrt::WuiFET::DGHeaderModel it0;
            headers.Append(it0); // empty first 
            for (auto& j : *days)
            {
                if (j.GetElementName() != "Day")
                    continue;
                std::shared_ptr<XML3::XMLElement> ee = j.FindElementZ("Name", true);
                ystring name = trim(ee->GetContent());
                winrt::WuiFET::DGHeaderModel it;
                it.Name(name.c_str());
                headers.Append(it);
            }
            return headers;
        }


        winrt::Windows::Foundation::Collections::IObservableVector<DGRowModel> gr1_is()
        {
            auto rows = winrt::single_threaded_observable_vector<DGRowModel>();
            if (!_SelectedRoom)
                return rows;
            if (ftt.Empty())
                ftt.BuildTimeTableResult(_ResultP, _ResultX);
            ystring SelectedRoomName = trim(_SelectedRoom->FindElementZ("Name", true)->GetContent());
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
                        cells.Append(c1);
                    }
                    else
                    {
                        // TT per student
                        ystring y;
                        for(auto& r1 : ftt.rooms[SelectedRoomName].entries)
                        {
                            if (r1.day == days2[column - 1] && r1.hour == hours2[row])
                            {
                                for (auto& j : r1.subjects)
                                {
                                    y += j.dispname;
                                    y += L"\r\n";
                                }
                                for (auto& j : r1.classes)
                                {
                                    y += j.dispname;
                                    y += L"\r\n";
                                }
                                for (auto& j : r1.teachers)
                                {
                                    y += j.dispname;
                                    y += L"\r\n";
                                }

                                break;
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


            ly1 = 1;
            return rows;
        }

        bool ly1 = 0;
        void gr1_ly(IInspectable, IInspectable)
        {
            if (ly1)
            {
                gr1().AutoMeasure(1, 1);
                ly1 = 0;
            }

        }


        void Export1(IInspectable, IInspectable)
        {
            if (ftt.Empty())
                ftt.BuildTimeTableResult(_ResultP, _ResultX);
            auto x = _ResultP->x;
            XML3::XMLElement* project_root = &x->GetRootElement();

            // Fill days/hours 
            auto days = project_root->FindElementZ("Days_List", true);
            auto hours = project_root->FindElementZ("Hours_List", true);
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

            // Create HTML
            std::string html;
            ystring y;

            // Header
            y.Format(LR"(
<html>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <script src="https://cdnjs.cloudflare.com/ajax/libs/jquery/3.7.1/jquery.min.js" integrity="sha512-v2CJ7UaYy4JwqLDIrZUI/4hqeoQieOmAZNXBeQyjo21dadnwR+8ZaIJVT8EE2iyI61OV8e6M8PP2/4hpQINQ/g==" crossorigin="anonymous" referrerpolicy="no-referrer"></script>
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.3/dist/css/bootstrap.min.css" rel="stylesheet">
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.3.3/dist/js/bootstrap.bundle.min.js"></script>
    <link rel="preconnect" href="https://fonts.googleapis.com">
   <link href="https://fonts.googleapis.com/css2?family=Noto+Serif:ital,wght@0,100..900;1,100..900&display=swap" rel="stylesheet">
   <title>%s</title>
   <div style="margin: 20px;">
   <h2>%s</h2>  
)", s(51),s(51));
            html += y;

            if (1)
            {
                // Create links to all rooms
                int t1 = 1;
                for (auto& rr : ftt.rooms)
                {
                    y.Format(LR"(<a class="btn btn-small btn-primary" href="#tea%i" style="margin: 2">%s</a> )", t1, rr.second.dispname.c_str());
                    html += y;
                    t1++;
                }
                html += R"(<p style="page-break-after:always;"></p>)";
            }

            int t1 = 1;
            for (auto& rr : ftt.rooms)
            {
                y.Format(LR"(<h4 id="tea%i">%s</h4><hr>)", t1, rr.second.dispname.c_str());
                t1++;
                html += y;

                // Create a bootstrap table 
                html += R"(<table class="table table-bordered table-striped" style="width:100%; font-family: 'Noto Serif', serif;">)";
                html += R"(<thead><th></th>)";

                // the days
                for (auto& d : days2)
                {
                    y.Format(LR"(<th>%s</th>)", d.c_str());
                    html += y;
                }
                html += R"(</thead><tbody>)";
                // the hours
                for (size_t hour = 0; hour < hours2.size(); hour++)
                {
                    html += "<tr>";
                    y.Format(LR"(<th>%s</th>)", hours2[hour].c_str());
                    html += y;

                    for (size_t day = 0; day < days2.size(); day++)
                    {
                        // Add a center text column
                        html += "<td style=\"text-align:center; vertical-align: middle;\">";

//                        html += "Test";

                        for(auto& rm : ftt.rooms[rr.first].entries)
                        {
                            if (rm.day == days2[day] && rm.hour == hours2[hour])
                            {
                                for (auto& t : rm.teachers)
                                {
                                    y.Format(LR"(<b>%s</b><br>)", t.dispname.c_str());
                                    html += y;
                                }
                                for (auto& s : rm.subjects)
                                {
                                    y.Format(LR"(<div><b>%s</b></div>)", s.dispname.c_str());
                                    html += y;
                                }
                                for (auto& s : rm.classes)
                                {
                                    y.Format(LR"(<div>%s</div>)", s.dispname.c_str());
                                    html += y;
                                }
                                break;
                            }
						}
                        html += "</td>";
                    }
                    html += "</tr>";

                }
                html += R"(</tbody></table>)";

                html += R"(<p style="page-break-after:always;"></p>)";
            }

            auto tf = TempFile();
            tf += L".html";
            std::vector<char> d;
            d.resize(html.size());
            memcpy(d.data(), html.data(), html.size());
            PutFile(tf.c_str(), d);
            ShellExecute(0, L"open", tf.c_str(), 0, 0, SW_SHOWNORMAL);

        }
    };
}

namespace winrt::WuiFET::factory_implementation
{
    struct ResultRooms : ResultRoomsT<ResultRooms, implementation::ResultRooms>
    {
    };
}
