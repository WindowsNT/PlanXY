#pragma once

#include "Constraints.g.h"
#include "constraintclass.hpp"
inline int _ConstraintsMode = 0;
ystring trim(ystring s);
winrt::Windows::Foundation::IInspectable WindowFromPage(winrt::Windows::Foundation::IInspectable pg);

namespace winrt::WuiFET::implementation
{
    struct Constraints : ConstraintsT<Constraints>
    {
        Constraints()
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
        }

        PROJECT* project = 0;
        void OnNavigatedTo(winrt::Microsoft::UI::Xaml::Navigation::NavigationEventArgs const& e)
        {
            if (e.Parameter())
            {
                auto param = e.Parameter().as<long long>();
                project = (PROJECT*)param;

                if (_ConstraintsMode == 0)
                    str1(s(19));
                if (_ConstraintsMode == 1)
                    str1(s(20));
            }
        }
        void Refresh(const wchar_t* s)
        {
            m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{ s });
        }
        void Refresh()
        {
            m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{ L"" });
        }

        winrt::event_token PropertyChanged(winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler)
        {
            return m_propertyChanged.add(handler);
        }
        void PropertyChanged(winrt::event_token const& token) noexcept
        {
            m_propertyChanged.remove(token);
        }
        static winrt::hstring txt(long jx)
        {
            return s(jx);
        }
        winrt::event<Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> m_propertyChanged;
        int LeftMode = 0; // 1 Teachers, 2 Classes, 3 Lessons
        XML3::XMLElement* SelectedLeft = 0;
        XML3::XMLElement* SelectedLeft2 = 0;
        ystring LeftFilter;
        void OnLeftFilter(IInspectable const&, IInspectable const&)
        {
            LeftFilter = fb11().Text().c_str();
            Refresh(L"Left_List");
        }

        winrt::Windows::Foundation::Collections::IObservableVector<winrt::WuiFET::Item> Left_List()
        {
            auto items = winrt::single_threaded_observable_vector<winrt::WuiFET::Item>();
            auto x = project->x;
            if (LeftMode == 1)
            {
                auto& r = x->GetRootElement()["Teachers_List"];
                for (auto& rr : r)
                {
                    std::shared_ptr<XML3::XMLElement> ee = rr.FindElementZ("Name", true);
                    ystring name = trim(ee->GetContent());
                    winrt::WuiFET::Item it;
                    it.Name1(name.c_str());
                    it.ptr((long long)&rr);

                    if (LeftFilter.length())
                    {
                        // Must be in name
                        auto name21 = name;
                        auto filter2 = LeftFilter;
                        std::transform(name21.begin(), name21.end(), name21.begin(), towlower);
                        std::transform(filter2.begin(), filter2.end(), filter2.begin(), towlower);
                        if (name21.find(filter2.c_str()) == ystring::npos)
                            continue;
                    }
                    items.Append(it);
                }
            }
            if (LeftMode == 3) // Subjects
            {
                auto& r = x->GetRootElement()["Subjects_List"];
                for (auto& rr : r)
                {
                    std::shared_ptr<XML3::XMLElement> ee = rr.FindElementZ("Name", true);
                    ystring name = trim(ee->GetContent());
                    winrt::WuiFET::Item it;
                    it.Name1(name.c_str());
                    it.ptr((long long)&rr);

                    if (LeftFilter.length())
                    {
                        // Must be in name
                        auto name21 = name;
                        auto filter2 = LeftFilter;
                        std::transform(name21.begin(), name21.end(), name21.begin(), towlower);
                        std::transform(filter2.begin(), filter2.end(), filter2.begin(), towlower);
                        if (name21.find(filter2.c_str()) == ystring::npos)
                            continue;
                    }
                    items.Append(it);
                }
            }
            if (LeftMode == 2)
            {
                auto& r = x->GetRootElement()["Students_List"];
                bool S1 = 0;
                for (auto& rr : r)
                {
                    std::shared_ptr<XML3::XMLElement> ee1 = rr.FindElementZ("Name", true);
                    ystring name1 = trim(ee1->GetContent());
                    winrt::WuiFET::Item it1;
                    it1.Name1(name1.c_str());
                    it1.ptr((long long)&rr);
                    if (LeftFilter.length())
                    {
                        // Must be in name
                        auto name21 = name1;
                        auto filter2 = LeftFilter;
                        std::transform(name21.begin(), name21.end(), name21.begin(), towlower);
                        std::transform(filter2.begin(), filter2.end(), filter2.begin(), towlower);
                        if (name21.find(filter2.c_str()) == ystring::npos)
                            S1 = 1;
                    }
                    if (S1 == 0)
                        items.Append(it1);
                    S1 = 0;
                    for (auto& rr2 : rr)
                    {
                        if (rr2.GetElementName() != "Group")
                            continue;

                        std::shared_ptr<XML3::XMLElement> ee2 = rr2.FindElementZ("Name", true);
                        ystring name2 = trim(ee2->GetContent());
                        winrt::WuiFET::Item it2;
                        it2.Name1(name2.c_str());
                        it2.ptr((long long)&rr2);
                        if (LeftFilter.length())
                        {
                            // Must be in name
                            auto name21 = name2;
                            auto filter2 = LeftFilter;
                            std::transform(name21.begin(), name21.end(), name21.begin(), towlower);
                            std::transform(filter2.begin(), filter2.end(), filter2.begin(), towlower);
                            if (name21.find(filter2.c_str()) == ystring::npos)
                                S1 = 1;
                        }
                        if (S1 == 0)
                            items.Append(it2);
                        S1 = 0;
                        for (auto& rr3 : rr2)
                        {
                            if (rr3.GetElementName() != "Subgroup")
                                continue;

                            std::shared_ptr<XML3::XMLElement> ee3 = rr3.FindElementZ("Name", true);
                            ystring name3 = trim(ee3->GetContent());
                            winrt::WuiFET::Item it3;
                            it3.Name1(name3.c_str());
                            it3.ptr((long long)&rr3);
                            if (LeftFilter.length())
                            {
                                // Must be in name
                                auto name21 = name3;
                                auto filter2 = LeftFilter;
                                std::transform(name21.begin(), name21.end(), name21.begin(), towlower);
                                std::transform(filter2.begin(), filter2.end(), filter2.begin(), towlower);
                                if (name21.find(filter2.c_str()) == ystring::npos)
                                    S1 = 1;
                            }
                            if (S1 == 0)
                                items.Append(it3);
                            S1 = 0;
                        }
                    }
                }
            }
            return items;
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
                auto grid = top.as<Microsoft::UI::Xaml::FrameworkElement>().FindName(L"C_Left").as<Microsoft::UI::Xaml::Controls::Grid>();
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

        int HowManyDays()
        {
            auto x = project->x;
            XML3::XMLElement* root = &x->GetRootElement();
            auto days = root->FindElementZ("Days_List", true);
            int n = 0;
            for (auto& j : *days)
            {
                if (j.GetElementName() != "Day")
                    continue;
                n++;
            }
            return n;
        }
        int HowManyHours()
        {
            auto x = project->x;
            XML3::XMLElement* root = &x->GetRootElement();
            auto days = root->FindElementZ("Hours_List", true);
            int n = 0;
            for (auto& j : *days)
            {
                if (j.GetElementName() != "Hour")
                    continue;
                n++;
            }
            return n;
        }

		std::wstring _str1, _str2, _str3, _str4;
		// Str1 : Time Constraints or Space Constraints
        // Str2 : Info bar at right
        winrt::hstring str1()
        {
            return winrt::hstring(_str1);
        }
        void str1(winrt::hstring const& value)
        {
            auto s = value.c_str();
            if (_str1 != s)
            {
                _str1 = s;
                m_propertyChanged(*this, winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"str1"));
            }
        }
        winrt::hstring str2()
        {
            if (ViewingConstraint && ViewingConstraint->d0.length())
				_str2 = ViewingConstraint->d0;
            return winrt::hstring(_str2);
        }
        void str2(winrt::hstring const& value)
        {
            auto s = value.c_str();
            if (_str2 != s)
            {
                _str2 = s;
                m_propertyChanged(*this, winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"str2"));
            }
        }
        winrt::hstring str3()
        {
            return winrt::hstring(_str3);
        }
        void str3(winrt::hstring const& value)
        {
            auto s = value.c_str();
            if (_str3 != s)
            {
                _str3 = s;
                m_propertyChanged(*this, winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"str3"));
            }
        }
        winrt::hstring str4()
        {
            return winrt::hstring(_str4);
        }
        void str4(winrt::hstring const& value)
        {
            auto s = value.c_str();
            if (_str4 != s)
            {
                _str4 = s;
                m_propertyChanged(*this, winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"str4"));
            }
		}

        int ConstraintsMode()
        {
            return _ConstraintsMode;
        }
        bool IsTS()
        {
            return _ConstraintsMode == 0;
        }
        bool IsSS()
        {
            return _ConstraintsMode == 1;
        }

        int _LeftVisible = 0;
        bool IsLeftVisible()
        {
            return _LeftVisible;
		}
		int _RightVisible = 0;
        bool IsRightVisible()
        {
            return _RightVisible;
		}
        bool _IsActiveVisible = 0;
        bool IsActiveVisible()
        {
            return _IsActiveVisible;
		}
		bool _IsActiveChecked = false;
        bool IsActiveChecked()
        {
            return _IsActiveChecked;
		}
        void IsActiveChecked(bool value)
        {
            if (_IsActiveChecked != value)
            {
                _IsActiveChecked = value;
                if (!_IsActiveChecked)
                {
                    // We remove the constraint 
                    if (SelectedLeft2)
                    {
						auto par = SelectedLeft2->GetParent(&project->x->GetRootElement());
                        if (par)
                        {
                            auto wi = WindowFromPage(*this).as<winrt::WuiFET::MainWindow>();
                            wi.Dirty();
                            par->RemoveElement(SelectedLeft2);
                            SelectedLeft2 = nullptr;
                            _IsActiveChecked = false;
                            _Percentage = 100;
                            RightRefresh();
                        }
                    }
                }
                else
                {
                    // We add it
                    if (ViewingConstraint)
                    {
                        auto x = project->x;
                        XML3::XMLElement* root = &x->GetRootElement();
                        auto r0 = root->FindElementZ(_ConstraintsMode == 1 ? "Space_Constraints_List" : "Time_Constraints_List", true);
						auto& newel = r0->AddElement(ViewingConstraint->x0.c_str());
                        newel.AddElement("Active").SetContent("True");
                        newel.AddElement("Weight_Percentage").SetContent("100");
                        SelectedLeft2 = &newel;
                        if (SelectedLeft && ViewingConstraint->SubType != 0)
                        {
                            const char* s2 = "Teacher";
                            if (ViewingConstraint->SubType == 2)
                                s2 = "Students";
                            if (ViewingConstraint->SubType == 3)
                                s2 = "Room";
                            if (ViewingConstraint->SubType == 4)
                                s2 = "Subject";
                            newel.AddElement(s2).SetContent(trim(SelectedLeft->FindElementZ("Name", true)->GetContent()));
                        }
                        // And the parameters
                        for (auto& param : ViewingConstraint->params)
                        {
                            newel.AddElement(param.x1.c_str()).SetContent(std::to_string(param.def).c_str());
						}
                        Refresh(L"RightList");
                        Refresh(L"IsGridRightVisible");
                        Refresh(L"zz1_is");
                        Refresh(L"zz1_ch");
                    }

                }
           }
		}

		bool _IsMultiple = false;
        bool IsMultiple()
        {
            return _IsMultiple;
        }

		bool _IsPercentageVisible = false;
        bool IsPercentageVisible()
        {
            return _IsPercentageVisible;
		}


		int _Percentage = 0;
        int Percentage()
        {
            return _Percentage;
        }
        void Percentage(int value)
        {
            if (_Percentage != value)
            {
                _Percentage = value;
            }
		}

        void LoadSelectedLeft()
        {
			_IsActiveChecked = false;
            if (SelectedLeft2)
            {
                _IsActiveChecked = 1;
				_Percentage = _wtoi(trim(SelectedLeft2->FindElementZ("Weight_Percentage", true)->GetContent()).c_str());
                Refresh(L"IsActiveChecked");
                Refresh(L"Percentage");
            }

        }

        void NoLeftInit()
        {
            auto x = project->x;
            XML3::XMLElement* root = &x->GetRootElement();
            const char* s1 = "Time_Constraints_List";
            if (ViewingConstraint->Type == 1)
                s1 = "Space_Constraints_List";
            auto r0 = root->FindElementZ(s1, true);
            _Percentage = 100;
            _IsActiveChecked = false;
            for (auto& r1 : *r0)
            {
                if (r1.GetElementName() != ViewingConstraint->x0)
                    continue;

                // Is Active?
                if (r1.FindElementZ("Active", true))
                {
                    auto ac = trim(r1.FindElementZ("Active", true)->GetContent());
                    if (ac == L"1" || ac == L"True" || ac == L"true")
                        _IsActiveChecked = 1;
                }
                int p = 100;
                p = _wtoi(trim(r1.FindElementZ("Weight_Percentage", true)->GetContent()).c_str());
                _Percentage = p;
                SelectedLeft2 = &r1;
                break;
            }
            _RightVisible = 1;
            RightRefresh();
        }

        void LeftChanged(IInspectable, IInspectable)
        {
            auto lv = Left_ListView();
            auto sel = lv.SelectedItem().as<winrt::WuiFET::Item>();
            SelectedLeft = 0;
            if (sel && sel.ptr())
                SelectedLeft = (XML3::XMLElement*)sel.ptr();
            SelectedLeft2 = nullptr;
            _IsActiveChecked = false;
            _Percentage = 100;
            if (SelectedLeft)
            {
                _RightVisible = 1;
                
                // Find the entry in the XML
                auto x = project->x;
                XML3::XMLElement* root = &x->GetRootElement();
                auto r0 = root->FindElementZ(_ConstraintsMode == 1 ? "Space_Constraints_List" : "Time_Constraints_List", true);
                for (auto& c0 : *r0)
                {
                    if (!ViewingConstraint)
						break;
                    if (c0.GetElementName() != ViewingConstraint->x0)
						continue;
                    std::string SearchElement1 = "";
                    std::string SearchElement2 = "";
                    if (LeftMode == 1)
                    {
                        SearchElement1 = "Teacher_Name";
                        SearchElement2 = "Teacher";
                    }
					auto elf = c0.FindElementZ(SearchElement1.c_str());
                    if (!elf)
                        elf = c0.FindElementZ(SearchElement2.c_str());
                    if (!elf)
                        continue;
                    auto tn = trim(elf->GetContent());
                    auto seln = trim(SelectedLeft->FindElementZ("Name", true)->GetContent());
                    if (tn != seln)
                        continue;
                    SelectedLeft2 = &c0; 
					LoadSelectedLeft();
					break;
                }
            }
            else
				_RightVisible = 0;
            RightRefresh();
        }

        void RightRefresh()
        {
            Refresh(L"IsRightVisible");
            Refresh(L"IsActiveVisible");
            Refresh(L"IsActiveChecked");
            Refresh(L"IsMultiple");
            Refresh(L"IsPercentageVisible");
            Refresh(L"IsGridRightVisible");
            Refresh(L"RightList");
            Refresh(L"z1_ch");
            Refresh(L"zz1_is");

        }

        void zz1_Click(IInspectable, IInspectable)
        {

        }

		bool _IsGridRightVisible = false;
        bool IsGridRightVisible()
        {
            return _IsGridRightVisible && _IsActiveChecked;
        }
		bool _IsListRightVisible = false;
        bool IsListRightVisible()
        {
            return _IsListRightVisible;
		}

        winrt::Windows::Foundation::Collections::IObservableVector<winrt::WuiFET::DGHeaderModel> zz1_ch()
        {
            auto headers = winrt::single_threaded_observable_vector<winrt::WuiFET::DGHeaderModel>();
            return headers;
        }

        winrt::Windows::Foundation::Collections::IObservableVector<DGRowModel> zz1_is()
        {
            auto rows = winrt::single_threaded_observable_vector<DGRowModel>();
            if (!project->x)
                return rows;
            if (!ViewingConstraint)
                return rows;
            for (size_t row = 0; row < ViewingConstraint->params.size(); row++)
            {
                auto& param = ViewingConstraint->params[row];
                auto cells = winrt::single_threaded_observable_vector<DGCellModel>();

                // Name 
                DGCellModel c1((int)row, 0, DGCellType::Block, param.d1);
                c1.Ptr((long long)&param);
                cells.Append(c1);

                // Value
                if (param.type == 0 || param.type == 1)
                {
                    DGCellModel c2((int)row, 1, param.type == 1 ? DGCellType::Check : DGCellType::Number, L"");
                    // Params: 

                    c2.WhatX(11);
                    c2.Ptr((long long)&param);
                    c2.Ptr3((long long)ViewingConstraint.get());
                    c2.pi2(WindowFromPage(*this));

                    c2.ValueNMin(param.from);
                    c2.ValueN(param.def);
                    if (param.type == 1)
                        c2.ValueBB(param.def);

                    // If exists
                    if (SelectedLeft2)
                    {
                        auto el = SelectedLeft2->FindElementZ(param.x1.c_str(), true);
                        c2.Ptr2((long long)el.get());
                        auto cont = trim(el->GetContent());
                        c2.ValueN(_wtoi(cont.c_str()));
                        if (param.type == 1)
                        {
                            bool T = cont == L"1" || cont == L"True" || cont == L"true";
                            c2.ValueN(T);
                            c2.ValueBB(T);
                        }
                    }

                    c2.ValueNMax(param.to);
                    cells.Append(c2);
                }
                DGRowModel rr((int)row, cells);
                rows.Append(rr);
            }
            return rows;
        }

        winrt::Windows::Foundation::Collections::IObservableVector<winrt::WuiFET::Item> Right_List()
        {
            auto items = winrt::single_threaded_observable_vector<winrt::WuiFET::Item>();
            return items;
        }


        std::shared_ptr<A_CONSTRAINT> ViewingConstraint;

        // Time + Teacher
        void TS_Teacher_NotAvailableTimes(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            LeftMode = 1; // Teachers
            _LeftVisible = 1;
            _IsActiveVisible = false;
            _IsMultiple = true;
            _IsPercentageVisible = true;
            _IsGridRightVisible = true;
            _IsListRightVisible = false;
            Refresh();
        }

        void TS_Teacher_Single(std::wstring d0,std::string x0,std::string x1,int from,int to,int def,bool R = 1)
        {
            LeftMode = 1; // Teachers
            _LeftVisible = 1;
            _IsActiveVisible = true;
            _IsMultiple = false;
            _IsPercentageVisible = false;
            _RightVisible = false;
            _IsGridRightVisible = true;
            _IsListRightVisible = false;

            ViewingConstraint = std::make_shared<A_CONSTRAINT>();
            ViewingConstraint->d0 = d0;
            ViewingConstraint->x0 = x0;
            ViewingConstraint->Type = 0;
            ViewingConstraint->SubType = 1;
            ViewingConstraint->HasTeacher = 1;
            ViewingConstraint->w_from = 100;
            ViewingConstraint->w_to = 100;
            ViewingConstraint->SupportsMultiple = false;

            if (1)
            {
                CONSTRAINT_PARAM p1;
                p1.d1 = d0;
                p1.type = 0;
                p1.x1 = x1;
                p1.from = from;
                p1.to = to;
                p1.def = def;
                ViewingConstraint->params.push_back(p1);
            }

            if (R)
                Refresh();
        }

        void TS_Teacher_MaxGapsDay(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
			TS_Teacher_Single(s(96), "ConstraintTeacherMaxGapsPerDay", "Max_Gaps", 0, HowManyHours(), HowManyHours());

        }
        void TS_Teacher_MinHoursDay(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
			TS_Teacher_Single(s(104), "ConstraintTeacherMinHoursDaily", "Minimum_Hours_Daily", 1, HowManyHours(), HowManyHours());
        }
        void TS_Teacher_MaxHoursDay(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
			TS_Teacher_Single(s(99), "ConstraintTeacherMaxHoursDaily", "Maximum_Hours_Daily", 1, HowManyHours(), HowManyHours());
        }
        void TS_Teacher_MaxSpanDay(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
			TS_Teacher_Single(s(102), "ConstraintTeacherMaxSpanPerDay", "Max_Span", 1, HowManyHours(), HowManyHours(),0);
            CONSTRAINT_PARAM p2;
            p2.d1 = s(103);
            p2.type = 1;
            p2.x1 = "Allow_One_Day_Exception_of_Plus_One";
            p2.from = 0;
            p2.to = 1;
            p2.def = 0;
            ViewingConstraint->params.push_back(p2);
            Refresh();
        }
        void TS_Teacher_MaxContinuousHours(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            TS_Teacher_Single(s(105), "ConstraintTeacherMaxHoursContinuously", "Maximum_Hours_Continuously", 1, HowManyHours(), HowManyHours());

        }
        void TS_Teacher_MinDaysWeek(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            TS_Teacher_Single(s(93), "ConstraintTeacherMinDaysPerWeek", "Minimum_Days_Per_Week", 1, HowManyDays(), HowManyDays());

        }
        void TS_Teacher_MaxDaysWeek(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            TS_Teacher_Single(s(94), "ConstraintTeacherMaxDaysPerWeek", "Max_Days_Per_Week", 1, HowManyDays(), HowManyDays());

        }
        void TS_Teacher_MaxGapsWeek(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            TS_Teacher_Single(s(97), "ConstraintTeacherMaxGapsPerWeek", "Max_Gaps", 1, HowManyHours() * HowManyDays(), HowManyHours() * HowManyDays());

        }


        // Time + Teachers
        void TS_Teachers_Single(std::wstring d0, std::string x0, std::string x1, int from, int to, int def, bool R = 1)
        {
            LeftMode = 0; // None
            _LeftVisible = 0;
            _IsActiveVisible = true;
            _IsMultiple = false;
            _IsPercentageVisible = false;
            _RightVisible = false;
            _IsGridRightVisible = true;
            _IsListRightVisible = false;

            ViewingConstraint = std::make_shared<A_CONSTRAINT>();
            ViewingConstraint->d0 = d0;
            ViewingConstraint->x0 = x0;
            ViewingConstraint->Type = 0;
            ViewingConstraint->SubType = 0;
            ViewingConstraint->w_from = 100;
            ViewingConstraint->w_to = 100;
            ViewingConstraint->SupportsMultiple = false;

            if (1)
            {
                CONSTRAINT_PARAM p1;
                p1.d1 = d0;
                p1.type = 0;
                p1.x1 = x1;
                p1.from = from;
                p1.to = to;
                p1.def = def;
                ViewingConstraint->params.push_back(p1);
            }

            NoLeftInit();

            if (R)
                Refresh();
        }

        void TS_Teachers_MaxGapsDay(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            TS_Teachers_Single(s(96), "ConstraintTeachesrMaxGapsPerDay", "Max_Gaps", 0, HowManyHours(), HowManyHours());
        }
        void TS_Teachers_MinHoursDay(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            TS_Teachers_Single(s(104), "ConstraintTeachersMinHoursDaily", "Minimum_Hours_Daily", 1, HowManyHours(), HowManyHours());
        }
        void TS_Teachers_MaxHoursDay(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            TS_Teachers_Single(s(99), "ConstraintTeachersMaxHoursDaily", "Maximum_Hours_Daily", 1, HowManyHours(), HowManyHours());
        }
        void TS_Teachers_MaxSpanDay(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            TS_Teachers_Single(s(102), "ConstraintTeachersMaxSpanPerDay", "Max_Span", 1, HowManyHours(), HowManyHours(), 0);
            CONSTRAINT_PARAM p2;
            p2.d1 = s(103);
            p2.type = 1;
            p2.x1 = "Allow_One_Day_Exception_of_Plus_One";
            p2.from = 0;
            p2.to = 1;
            p2.def = 0;
            ViewingConstraint->params.push_back(p2);
            Refresh();

        }
        void TS_Teachers_MaxContinuousHours(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            TS_Teachers_Single(s(105), "ConstraintTeachersMaxHoursContinuously", "Maximum_Hours_Continuously", 1, HowManyHours(), HowManyHours());
        }
        void TS_Teachers_MinDaysWeek(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            TS_Teachers_Single(s(93), "ConstraintTeachersMinDaysPerWeek", "Minimum_Days_Per_Week", 1, HowManyDays(), HowManyDays());

        }
        void TS_Teachers_MaxDaysWeek(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            TS_Teachers_Single(s(94), "ConstraintTeachersMaxDaysPerWeek", "Max_Days_Per_Week", 1, HowManyDays(), HowManyDays());

        }
        void TS_Teachers_MaxGapsWeek(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            TS_Teachers_Single(s(96), "ConstraintTeachersMaxGapsPerWeek", "Max_Gaps", 0, HowManyHours() * HowManyDays(), HowManyHours() * HowManyDays());

        }









        // Space + Teacher
        void SS_Teacher_Single(std::wstring d0, std::string x0, std::string x1, int from, int to, int def, bool R = 1)
        {
            LeftMode = 1; // Teachers
            _LeftVisible = 1;
            _IsActiveVisible = true;
            _IsMultiple = false;
            _IsPercentageVisible = false;
            _RightVisible = false;
            _IsGridRightVisible = true;
            _IsListRightVisible = false;

            ViewingConstraint = std::make_shared<A_CONSTRAINT>();
            ViewingConstraint->d0 = d0;
            ViewingConstraint->x0 = x0;
            ViewingConstraint->Type = 1;
            ViewingConstraint->SubType = 1;
            ViewingConstraint->HasTeacher = 1;
            ViewingConstraint->w_from = 100;
            ViewingConstraint->w_to = 100;
            ViewingConstraint->SupportsMultiple = false;

            if (1)
            {
                CONSTRAINT_PARAM p1;
                p1.d1 = d0;
                p1.type = 0;
                p1.x1 = x1;
                p1.from = from;
                p1.to = to;
                p1.def = def;
                ViewingConstraint->params.push_back(p1);
            }

            if (R)
                Refresh();
        }
        void SS_Teacher_MaxRoomChangesDay(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            SS_Teacher_Single(s(139), "ConstraintTeacherMaxRoomChangesPerDay", "Max_Room_Changes_Per_Day", 1, HowManyHours(), HowManyHours());

        }
        void SS_Teacher_MaxRoomChangesDayInterval(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            SS_Teacher_Single(s(141), "ConstraintTeacherMaxRoomChangesPerDayInInterval", "Max_Room_Changes_Per_Day", 1, HowManyHours(), HowManyHours(),0);
            if (1)
            {
                CONSTRAINT_PARAM p1;
                p1.d1 = s(141);
                p1.type = 0;
                p1.x1 = "Interval_Start_Hour";
                p1.from = 0;
                p1.to = HowManyHours();
                p1.def = 0;
                ViewingConstraint->params.push_back(p1);
            }

            if (1)
            {
                CONSTRAINT_PARAM p1;
                p1.d1 = s(142);
                p1.type = 0;
                p1.x1 = "Interval_End_Hour";
                p1.from = 0;
                p1.to = HowManyHours();
                p1.def = p1.to;
                ViewingConstraint->params.push_back(p1);
            }
            Refresh();
        }
        void SS_Teacher_MaxBuildingChangesDay(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            SS_Teacher_Single(s(143), "ConstraintTeacherMaxBuildingChangesPerDay", "Max_Building_Changes_Per_Day", 1, HowManyHours(), HowManyHours());

        }
        void SS_Teacher_MaxBuildingChangesDayInterval(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            SS_Teacher_Single(s(143), "ConstraintTeacherMaxBuildingChangesPerDayInInterval", "Max_Building_Changes_Per_Day", 1, HowManyHours(), HowManyHours());
            if (1)
            {
                CONSTRAINT_PARAM p1;
                p1.d1 = s(141);
                p1.type = 0;
                p1.x1 = "Interval_Start_Hour";
                p1.from = 0;
                p1.to = HowManyHours();
                p1.def = 0;
                ViewingConstraint->params.push_back(p1);
            }

            if (1)
            {
                CONSTRAINT_PARAM p1;
                p1.d1 = s(142);
                p1.type = 0;
                p1.x1 = "Interval_End_Hour";
                p1.from = 0;
                p1.to = HowManyHours();
                p1.def = p1.to;
                ViewingConstraint->params.push_back(p1);
            }
            Refresh();
        }
        void SS_Teacher_MaxRoomChangesWeek(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            SS_Teacher_Single(s(139), "ConstraintTeacherMaxRoomChangesPerWeek", "Max_Room_Changes_Per_Week", 1, HowManyHours(), HowManyHours());

        }
        void SS_Teacher_MaxBuildingChangesWeek(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            SS_Teacher_Single(s(139), "ConstraintTeacherMaxBuildingChangesPerWeek", "Max_Building_Changes_Per_Week", 1, HowManyHours(), HowManyHours());

        }


		// Space + Teachers
        void SS_Teachers_Single(std::wstring d0, std::string x0, std::string x1, int from, int to, int def, bool R = 1)
        {
            LeftMode = 0; // None
            _LeftVisible = 0;
            _IsActiveVisible = true;
            _IsMultiple = false;
            _IsPercentageVisible = false;
            _RightVisible = false;
            _IsGridRightVisible = true;
            _IsListRightVisible = false;

            ViewingConstraint = std::make_shared<A_CONSTRAINT>();
            ViewingConstraint->d0 = d0;
            ViewingConstraint->x0 = x0;
            ViewingConstraint->Type = 1;
            ViewingConstraint->SubType = 0;
            ViewingConstraint->w_from = 100;
            ViewingConstraint->w_to = 100;
            ViewingConstraint->SupportsMultiple = false;

            if (1)
            {
                CONSTRAINT_PARAM p1;
                p1.d1 = d0;
                p1.type = 0;
                p1.x1 = x1;
                p1.from = from;
                p1.to = to;
                p1.def = def;
                ViewingConstraint->params.push_back(p1);
            }

            NoLeftInit();

            if (R)
                Refresh();
        }

        void SS_Teachers_MaxRoomChangesDay(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            SS_Teachers_Single(s(139), "ConstraintTeachersMaxRoomChangesPerDay", "Max_Room_Changes_Per_Day", 1, HowManyHours(), HowManyHours());

        }
        void SS_Teachers_MaxRoomChangesWeek(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            SS_Teachers_Single(s(139), "ConstraintTeachersMaxRoomChangesPerWeek", "Max_Room_Changes_Per_Week", 1, HowManyHours(), HowManyHours());

        }

        // Space + Class
        void SS_Class_Single(std::wstring d0, std::string x0, std::string x1, int from, int to, int def, bool R = 1)
        {
            LeftMode = 2; // Class
            _LeftVisible = 1;
            _IsActiveVisible = true;
            _IsMultiple = false;
            _IsPercentageVisible = false;
            _RightVisible = false;
            _IsGridRightVisible = true;
            _IsListRightVisible = false;

            ViewingConstraint = std::make_shared<A_CONSTRAINT>();
            ViewingConstraint->d0 = d0;
            ViewingConstraint->x0 = x0;
            ViewingConstraint->Type = 1;
            ViewingConstraint->SubType = 2;
            ViewingConstraint->HasClass = 1;
            ViewingConstraint->w_from = 100;
            ViewingConstraint->w_to = 100;
            ViewingConstraint->SupportsMultiple = false;

            if (1)
            {
                CONSTRAINT_PARAM p1;
                p1.d1 = d0;
                p1.type = 0;
                p1.x1 = x1;
                p1.from = from;
                p1.to = to;
                p1.def = def;
                ViewingConstraint->params.push_back(p1);
            }

            if (R)
                Refresh();
        }

        void SS_Class_MaxRoomChangesDay(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
          SS_Class_Single(s(139), "ConstraintStudentSetMaxRoomChangesPerDay", "Max_Room_Changes_Per_Day", 1, HowManyHours(), HowManyHours());
        }
        void SS_Class_MaxRoomChangesWeek(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            SS_Class_Single(s(139), "ConstraintStudentSetMaxRoomChangesPerWeek", "Max_Room_Changes_Per_Week", 1, HowManyHours(), HowManyHours());
        }


		// Space + Subject
        void SS_Lesson_PreferredRoom(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            LeftMode = 3; // Lesson
            ViewingConstraint = nullptr;
            _LeftVisible = 1;
            _IsActiveVisible = true;
            _IsMultiple = true;
            _IsPercentageVisible = true;
            _RightVisible = false;
            _IsGridRightVisible = false;
            _IsListRightVisible = true;

            Refresh();
        }
        void SS_Lesson_PreferredRooms(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            LeftMode = 3; // Lesson
            ViewingConstraint = nullptr;
            _LeftVisible = 1;
            _IsActiveVisible = true;
            _IsMultiple = true;
            _IsPercentageVisible = true;
            _RightVisible = false;
            _IsGridRightVisible = true;
            _IsListRightVisible = false;

            Refresh();

        }


    };
}

namespace winrt::WuiFET::factory_implementation
{
    struct Constraints : ConstraintsT<Constraints, implementation::Constraints>
    {
    };
}
