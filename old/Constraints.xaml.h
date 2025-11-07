#pragma once

#include "Constraints.g.h"
#include "constraintclass.hpp"
inline int _ConstraintsMode = 0;
inline int _NewPutGlobalPercNext = 100;
ystring trim(ystring s);
winrt::Windows::Foundation::IInspectable WindowFromPage(winrt::Windows::Foundation::IInspectable pg);

namespace winrt::WuiFET::implementation
{
    using namespace winrt;
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Controls;
    using namespace Microsoft::UI::Xaml::Input;

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
            MustTC0 = 1;
            m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{ s });
            MustTC0 = 0;
        }
        void Refresh()
        {
            MustTC0 = 1;
            m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{ L"" });
            MustTC0 = 0;
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
        bool _OnlyShowEntries = false;

        void OnlyShowEntries(bool v)
        {
            _OnlyShowEntries = v;
            Refresh(L"Left_List");
		}
        bool OnlyShowEntries()
        {
            return _OnlyShowEntries;
		}

        int LeftMode = 0; // 1 Teachers, 2 Classes, 3 Lessons, 4 Rooms, 5 Activities, 6 Tags
        int MultiLeft = 0; // Multiple Pick left

        // getter for leftsm
        Microsoft::UI::Xaml::Controls::ListViewSelectionMode leftsm()
        {
            if (MultiLeft)
                return Microsoft::UI::Xaml::Controls::ListViewSelectionMode::Multiple;
            return Microsoft::UI::Xaml::Controls::ListViewSelectionMode::Single;
		}


        std::vector<XML3::XMLElement*> SelectedLefts;
        std::vector<XML3::XMLElement*> SelectedLefts2;
        XML3::XMLElement* SelectedLeft = 0;
        XML3::XMLElement* SelectedLeft2 = 0;
        ystring LeftFilter;
        void OnLeftFilter(IInspectable const&, IInspectable const&)
        {
            LeftFilter = fb11().Text().c_str();
            Refresh(L"Left_List");
            Refresh(L"leftsm");
        }

        winrt::Windows::Foundation::Collections::IObservableVector<winrt::WuiFET::Item> Left_List()
        {
            auto items = winrt::single_threaded_observable_vector<winrt::WuiFET::Item>();
            auto x = project->x;
            if (LeftMode == 1)
            {
                return GetList(x->GetRootElement()["Teachers_List"], LeftFilter.c_str(), 0, [&](XML3::XMLElement* ekx) -> bool { if (!_OnlyShowEntries) return true; return LeftChanged2(ekx);  });
            }
            if (LeftMode == 3) // Subjects
            {
                return GetList(x->GetRootElement()["Subjects_List"], LeftFilter.c_str(),0, [&](XML3::XMLElement* ekx) -> bool { if (!_OnlyShowEntries) return true; return LeftChanged2(ekx);  });
            }
            if (LeftMode == 6) // tags
            {
                return GetList(x->GetRootElement()["Activity_Tags_List"], LeftFilter.c_str(), 0, [&](XML3::XMLElement* ekx) -> bool { if (!_OnlyShowEntries) return true; return LeftChanged2(ekx);  });
            }
            if (LeftMode == 4) // Rooms
            {
                return GetList(x->GetRootElement()["Rooms_List"], LeftFilter.c_str(), 0, [&](XML3::XMLElement* ekx) -> bool { if (!_OnlyShowEntries) return true; return LeftChanged2(ekx);  });
            }
           
            if (LeftMode == 5) // Activities
            {
                auto& r = x->GetRootElement()["Activities_List"];
                for (auto& rr : r)
                {
                    ystring name = DisplayNameForActivity(project, rr, 0xFFFFFFFF);
//                    std::shared_ptr<XML3::XMLElement> ee = rr.FindElementZ("Id", true);
 //                   ystring name = trim(ee->GetContent());
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

                    if (_OnlyShowEntries)
                    {
                        if (LeftChanged2(&rr) == 0)
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
                    ystring name1 = DisplayName(rr);
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

                    if (_OnlyShowEntries)
                    {
                        if (LeftChanged2(&rr) == 0)
                            S1 = 1;
                    }

                    if (S1 == 0)
                        items.Append(it1);
                    S1 = 0;
                    for (auto& rr2 : rr)
                    {
                        if (rr2.GetElementName() != "Group")
                            continue;

                        ystring name2 = DisplayName(rr2);
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

                        if (_OnlyShowEntries)
                        {
                            if (LeftChanged2(&rr2) == 0)
                                S1 = 1;
						}

                        if (S1 == 0)
                            items.Append(it2);
                        S1 = 0;
                        for (auto& rr3 : rr2)
                        {
                            if (rr3.GetElementName() != "Subgroup")
                                continue;

                            ystring name3 = DisplayName(rr3);

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

                            if (_OnlyShowEntries)
                            {
                                if (LeftChanged2(&rr3) == 0)
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
							if (ViewingConstraint->SubType == 5)
								s2 = "Activity_Id";
                            newel.AddElement(s2).SetContent(trim(SelectedLeft->FindElementZ("Name", true)->GetContent()));
                        }
                        // And the parameters
                        for (auto& param : ViewingConstraint->params)
                        {
                            newel.AddElement(param.x1.c_str()).SetContent(std::to_string(param.def).c_str());
						}
                        Refresh(L"Right_List");
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


		int _Percentage = 100;
        int Percentage()
        {
            return _Percentage;
        }
        void Percentage(int value)
        {
            if (_Percentage != value)
            {
                _Percentage = value;

                if (SelectedLeft2)
                {
                    ystring v;
                    v.Format(L"%i", value);
                    SelectedLeft2->FindElementZ("Weight_Percentage", true)->SetContent(v);
                }
                else
                {
                    _NewPutGlobalPercNext = _Percentage;
                }

            }
		}

        void LoadSelectedLeft()
        {
			_IsActiveChecked = false;
            if (SelectedLeft2 && (SelectedLeft || SelectedLefts.size()))
            {
                _IsActiveChecked = 1;
				_Percentage = _wtoi(trim(SelectedLeft2->FindElementZ("Weight_Percentage", true)->GetContent()).c_str());
                Refresh(L"IsActiveChecked");
                Refresh(L"Percentage");

                std::set<ystring> NamesToSearch;
                if (SelectedLefts.size() && MultiLeft)
                {
                    for (auto sl : SelectedLefts)
                    {
                        ystring SelectedLeftName = trim(sl->FindElementZ(LeftMode == 5 ? "Id" : "Name", true)->GetContent());
                        NamesToSearch.insert(SelectedLeftName);
                    }
                }
                else
                if (SelectedLeft)
                {
                    ystring SelectedLeftName = trim(SelectedLeft->FindElementZ(LeftMode == 5 ? "Id" : "Name", true)->GetContent());
                    NamesToSearch.insert(SelectedLeftName);
                }

                // Special Load
                ystring y;
                y.Format(L"%s", s(83));
                str3(y.c_str());

                if (SpecialView == 1 || SpecialView == 3 || SpecialView == 4)
                {
                    std::string search_for = "ConstraintTeacherNotAvailableTimes";
                    std::string search_for2 = "Teacher";
                    if (LeftMode == 2)
                    {
						search_for = "ConstraintStudentsSetNotAvailableTimes";
						search_for2 = "Students";
                    }
                    if (LeftMode == 5)
                    {
						search_for = "ConstraintActivityPreferredStartingTimes";
                        if (SpecialView == 4)
							search_for = "ConstraintActivityPreferredTimeSlots";
						search_for2 = "Activity_Id";
                    }
                    if (LeftMode == 6)
                    {
						search_for = "ConstraintTagNotAvailableTimes";
						search_for2 = "Tag";
                    }
                    if (LeftMode == 4)
                    {
						search_for = "ConstraintRoomNotAvailableTimes";
						search_for2 = "Room";
                    }
                    const char* s1 = "Time_Constraints_List";
                    auto x = project->x;
                    XML3::XMLElement* root = &x->GetRootElement();
                    if (ViewingConstraint && ViewingConstraint->Type == 1)
                        s1 = "Space_Constraints_List";
                    auto r0 = root->FindElementZ(s1, true);
                    for (auto& r1 : *r0)
                    {
                        if (r1.GetElementName() != search_for)
                            continue;
                        auto tn = trim(r1.FindElementZ(search_for2.c_str(), true)->GetContent());

                        bool RF = 0;
                        for(auto& rs : NamesToSearch)
                        {
                            if (tn == rs)
                            {
                                RF = 1;
                                break;
                            }
						}
                        if (!RF)
							continue;

						SelectedLeft2 = &r1; // default editable the first one
                        int p = 100;
                        p = _wtoi(trim(SelectedLeft2->FindElementZ("Weight_Percentage", true)->GetContent()).c_str());
                        _Percentage = p;
                        Refresh(L"Percentage");
                        Refresh(L"zz1_ch");
                        Refresh(L"zz1_is");

                        // There is one found, change y
                        y.Format(L"%s #%i", s(82), 1);
                        str3(y.c_str());
                        break;
                    }
                }


                if (SpecialView == 2)
                {
                    std::string search_for = "ConstraintSubjectPreferredRooms";
                    std::string search_for2 = "Subject";
                    if (LeftMode == 1)
                    {
						search_for = "ConstraintTeacherPreferredRooms";
						search_for2 = "Teacher";
                    }
                    if (LeftMode == 2)
                    {
						search_for = "ConstraintStudentsSetPreferredRooms";
						search_for2 = "Students";
                    }
                    if (LeftMode == 5)
                    {
                        search_for = "ConstraintActivityPreferredRooms";
                        search_for2 = "Activity_Id";
                    }
                    if (LeftMode == 6)
                    {
						search_for = "ConstraintTagPreferredRooms";
						search_for2 = "Tag";
                    }
                    const char* s1 = "Space_Constraints_List";
                    auto x = project->x;
                    XML3::XMLElement* root = &x->GetRootElement();
                    if (ViewingConstraint && ViewingConstraint->Type == 0)
                        s1 = "Time_Constraints_List";
                    auto r0 = root->FindElementZ(s1, true);
                    for (auto& r1 : *r0)
                    {
                        if (r1.GetElementName() != search_for)
                            continue;
                        auto tn = trim(r1.FindElementZ(search_for2.c_str(), true)->GetContent());
                      
                        bool RF = 0;
                        for (auto& rs : NamesToSearch)
                        {
                            if (tn == rs)
                            {
                                RF = 1;
                                break;
                            }
                        }
                        if (!RF)
                            continue;



                        SelectedLeft2 = &r1; // default editable the first one
                        int p = 100;
                        p = _wtoi(trim(SelectedLeft2->FindElementZ("Weight_Percentage", true)->GetContent()).c_str());
                        _Percentage = p;
                        Refresh(L"Percentage");
                        Refresh(L"zz1_ch");
                        Refresh(L"zz1_is");
                        Refresh(L"Right_List");

                        // There is one found, change y
                        y.Format(L"%s #%i", s(82), 1);
                        str3(y.c_str());
                        break;
                    }
                }


            }

        }

        void NoLeftInit()
        {
            auto x = project->x;
            XML3::XMLElement* root = &x->GetRootElement();
            const char* s1 = "Time_Constraints_List";
            if (ViewingConstraint && ViewingConstraint->Type == 1)
                s1 = "Space_Constraints_List";
            auto r0 = root->FindElementZ(s1, true);
            _Percentage = 100;
            _IsActiveChecked = false;
            for (auto& r1 : *r0)
            {
                if (!ViewingConstraint)
					break;
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
            LeftChanged2(nullptr);
        }

        bool LeftChanged2(XML3::XMLElement* TestOnly)
        {
            bool Test = false;
            auto lv = Left_ListView();


            if (MultiLeft)
            {
                SelectedLefts2.clear();
                auto sels = lv.SelectedItems();
				SelectedLefts.clear();
                for (auto j : sels)
                {
                    auto sel = j.as<winrt::WuiFET::Item>();
                    if (sel && sel.ptr())
                    {
                        XML3::XMLElement* e = (XML3::XMLElement*)sel.ptr();
                        SelectedLefts.push_back(e);
					}
                }

                if (SelectedLefts.size())
                {
                    _RightVisible = 1;


                    _Percentage = 100;
                    _IsActiveChecked = false;

                    // Special Load
                    ystring y;
                    y.Format(L"%s", s(83));
                    str3(y.c_str());

                    // Find the entry in the XML
                    auto x = project->x;
                    XML3::XMLElement* root = &x->GetRootElement();
                    auto r0 = root->FindElementZ(_ConstraintsMode == 1 ? "Space_Constraints_List" : "Time_Constraints_List", true);
                    for (auto& c0 : *r0)
                    {
                        std::string search_for = "ConstraintMinDaysBetweenActivities";
                        if (c0.GetElementName() != search_for)
                            continue;

                        std::string SearchElement1 = "";
                        std::string SearchElement2 = "";
                        if (LeftMode == 1)
                        {
                            SearchElement1 = "Teacher_Name";
                            SearchElement2 = "Teacher";
                        }
                        if (LeftMode == 2)
                        {
                            SearchElement1 = "Students";
                        }
                        if (LeftMode == 3)
                        {
                            SearchElement1 = "Subject";
                        }
                        if (LeftMode == 4)
                        {
                            SearchElement1 = "Room";
                        }
                        if (LeftMode == 5)
                        {
                            SearchElement1 = "Activity_Id";
                        }
                        if (LeftMode == 6)
                        {
                            SearchElement1 = "Tag";
                        }

                        if (SearchElement1.length() == 0 && SearchElement2.length() == 0)
                            continue;
                        int FoundElements = 0;
                        int num_xml = 0;
                        for (auto& c1 : c0)
                        {
                            if (c1.GetElementName() == "Number_of_Activities")
                            {
								num_xml = _wtoi(trim(c1.GetContent()).c_str());
                            }
                            if (c1.GetElementName() != SearchElement1 && c1.GetElementName() != SearchElement2)
								continue;
                            auto tn = trim(c1.GetContent());

                            for (auto selk : SelectedLefts)
                            {
                                auto seln = trim(selk->FindElementZ(LeftMode == 5 ? "Id" : "Name", true)->GetContent());
                                if (tn == seln)
                                    FoundElements++;
                            }
                        }
                        if (FoundElements != SelectedLefts.size())
							continue;

                        if (SelectedLefts.size() != num_xml)
                            continue;

                        if (!TestOnly)
                        {
                            SelectedLeft2 = &c0;
                            LoadSelectedLeft();
                        }
                        Test = 1;
                        break;
                    }
                }
                else
                 _RightVisible = 0;
                if (!TestOnly)
                    RightRefresh();
                return Test;
            }

            SelectedLeft = 0;
            auto sel = lv.SelectedItem().as<winrt::WuiFET::Item>();
            if (sel && sel.ptr())
                SelectedLeft = (XML3::XMLElement*)sel.ptr();
            SelectedLeft2 = nullptr;
            if (!TestOnly)
            {
                _IsActiveChecked = false;
                _Percentage = 100;
            }
            if (TestOnly)
				SelectedLeft = TestOnly;


            if (SelectedLeft)
            {
                _RightVisible = 1;
                

                // Special Load
                ystring y;
                y.Format(L"%s", s(83));
                str3(y.c_str());

                // Find the entry in the XML
                auto x = project->x;
                XML3::XMLElement* root = &x->GetRootElement();
                auto r0 = root->FindElementZ(_ConstraintsMode == 1 ? "Space_Constraints_List" : "Time_Constraints_List", true);
                for (auto& c0 : *r0)
                {
                    if (ViewingConstraint)
                    {
                        if (c0.GetElementName() != ViewingConstraint->x0)
                            continue;
                    }
                    else
                    if (SpecialView == 1)
                    {
						std::string search_for = "ConstraintTeacherNotAvailableTimes";
                        if (LeftMode == 2)
							search_for = "ConstraintStudentsSetNotAvailableTimes";
                        if (LeftMode == 4)
							search_for = "ConstraintRoomNotAvailableTimes";
						if (LeftMode == 5)
							search_for = "ConstraintActivityNotAvailableTimes";
						if (LeftMode == 6)
							search_for = "ConstraintTagNotAvailableTimes";
                        if (c0.GetElementName() != search_for)
                            continue;
                    }
                    else
                    if (SpecialView == 2)
                    {
						std::string search_for = "ConstraintSubjectPreferredRooms";
                        if (LeftMode == 1)
							search_for = "ConstraintTeacherPreferredRooms";
						if (LeftMode == 2)
							search_for = "ConstraintStudentsSetPreferredRooms";
						if (LeftMode == 5)
							search_for = "ConstraintActivityPreferredRooms";
						if (LeftMode == 6)
							search_for = "ConstraintTagPreferredRooms";
                        if (c0.GetElementName() != search_for)
                            continue;
                    }
                    else
                    if (SpecialView == 3)
                    {
                        std::string search_for = "ConstraintActivityPreferredStartingTimes";
                        if (c0.GetElementName() != search_for)
                            continue;
                    }
                    else
                    if (SpecialView == 4)
                    {
                        std::string search_for = "ConstraintActivityPreferredTimeSlots";
                        if (c0.GetElementName() != search_for)
                            continue;
                    }

                    std::string SearchElement1 = "";
                    std::string SearchElement2 = "";
                    if (LeftMode == 1)
                    {
                        SearchElement1 = "Teacher_Name";
                        SearchElement2 = "Teacher";
                    }
                    if (LeftMode == 2)
                    {
                        SearchElement1 = "Students";
					}
                    if (LeftMode == 3)
                    {
                        SearchElement1 = "Subject";
                    }
                    if (LeftMode == 4)
                    {
                        SearchElement1 = "Room";
                    }
                    if (LeftMode == 5)
                    {
                        SearchElement1 = "Activity_Id";
					}
                    if (LeftMode == 6)
                    {
                        SearchElement1 = "Tag";
                    }

                    if (SearchElement1.length() == 0 && SearchElement2.length() == 0)
						continue;
					auto elf = c0.FindElementZ(SearchElement1.c_str());
                    if (!elf)
                        elf = c0.FindElementZ(SearchElement2.c_str());
                    if (!elf)
                        continue;
                    auto tn = trim(elf->GetContent());
                    auto seln = trim(SelectedLeft->FindElementZ(LeftMode == 5 ? "Id" : "Name", true)->GetContent());
                    if (tn != seln)
                        continue;

                    if (!TestOnly)
                    {
                        SelectedLeft2 = &c0;
                        LoadSelectedLeft();
                    }
                    Test = 1;
					break;
                }
            }
            else
				_RightVisible = 0;
            if (!TestOnly)
                RightRefresh();
            return Test;
        }

        void RightRefresh()
        {
            Refresh(L"IsListRightVisible");
            Refresh(L"IsRightVisible");
            Refresh(L"IsActiveVisible");
            Refresh(L"IsActiveChecked");
            Refresh(L"IsMultiple");
            Refresh(L"IsPercentageVisible");
            Refresh(L"IsGridRightVisible");
            Refresh(L"Right_List");
            Refresh(L"Percentage");
            Refresh(L"zz1_ch");
            Refresh(L"zz1_is");
        }

        void RightChanged(IInspectable, IInspectable)
        {
            // right list
            // If in refresh, skip
            if (MustTC0)
                return;

			auto lv = Right_ListView();
            auto sels = lv.SelectedItems();
            if (SpecialView == 2)
            {
                if (SelectedLeft2)
                {
                    // Remove all "Room"
                    for (int i = (int)SelectedLeft2->GetChildrenNum() - 1; i >= 0; i--)
                    {
                        auto& el = SelectedLeft2->GetChildren().at(i);
                        if (el->GetElementName() == "Preferred_Room")
                            SelectedLeft2->RemoveElement(i);
                    }

                    if (sels.Size() == 0)
                    {
                        // Remove it
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
                    else
                    {
                        // Add all selected
                        for (auto it : sels)
                        {
                            auto sel = it.as<winrt::WuiFET::Item>();
                            if (sel && sel.ptr())
                            {
                                XML3::XMLElement* el = (XML3::XMLElement*)sel.ptr();
                                auto name = trim(el->FindElementZ("Name", true)->GetContent());
                                auto& e2 = SelectedLeft2->AddElement("Preferred_Room");
                                e2.SetContent(name);
                            }
                        }
                        SelectedLeft2->FindElementZ("Number_of_Preferred_Rooms", true)->SetContent(std::to_string(sels.Size()).c_str());

                    }
                }
                else
                {
                    // We create a new SelectedLeft2
                    if (sels.Size() == 0 || !SelectedLeft)
						return;
                    auto x = project->x;
                    XML3::XMLElement* root = &x->GetRootElement();
                    auto r0 = root->FindElementZ(_ConstraintsMode == 1 ? "Space_Constraints_List" : "Time_Constraints_List", true);
					std::string search_for = "ConstraintSubjectPreferredRooms";
                    if (LeftMode == 1)
						search_for = "ConstraintTeacherPreferredRooms";
					if (LeftMode == 2)
						search_for = "ConstraintStudentsSetPreferredRooms";
					if (LeftMode == 5)
						search_for = "ConstraintActivityPreferredRooms";
					if (LeftMode == 6)
						search_for = "ConstraintTagPreferredRooms";

					auto& l2 = r0->AddElement(search_for.c_str());
					SelectedLeft2 = &l2;

                    // Add all selected
                    for (auto it : sels)
                    {
                        auto sel = it.as<winrt::WuiFET::Item>();
                        if (sel && sel.ptr())
                        {
                            XML3::XMLElement* el = (XML3::XMLElement*)sel.ptr();
                            auto name = trim(el->FindElementZ("Name", true)->GetContent());
                            auto& e2 = SelectedLeft2->AddElement("Preferred_Room");
                            e2.SetContent(name);
                        }
                    }
                    SelectedLeft2->FindElementZ("Number_of_Preferred_Rooms", true)->SetContent(std::to_string(sels.Size()).c_str());
					SelectedLeft2->AddElement("Active").SetContent("True");
                    SelectedLeft2->AddElement("Weight_Percentage").SetContent(std::to_string(_Percentage).c_str());
                    const char* s2 = "Subject";
                    if (LeftMode == 1)
						s2 = "Teacher";
					if (LeftMode == 2)
						s2 = "Students";
					if (LeftMode == 5)
						s2 = "Activity_Id";
					if (LeftMode == 6)
						s2 = "Tag";
                    SelectedLeft2->AddElement(s2).SetContent(trim(SelectedLeft->FindElementZ("Name", true)->GetContent()));
                    if (LeftMode == 5)
                        SelectedLeft2->AddElement(s2).SetContent(trim(SelectedLeft->FindElementZ("Id", true)->GetContent()));
                }
            }
        }

        bool MustTC0 = 0;
        void zz1_Click(IInspectable sender, IInspectable)
        {
            if (!SelectedLeft)
                return;

            auto button = sender.as<DropDownButton>();
            MenuFlyout flyout;
            flyout.Placement(Microsoft::UI::Xaml::Controls::Primitives::FlyoutPlacementMode::Bottom);
            ystring SelectedName = trim(SelectedLeft->FindElementZ(LeftMode == 5 ? "Id" : "Name", true)->GetContent());
            auto x = project->x;
            XML3::XMLElement* root = &x->GetRootElement();
            auto r0 = root->FindElementZ(_ConstraintsMode == 1 ? "Space_Constraints_List" : "Time_Constraints_List", true);

            // New Entry
            if (1)
            {
                ystring y;
                y.Format(L"%s", s(83));
                MenuFlyoutItem item;
                item.Text(y.c_str());
                item.Tag(winrt::box_value((long long)0));
                item.Click([&](IInspectable const&, RoutedEventArgs const&)
                    {
                        SelectedLeft2 = 0;
                        _Percentage = 100;
                        Refresh(L"Right_List");
                        Refresh(L"Percentage");
                        Refresh(L"zz1_is");
                        Refresh(L"zz1_ch");
                        str3(s(83));
                    });
                flyout.Items().Append(item);
            }


            int yy = 0;
            std::string search_for1;
            std::string search_for2 = "Teacher";
            if (SpecialView == 1 || SpecialView == 3 || SpecialView == 4)
            {
                search_for1 = "ConstraintTeacherNotAvailableTimes";
                if (LeftMode == 2)
                {
                    search_for1 = "ConstraintStudentsSetNotAvailableTimes";
                    search_for2 = "Students";
                }
                if (LeftMode == 4)
                {
                    search_for1 = "ConstraintRoomNotAvailableTimes";
                    search_for2 = "Room";
                }
                if (LeftMode == 6)
                {
                    search_for1 = "ConstraintTagNotAvailableTimes";
                    search_for2 = "Tag";
				}
                if (LeftMode == 5)
                {
                    search_for1 = "ConstraintActivityPreferredStartingTimes";
                    if (SpecialView == 4)
						search_for1 = "ConstraintActivityPreferredTimeSlots";
                    search_for2 = "Activity_Id";
                }
            }
            if (SpecialView == 2)
            {
                search_for1 = "ConstraintSubjectPreferredRooms";
				search_for2 = "Subject";
                if (LeftMode == 1)
                {
                    search_for1 = "ConstraintTeacherPreferredRooms";
                    search_for2 = "Teacher";
				}
                if (LeftMode == 2)
                {
                    search_for1 = "ConstraintStudentsSetPreferredRooms";
                    search_for2 = "Students";
				}
                if (LeftMode == 5)
                {
					search_for1 = "ConstraintActivityPreferredRooms";
					search_for2 = "Activity_Id";
                }
                if (LeftMode == 6)
                {
                    search_for1 = "ConstraintTagPreferredRooms";
                    search_for2 = "Tag";
                }
            }
            if (SpecialView == 3 || SpecialView == 4)
            {
                search_for1 = "ConstraintSubjectPreferredRooms";
                search_for2 = "Subject";
             
                if (LeftMode == 5)
                {
                    search_for1 = "ConstraintActivityPreferredStartingTimes";
					if (SpecialView == 4)
						search_for1 = "ConstraintActivityPreferredTimeSlots";
                    search_for2 = "Activity_Id";
                }
                if (LeftMode == 6)
                {
                    search_for1 = "ConstraintTagPreferredRooms";
					search_for2 = "Tag";
                }

            }

            for (auto& r1 : *r0)
            {
                if (r1.GetElementName() != search_for1)
                    continue;
                // This teacher?
                auto tn = trim(r1.FindElementZ(search_for2.c_str(), true)->GetContent());
                if (tn != SelectedName)
                    continue;
                ystring y;
                yy++;
                auto whatper = _wtoi(trim(r1.FindElementZ("Weight_Percentage", true)->GetContent()).c_str());
                y.Format(L"%s #%i (%i%%)", s(82), yy, whatper);

                MenuFlyoutItem item;
                item.Text(y.c_str());
                item.Tag(winrt::box_value((long long)&r1));
                item.Click([&](IInspectable const& sender, RoutedEventArgs const&)
                    {
                        auto it = sender.as<MenuFlyoutItem>();
                        SelectedLeft2 = (XML3::XMLElement*)unbox_value<long long>(it.Tag());
                        _Percentage = _wtoi(trim(SelectedLeft2->FindElementZ("Weight_Percentage", true)->GetContent()).c_str());
                        ystring y = it.Text().c_str();
                        wchar_t buf[256] = {};
                        wcscpy_s(buf, 256, y.c_str());
                        auto ch = wcschr(buf, L'(');
                        if (ch)
                            *ch = 0;
                        str3(buf);
                        Refresh(L"Right_List");
                        Refresh(L"Percentage");
                        Refresh(L"zz1_is");
                        Refresh(L"zz1_ch");

                    });
                flyout.Items().Append(item);
            }


            button.Flyout(flyout);

        }

		bool _IsGridRightVisible = false;
        bool IsGridRightVisible()
        {
            return _IsGridRightVisible && (_IsActiveChecked || SpecialView);
        }
		bool _IsListRightVisible = false;
        bool IsListRightVisible()
        {
            return _IsListRightVisible;
		}

        winrt::Windows::Foundation::Collections::IObservableVector<winrt::WuiFET::DGHeaderModel> zz1_ch()
        {
            auto headers = winrt::single_threaded_observable_vector<winrt::WuiFET::DGHeaderModel>();
            if (SpecialView == 1 || SpecialView == 3 || SpecialView == 4)
            {
                // Should show the days
                auto x = project->x;
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

            }
            return headers;
        }

        winrt::Windows::Foundation::Collections::IObservableVector<DGRowModel> zz1_is()
        {
            auto rows = winrt::single_threaded_observable_vector<DGRowModel>();
            if (!project->x)
                return rows;
            if (SpecialView == 1 || SpecialView == 3 || SpecialView == 4)
            {
                // Showing the Not Available 
                auto x = project->x;
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



                int i = -1;
                for (auto& hou : *hours)
                {
                    if (hou.GetElementName() != "Hour")
                        continue;
                    i++;
                    auto cells = winrt::single_threaded_observable_vector<DGCellModel>();

                    int y = -1;
                    DGCellModel c10(i, 0, DGCellType::Block, std::to_wstring(i + 1));

                    cells.Append(c10);
                    for (auto& day : *days)
                    {
                        if (day.GetElementName() != "Day")
                            continue;
                        y++;

                        DGCellModel c1(i, y + 1, DGCellType::Check, L"");
                        if (LeftMode == 1)
                            c1.WhatX(1);
						if (LeftMode == 2)
							c1.WhatX(2);
                        if (LeftMode == 3)
							c1.WhatX(3);
						if (LeftMode == 4)
							c1.WhatX(7);
                        if (LeftMode == 5)
                        {
                            if (SpecialView == 4)
                                c1.WhatX(6);
							else
                                c1.WhatX(5);
                        }
						if (LeftMode == 6)
							c1.WhatX(8);

                        c1.pi1(zz1_gr());
                        c1.pi2(WindowFromPage(*this));

                        if (SelectedLeft2) // ConstraintTeacherNotAvailableTimes
                        {
                            bool Put = 0;
                            c1.Ptr4((long long)SelectedLeft2);
                            for (auto& el : *SelectedLeft2)
                            {
                                if (SpecialView == 1)
                                {
                                    if (el.GetElementName() != "Not_Available_Time")
                                        continue;
                                }
                                if (SpecialView == 3)
                                {
                                    if (el.GetElementName() != "Preferred_Starting_Time")
                                        continue;
								}
                                if (SpecialView == 4)
                                {
                                    if (el.GetElementName() != "Preferred_Time_Slot")
                                        continue;
                                }
                                if (trim(el.FindElementZ("Day", true)->GetContent()) != days2[y])
                                    continue;
                                if (trim(el.FindElementZ("Hour", true)->GetContent()) != hours2[i])
                                    continue;
                                c1.ValueBB(true);
                                c1.Ptr((long long)&el);
                                Put = 1;
                                break;
                            }
                        }

                        c1.Ptr3((long long)SelectedLeft); 
                        c1.Ptr2((long long)root);
                        c1.Ptr5((long long)project);
                        cells.Append(c1);
                    }

                    DGRowModel rr(i, cells);
                    rows.Append(rr);
                }
                return rows;
            }
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

        ystring RightFilter;
        void OnRightFilter(IInspectable const&, IInspectable const&)
        {
            RightFilter = fb11r().Text().c_str();
            Refresh(L"Right_List");
        }


        void GenericContentChanging(IInspectable const& sender,
            winrt::Microsoft::UI::Xaml::Controls::ContainerContentChangingEventArgs const& args)
        {
            if (args.InRecycleQueue())
                return;

            auto lv = sender.as<winrt::Microsoft::UI::Xaml::Controls::ListView>();
            if (!lv)
                return;

            auto sels = lv.SelectedItems();
            if (!sels)
                return;

            auto it = args.Item().try_as<winrt::WuiFET::Item>();
            if (!it)
                return;

            uint32_t idx = 0;
            bool exists = sels.IndexOf(it, idx);

            if (it.Sel())
            {
                if (!exists)
                    sels.Append(it);
            }
            else if (exists)
            {
                sels.RemoveAt(idx);
            }
        }




        winrt::Windows::Foundation::Collections::IObservableVector<winrt::WuiFET::Item> Right_List()
        {
            auto items = winrt::single_threaded_observable_vector<winrt::WuiFET::Item>();
            if (SpecialView == 2)
            {
                // Rooms
                auto x = project->x;
                auto& re = x->GetRootElement()["Rooms_List"];
                for (auto& rr2 : re)
                {
                    auto name1 = trim(rr2.FindElementZ("Name", true)->GetContent().c_str());
                    auto name2 = DisplayName(rr2);
                    std::transform(name2.begin(), name2.end(), name2.begin(), towlower);
                    if (name2.find(RightFilter.c_str()) == ystring::npos)
                        continue;

                    winrt::WuiFET::Item it;
                    it.Name1(name2.c_str());
                    it.ptr((long long)&rr2);
                    if (SelectedLeft2)
                    {
						std::string search_for = "Preferred_Room";
                        for(auto& s : *SelectedLeft2)
                        {
                            if (s.GetElementName() != search_for)
                                continue;
                            auto v = trim(s.GetContent());
                            if (v == name1)
                                it.Sel(true);
						}
					}
                    items.Append(it);

                }

            }
            return items;
        }


        std::shared_ptr<A_CONSTRAINT> ViewingConstraint;
        int SpecialView = 0; // 1 - Not Available Times, 2 Rooms, 3 Preferred Starting Times , 4 Preferred Time Slots
        int SpecialSubView = 0;

        // Time + Teacher
        void TS_Teacher_NotAvailableTimes(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            str2(s(77));
            LeftMode = 1; // Teachers
            MultiLeft = 0;
            _LeftVisible = 1;
            _Percentage = 100;
            _IsActiveVisible = false;
            _IsMultiple = true;
            _IsPercentageVisible = true;
            _IsGridRightVisible = true;
            _IsListRightVisible = false;
            ViewingConstraint = nullptr;
            SpecialView = 1;
            Refresh();
        }

        void TS_Teacher_Single(std::wstring d0,std::string x0,std::string x1,int from,int to,int def,bool R = 1)
        {
            SpecialView = 0;
            LeftMode = 1; // Teachers
            MultiLeft = 0;
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

            if (x1.length())
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
        void TS_Teacher_NoTwoDays(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            TS_Teacher_Single(s(162), "ConstraintTeacherNoTwoConsecutiveDays", "", 0, 0,0);

        }


        // Time + Teachers
        void TS_Teachers_Single(std::wstring d0, std::string x0, std::string x1, int from, int to, int def, bool R = 1)
        {
            SpecialView = 0;
            LeftMode = 0; // None
            MultiLeft = 0;
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

            if (x1.length())
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
            TS_Teachers_Single(s(97), "ConstraintTeachersMaxGapsPerWeek", "Max_Gaps", 0, HowManyHours() * HowManyDays(), HowManyHours() * HowManyDays());

        }






        // Time + Class
        void TS_Class_NotAvailableTimes(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            str2(s(77));
            LeftMode = 2; // Class
            MultiLeft = 0;
            _LeftVisible = 1;
            _Percentage = 100;
            _IsActiveVisible = false;
            _IsMultiple = true;
            _IsPercentageVisible = true;
            _IsGridRightVisible = true;
            _IsListRightVisible = false;
            ViewingConstraint = nullptr;
            SpecialView = 1;
            Refresh();
        }


        // Time + Space + Room
        void SS_Room_NotAvailableTimes(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            str2(s(77));
            LeftMode = 4; // Room
            MultiLeft = 0;
            _LeftVisible = 1;
            _Percentage = 100;
            _IsActiveVisible = false;
            _IsMultiple = true;
            _IsPercentageVisible = true;
            _IsGridRightVisible = true;
            _IsListRightVisible = false;
            ViewingConstraint = nullptr;
            SpecialView = 1;
            Refresh();
        }

        // Time + Activity
        void TS_Activity_Single(std::wstring d0, std::string x0, std::string x1, int perfrom,int perto,bool mult,int from, int to, int def, bool multil = 0,bool multiple = 0,bool R = 1)
        {
            SpecialView = 0;
            LeftMode = 5; // Activities
            MultiLeft = multil;
            _LeftVisible = 1;
            _IsActiveVisible = true;
            _IsMultiple = multiple;
            _IsPercentageVisible = (perfrom < 100 || perto < 100) ? true : false;
            _RightVisible = false;
            _IsGridRightVisible = true;
            _IsListRightVisible = false;

            ViewingConstraint = std::make_shared<A_CONSTRAINT>();
            ViewingConstraint->d0 = d0;
            ViewingConstraint->x0 = x0;
            ViewingConstraint->Type = 0;
            ViewingConstraint->SubType = 5;
            ViewingConstraint->w_from = perfrom;
            ViewingConstraint->w_to = perto;
            ViewingConstraint->SupportsMultiple = mult;


            if (x1.length())
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

        void TS_Activity_BeginsStudentsDay(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            TS_Activity_Single(s(171), "ConstraintActivityBeginsStudentsDay", "",0,100,false, 0, 0, 0);

        }
        void TS_Activity_EndsStudentsDay(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            TS_Activity_Single(s(172), "ConstraintActivityEndsStudentsDay", "", 0, 100, false, 0, 0, 0);

        }
        void TS_Activity_BeginsEndsStudentsDay(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            TS_Activity_Single(s(173), "ConstraintActivityBeginsOrEndsStudentsDay", "", 0, 100, false, 0, 0, 0);

        }
        void TS_Activity_BeginsTeacherDay(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            TS_Activity_Single(s(174), "ConstraintActivityBeginsTeachersDay", "", 0, 100, false, 0, 0, 0);

        }
        void TS_Activity_EndsTeacherDay(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            TS_Activity_Single(s(175), "ConstraintActivityEndsTeachersDay", "", 0, 100, false, 0, 0, 0);

        }
        void TS_Activity_BeginsEndsTeacherDay(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            TS_Activity_Single(s(176), "ConstraintActivityBeginsOrEndsTeachersDay", "", 0, 100, false, 0, 0, 0);

        }

        void TS_Activities_MinDaysBetween(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            TS_Activity_Single(s(177), "ConstraintMinDaysBetweenActivities", "", 0, 100, true, 0, 0, true,true,true);

        }

        void TS_Activity_PreferredStartingTimes(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            str2(s(163));
            LeftMode = 5; // Activities
            MultiLeft = 0;
            _LeftVisible = 1;
            _Percentage = 100;
            _IsActiveVisible = false;
            _IsMultiple = true;
            _IsPercentageVisible = true;
            _IsGridRightVisible = true;
            _IsListRightVisible = false;
            ViewingConstraint = nullptr;
            SpecialView = 3;
            Refresh();
        }
        void TS_Activity_PreferredTimeSlots(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            str2(s(163));
            LeftMode = 5; // Activities
            MultiLeft = 0;
            _LeftVisible = 1;
            _Percentage = 100;
            _IsActiveVisible = false;
            _IsMultiple = true;
            _IsPercentageVisible = true;
            _IsGridRightVisible = true;
            _IsListRightVisible = false;
            ViewingConstraint = nullptr;
            SpecialView = 4;
            Refresh();
        }

        // Space + Teacher
        void SS_Teacher_PreferredRooms(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            str2(s(146));
            LeftMode = 1; // Teachers
            MultiLeft = 0;
            SpecialView = 2;
            SpecialSubView = 0;
            ViewingConstraint = nullptr;
            _LeftVisible = 1;
            _IsMultiple = true;
            _IsPercentageVisible = true;
            _RightVisible = false;
            _IsGridRightVisible = false;
            _IsListRightVisible = true;
            Refresh();
        }

        void SS_Teacher_Single(std::wstring d0, std::string x0, std::string x1, int from, int to, int def, bool R = 1)
        {
            SpecialView = 0;
            LeftMode = 1; // Teachers
            MultiLeft = 0;
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
            SS_Teacher_Single(s(140), "ConstraintTeacherMaxRoomChangesPerDayInInterval", "Max_Room_Changes_Per_Day", 1, HowManyHours(), HowManyHours(),0);
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
        void SS_Teacher_MinGapsBetweenRoomChanges(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            SS_Teacher_Single(s(179), "ConstraintTeacherMinGapsBetweenRoomChanges", "Min_Gaps_Between_Room_Changes", 1, HowManyHours(), HowManyHours());

        }
        void SS_Teacher_MinGapsBetweenBuildingChanges(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            SS_Teacher_Single(s(180), "ConstraintTeacherMinGapsBetweenBuildingChanges", "Min_Gaps_Between_Building_Changes", 1, HowManyHours(), HowManyHours());

        }

		// Space + Teachers
        void SS_Teachers_Single(std::wstring d0, std::string x0, std::string x1, int from, int to, int def, bool R = 1)
        {
            SpecialView = 0;
            LeftMode = 0; // None
            MultiLeft = 0;
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

        void SS_Teachers_MinGapsBetweenRoomChanges(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            SS_Teachers_Single(s(179), "ConstraintTeachersMinGapsBetweenRoomChanges", "Min_Gaps_Between_Room_Changes", 1, HowManyHours(), HowManyHours());

        }
        void SS_Teachers_MinGapsBetweenBuildingChanges(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            SS_Teachers_Single(s(180), "ConstraintTeachersMinGapsBetweenBuildingChanges", "Min_Gaps_Between_Building_Changes", 1, HowManyHours(), HowManyHours());

        }

        void SS_Teachers_MaxRoomChangesDayInterval(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            SS_Teachers_Single(s(140), "ConstraintTeachersMaxRoomChangesPerDayInInterval", "Max_Room_Changes_Per_Day", 1, HowManyHours(), HowManyHours(), 0);
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

        void SS_Teachers_MaxBuildingChangesWeek(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            SS_Teachers_Single(s(139), "ConstraintTeachersMaxBuildingChangesPerWeek", "Max_Building_Changes_Per_Week", 1, HowManyHours(), HowManyHours());

        }

        // Space + Class
        void SS_Class_PreferredRooms(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            str2(s(146));
            LeftMode = 2; // Class
            MultiLeft = 0;
            SpecialView = 2;
            SpecialSubView = 0;
            ViewingConstraint = nullptr;
            _LeftVisible = 1;
            _IsMultiple = true;
            _IsPercentageVisible = true;
            _RightVisible = false;
            _IsGridRightVisible = false;
            _IsListRightVisible = true;
            Refresh();
        }

        void SS_Class_Single(std::wstring d0, std::string x0, std::string x1, int from, int to, int def, bool R = 1)
        {
            SpecialView = 0;
            LeftMode = 2; // Class
            MultiLeft = 0;
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
        void SS_Class_MaxBuildingChangesDay(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            SS_Class_Single(s(143), "ConstraintStudentSetMaxBuildingChangesPerDay", "Max_Building_Changes_Per_Day", 1, HowManyHours(), HowManyHours());
        }
        void SS_Class_MaxBuildingChangesWeek(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            SS_Class_Single(s(143), "ConstraintStudentSetMaxBuildingChangesPerWeek", "Max_Building_Changes_Per_Week", 1, HowManyHours(), HowManyHours());
        }


		// Space + Subject
        void SS_Lesson_PreferredRooms(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            str2(s(146));
            LeftMode = 3; // Lesson
            MultiLeft = 0;
            SpecialView = 2;
            SpecialSubView = 0;
            ViewingConstraint = nullptr;
            _LeftVisible = 1;
            _IsMultiple = true;
            _IsPercentageVisible = true;
            _RightVisible = false;
            _IsGridRightVisible = false;
            _IsListRightVisible = true;

            Refresh();

        }

        // Space + Activity
        void SS_Activity_PreferredRooms(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            str2(s(146));
            LeftMode = 5; // Activities
            MultiLeft = 0;
            SpecialView = 2;
            SpecialSubView = 0;
            ViewingConstraint = nullptr;
            _LeftVisible = 1;
            _IsMultiple = true;
            _IsPercentageVisible = true;
            _RightVisible = false;
            _IsGridRightVisible = false;
            _IsListRightVisible = true;
            Refresh();
        }

        // Space + Tag
        void SS_Tag_PreferredRooms(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            str2(s(146));
            LeftMode = 6; // Tag
            MultiLeft = 0;
            SpecialView = 2;
            SpecialSubView = 0;
            ViewingConstraint = nullptr;
            _LeftVisible = 1;
            _IsMultiple = true;
            _IsPercentageVisible = true;
            _RightVisible = false;
            _IsGridRightVisible = false;
            _IsListRightVisible = true;

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
