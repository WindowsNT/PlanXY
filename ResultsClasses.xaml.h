#pragma once

#include "ResultsClasses.g.h"
#include "timetable.hpp"
ystring trim(ystring s);
std::wstring TempFile();

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

       
        struct COMBO_ROOM
        {
            std::wstring r;
            std::vector<std::wstring> others;
        };
        std::vector<COMBO_ROOM> combo_rooms;

        void CreateComboRooms()
        {
            // Also put PlanXY combos
            auto x = _ResultP->x;
            auto& my = x->GetRootElement()[MY_FET_TAG];
            auto& myr = my["Data"]["ComboStudents"];
            for (auto& m : myr)
            {
                ystring name = trim(m.FindElementZ("Name", true)->GetContent());
                COMBO_ROOM cr;
                cr.r = name.c_str();
                for (auto& mm : m)
                {
                    if (mm.GetElementName() == "Students")
                    {
                        ystring sub = trim(mm.GetContent());
                        cr.others.push_back(sub.c_str());
                    }
                }
                combo_rooms.push_back(cr);
            }
        }


        FINISHEDTT ftt;





        // WhatX 1: Class
        ystring LeftFilter;
        void OnLeftFilter(IInspectable const&, IInspectable const&)
        {
            LeftFilter = fb11().Text().c_str();
            Refresh(L"Classes_List");
        }

        winrt::Windows::Foundation::Collections::IObservableVector<winrt::WuiFET::Item> Classes_List()
        {
            auto items = winrt::single_threaded_observable_vector<winrt::WuiFET::Item>();
            auto x = _ResultP->x;
            if (!x)
                return items;

            auto& r = x->GetRootElement()["Students_List"];
            for (auto& rr : r)
            {
                ystring name = DisplayName(rr);
                winrt::WuiFET::Item it;
                it.Name1(name.c_str());
                it.ptr((long long)&rr);

                // If Filter
                bool S = 1;
                if (!LeftFilter.empty())
                {
                    if (name.find(LeftFilter) == ystring::npos)
                        S = 0;
				}
                if (S == 1) 
                    items.Append(it);
                S = 1;

                for (auto& rrr : rr)
                {
                    if (rrr.GetElementName() != "Group")
						continue;

                    ystring name2 = DisplayName(rrr);
                    winrt::WuiFET::Item it2;
                    it2.Name1(name2.c_str());
                    it2.ptr((long long)&rrr);
					// If Filter
                    if (!LeftFilter.empty())
                    {
                        if (name2.find(LeftFilter) == ystring::npos)
                            S = 0;
					}
					if (S == 1)
                        items.Append(it2);
					S = 1;

                    for (auto& rrrr : rrr)
                    {
                        if (rrrr.GetElementName() != "Subgroup")
                            continue;

                        ystring name3 = DisplayName(rrrr);
                        winrt::WuiFET::Item it3;
                        it3.Name1(name3.c_str());
                        it3.ptr((long long)&rrrr);
                        // Check filter
                        if (!LeftFilter.empty())
                        {
                            if (name3.find(LeftFilter) == ystring::npos)
                                S = 0;
                        }
						if (S == 1)
                            items.Append(it3);
						S = 1;
                    }

                }

            }


            // Also put PlanXY combos
            auto& my = x->GetRootElement()[MY_FET_TAG];
            auto& myr = my["Data"]["ComboStudents"];
            for (auto& m : myr)
            {
                ystring name3 = trim(m.FindElementZ("Name", true)->GetContent());
                bool S = 1;
                if (!LeftFilter.empty())
                {
                    if (name3.find(LeftFilter) == ystring::npos)
                        S = 0;
                }
                if (S == 1)
                {
                    winrt::WuiFET::Item it3;
                    it3.Name1(name3.c_str());
                    it3.ptr((long long)&m);
					items.Append(it3);
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
            if (ftt.Empty())
            {
                ftt.BuildTimeTableResult(_ResultP, _ResultX);
                CreateComboRooms();
            }

            std::set<ystring> RoomsToSearch;

            auto crx = trim(_SelectedClass->FindElementZ("Name", true)->GetContent());
            RoomsToSearch.insert(crx);
            for (auto& c : combo_rooms)
            {
                if (c.r == crx)
                {
                    for (auto& c2 : c.others)
                    {
                        RoomsToSearch.insert(c2);
                    }
					break;
                }
            }

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
                        for (auto& r1 : ftt.classes)
                        {
                            for (auto& rro : RoomsToSearch)
                            {
                                if (r1.second.dispname != rro)
                                    continue;
                                for (auto& rr1 : r1.second.entries)
                                {
                                    if (rr1.day != days2[column - 1])
                                        continue;
                                    if (rr1.hour != hours2[row])
                                        continue;

                                    for (auto& r4 : rr1.subjects)
                                    {
                                        y += r4.dispname;
                                        y += L"\r\n";
                                    }
                                    for (auto& r4 : rr1.teachers)
                                    {
                                        y += r4.dispname;
                                        y += L"\r\n";
                                    }
                                    for (auto& r4 : rr1.rooms)
                                    {
                                        y += r4.dispname;
                                        y += L"\r\n";
                                    }
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


        void Export1(auto, auto)
        {
            Export0(0);
        }
        void Export2(auto, auto)
        {
            Export0(1);
        }

        void Export0(int mode)
        {
            if (ftt.Empty())
            {
                ftt.BuildTimeTableResult(_ResultP, _ResultX);
                CreateComboRooms();
            }

            auto x = _ResultP->x;
            XML3::XMLElement* project_root = &x->GetRootElement();
            //            XML3::XMLElement* result_root = _ResultX;

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
)", s(55), s(55));
            html += y;

			std::vector<std::wstring> ClassList;
			std::map<std::wstring, std::wstring> NameToLong;


            if (mode == 0 || mode == 2)
            {
                for (auto& rr : ftt.classes)
                {
					ClassList.push_back(rr.second.dispname);
                }
            }

            if (mode == 1 || mode == 2)
            {
                // Combo also
                for (auto& c : combo_rooms)
                {
                    ClassList.push_back(c.r);
                }

            }


            if (1)
            {
                // Create links to all rooms
                int t1 = 1;
                for (auto& name : ClassList)
                {
                    y.Format(LR"(<a class="btn btn-small btn-primary" href="#tea%i" style="margin: 2">%s</a> )", t1, name.c_str());
                    html += y;
                    t1++;
                }
                html += R"(<p style="page-break-after:always;"></p>)";
            }

            int t1 = 1;
            for (auto& name : ClassList)
            {
                y.Format(LR"(<h4 id="tea%i">%s</h4><hr>)", t1, name.c_str());
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
                        ystring shortname = name;
                        for (auto& j : NameToLong)
                        {
                            if (j.second == name)
                            {
                                shortname = j.first;
                                break;
                            }
                        }
                        bool F = 0;

                        if (mode == 1)
                        {
                            html += "<td style=\"text-align:center; vertical-align: middle; ";
                            html += "\">";
                        }

                        for (auto& rm : ftt.classes)
                        {
                            bool Found = 0;
                            if (rm.second.dispname != name && rm.first != shortname)
                            {
                                // search in combo
                                if (mode == 1 || mode == 2)
                                {
                                    for (auto& c : combo_rooms)
                                    {
                                        if (c.r != name && c.r != shortname)
                                            continue;
                                        for (auto& cr2 : c.others)
                                        {
                                            if (rm.second.dispname == cr2)
                                            {
                                                Found = 1;
                                                break;
                                            }
                                        }
                                        if (Found)
                                            break;
                                    }
								}
                            }
                            else
                                Found = 1;
                            if (!Found)
                                continue;
                            for (auto& rr : rm.second.entries)
                            {
                                if (rr.day != days2[day])
                                    continue;
                                if (rr.hour != hours2[hour])
                                    continue;



                                // Add a center text column
                                ystring bg_color;
                                ystring fg_color;

                                bool ThereIsBG = 0;
                                for (auto& s : rr.subjects)
                                {
                                    ystring bg = s.RootInProject ? DisplayName(*s.RootInProject, 2) : ystring(L""); // get bg
                                    if (bg.length())
                                    {
                                        ThereIsBG = 1;
                                        bg_color = bg;
                                    }
                                }
                                for (auto& s : rr.activities)
                                {
                                    ystring bg = s.RootInProject ? DisplayName(*s.RootInProject, 2) : ystring(L""); // get bg
                                    if (bg.length())
                                    {
                                        ThereIsBG = 1;
                                        bg_color = bg;
                                    }
                                }

                                if (ThereIsBG)
                                {
                                    fg_color = ColorToString(EstimateFGFromBG(StringToColor(bg_color.c_str())));
                                }



                                if (mode == 0)
                                {
                                    html += "<td style=\"text-align:center; vertical-align: middle; ";
                                    if (ThereIsBG)
                                    {
                                        html += "background-color: #";
                                        html += bg_color;
                                        html += ";";
                                        html += "color: #";
                                        html += fg_color;
                                        html += ";";
                                    }
                                    html += "\">";
                                }

                                //                        html += "Test";
                                for (auto& t : rr.teachers)
                                {
                                    // Is Tamb? if so, so the kid instead 
                                    if (_wcsnicmp(t.name.c_str(), L"TAMB", 4) != 0)
                                    {
                                        y.Format(LR"(<b>%s</b><br>)", t.dispname.c_str());
                                        html += y;
                                    }
                                    else
                                    {
                                        y.Format(LR"(<b>%s</b><br>)",rm.second.name.c_str());
                                        html += y;

                                    }
                                }
                                for (auto& s : rr.subjects)
                                {
                                    y.Format(LR"(<div><b>%s</b></div>)", s.dispname.c_str());
                                    html += y;
                                }
                                for (auto& s : rr.rooms)
                                {
                                    y.Format(LR"(<div>%s</div>)", s.dispname.c_str());
                                    html += y;
                                }
                                if (mode == 0)
                                    html += "</td>";
                                F = 1;
							}
                        }
                        if (F == 0)
                        {
                            if (mode == 0)
                                html += "<td></td>";
                        }

                        if (mode == 1)
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
    struct ResultsClasses : ResultsClassesT<ResultsClasses, implementation::ResultsClasses>
    {
    };
}
