#pragma once

#include "Constraints.g.h"
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
                if (_ConstraintsMode == 2)
                    str1(s(187));
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


        bool IsLeftVisible()
        {
            return _LeftVisible;
        }
        bool IsRightVisible()
        {
            return _RightVisible;
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
            if (ViewingConstraint && ViewingConstraint->Description.length())
               _str2 = ViewingConstraint->Description;
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
        bool IsES()
        {
            return _ConstraintsMode == 2;
        }



        void OnlyShowEntries(bool v)
        {
            _OnlyShowEntries = v;
            Refresh(L"Left_List");
        }
        bool OnlyShowEntries()
        {
            return _OnlyShowEntries;
        }

        std::shared_ptr<XML3::XMLElement> RootOfConstraint()
        {
            auto x = project->x;
            XML3::XMLElement* root = &x->GetRootElement();
            if (_ConstraintsMode == 0)
                return root->FindElementZ("Time_Constraints_List", true);
			if (_ConstraintsMode == 1)
                return root->FindElementZ("Space_Constraints_List");
			if (_ConstraintsMode == 2)
				return root->FindElementZ(MY_FET_TAG,true)->FindElementZ("Data",true)->FindElementZ("ComboStudents", true);

            return nullptr;
        }

        bool HasEntry(XML3::XMLElement* ekx)
        {
            if (!ekx || !ViewingConstraint)
                return false;

            std::string LookFor = "Name";
            if (WhatLeftVisible == WHATVISIBLE::ACTIVITIES)
                LookFor = "Id";
            auto testwhat = trim(ekx->FindElementZ(LookFor.c_str(), true)->GetContent());
            auto r0 = RootOfConstraint();

            for (auto& c0 : *r0)
            {
                if (c0.GetElementName() != ViewingConstraint->FETXMLEntry.a_str())
                    continue;

                // If this element has
                std::string SearchElement = WhatVisibleText(WhatLeftVisible).a_str();

                for (auto& c1 : c0)
                {
                    if (c1.GetElementName() != SearchElement)
                        continue;

                    auto content = trim(c1.GetContent());
                    if (content == testwhat)
                    {
                        return true;
					}
                }
            }
            return false;
		}

        winrt::Windows::Foundation::Collections::IObservableVector<winrt::WuiFET::Item> Left_List()
        {
            auto items = winrt::single_threaded_observable_vector<winrt::WuiFET::Item>();
            auto x = project->x;


            std::vector<std::wstring> selections;
            if (LeftSelections.find((int)WhatLeftVisible) != LeftSelections.end())
            {
                for (auto& sel : LeftSelections[(int)WhatLeftVisible])
                {
                    std::string LookFor = "Name";
                    if (WhatLeftVisible == WHATVISIBLE::ACTIVITIES)
                        LookFor = "Id";
                    selections.push_back(trim(sel.ptr->FindElementZ(LookFor.c_str(), true)->GetContent()).c_str());
                }
			}

            if (WhatLeftVisible == WHATVISIBLE::TEACHERS)
            {
                // Teachers
                return GetList(x->GetRootElement()["Teachers_List"], LeftFilter.c_str(), &selections, [&](XML3::XMLElement* ekx) -> bool { if (!_OnlyShowEntries) return true; return HasEntry(ekx);  });
            }
            if (WhatLeftVisible == WHATVISIBLE::CLASSES)
            {
                // Classes
                return GetList(x->GetRootElement()["Students_List"], LeftFilter.c_str(), &selections, [&](XML3::XMLElement* ekx) -> bool { if (!_OnlyShowEntries) return true; return HasEntry(ekx);  },true);
			}
            if (WhatLeftVisible == WHATVISIBLE::LESSONS)
            {
                // Lessons
                return GetList(x->GetRootElement()["Subjects_List"], LeftFilter.c_str(), &selections, [&](XML3::XMLElement* ekx) -> bool { if (!_OnlyShowEntries) return true; return HasEntry(ekx);  });
            }
            if (WhatLeftVisible == WHATVISIBLE::ACTIVITIES)
            {
                // Activities
                return GetList(x->GetRootElement()["Activities_List"], LeftFilter.c_str(), &selections, [&](XML3::XMLElement* ekx) -> bool { if (!_OnlyShowEntries) return true; return HasEntry(ekx);  },false,project);
			}
            if (WhatLeftVisible == WHATVISIBLE::TAGS)
            {
                // Tags
                return GetList(x->GetRootElement()["Activity_Tags_List"], LeftFilter.c_str(), &selections, [&](XML3::XMLElement* ekx) -> bool { if (!_OnlyShowEntries) return true; return HasEntry(ekx);  });
			}
            if (WhatLeftVisible == WHATVISIBLE::ROOMS)
            {
                // Rooms
                return GetList(x->GetRootElement()["Rooms_List"], LeftFilter.c_str(), &selections, [&](XML3::XMLElement* ekx) -> bool { if (!_OnlyShowEntries) return true; return HasEntry(ekx);  });
            }
            return items;
        }

        // getter for leftsm
        Microsoft::UI::Xaml::Controls::ListViewSelectionMode leftsm()
        {
            if (!ViewingConstraint)
                return Microsoft::UI::Xaml::Controls::ListViewSelectionMode::Single;
            if (WhatLeftVisible == WHATVISIBLE::TEACHERS)
            {
                if (ViewingConstraint->LeftTeachers == 2)
                    return Microsoft::UI::Xaml::Controls::ListViewSelectionMode::Multiple;
                return Microsoft::UI::Xaml::Controls::ListViewSelectionMode::Single;
            }
            if (WhatLeftVisible == WHATVISIBLE::CLASSES)
            {
                if (ViewingConstraint->LeftClasses == 2)
                    return Microsoft::UI::Xaml::Controls::ListViewSelectionMode::Multiple;
                return Microsoft::UI::Xaml::Controls::ListViewSelectionMode::Single;
            }
            if (WhatLeftVisible == WHATVISIBLE::LESSONS)
            {
                if (ViewingConstraint->LeftLessons == 2)
                    return Microsoft::UI::Xaml::Controls::ListViewSelectionMode::Multiple;
                return Microsoft::UI::Xaml::Controls::ListViewSelectionMode::Single;
			}
            if (WhatLeftVisible == WHATVISIBLE::ACTIVITIES)
            {
                if (ViewingConstraint->LeftActivities == 2)
                    return Microsoft::UI::Xaml::Controls::ListViewSelectionMode::Multiple;
                return Microsoft::UI::Xaml::Controls::ListViewSelectionMode::Single;
            }
            if (WhatLeftVisible == WHATVISIBLE::TAGS)
            {
                if (ViewingConstraint->LeftTags == 2)
                    return Microsoft::UI::Xaml::Controls::ListViewSelectionMode::Multiple;
                return Microsoft::UI::Xaml::Controls::ListViewSelectionMode::Single;
			}
            if (WhatLeftVisible == WHATVISIBLE::ROOMS)
            {
                if (ViewingConstraint->LeftRooms == 2)
                    return Microsoft::UI::Xaml::Controls::ListViewSelectionMode::Multiple;
                return Microsoft::UI::Xaml::Controls::ListViewSelectionMode::Single;
			}

            return Microsoft::UI::Xaml::Controls::ListViewSelectionMode::Single;
        }

        Microsoft::UI::Xaml::Controls::ListViewSelectionMode rightsm()
        {
            if (!ViewingConstraint)
                return Microsoft::UI::Xaml::Controls::ListViewSelectionMode::Multiple;
            if (WhatRightVisible == WHATVISIBLE::ROOMS)
            {
                if (ViewingConstraint->RightRooms == 2)
                    return Microsoft::UI::Xaml::Controls::ListViewSelectionMode::Multiple;
                return Microsoft::UI::Xaml::Controls::ListViewSelectionMode::Single;
            }
            return Microsoft::UI::Xaml::Controls::ListViewSelectionMode::Multiple;
        }

        void SB_Left(IInspectable, IInspectable)
        {
            auto sb = Left_SB();
			auto it = sb.SelectedItem();
            if (!it)
                return;
            WhatLeftVisible = (WHATVISIBLE)winrt::unbox_value<int>(it.Tag());
            MustTC1 = 1;
            Refresh(L"Left_List");
            MustTC1 = 0;
        }

        void SB_Right(IInspectable, IInspectable)
        {
            auto sb = Right_SB();
            auto it = sb.SelectedItem();
            if (!it)
                return;
            WhatRightVisible = (WHATVISIBLE)winrt::unbox_value<int>(it.Tag());
            Refresh(L"Right_List");
        }


        void OnLeftFilter(IInspectable const&, IInspectable const&)
        {
            LeftFilter = fb11().Text().c_str();
            Refresh(L"Left_List");
        }

        void OnRightFilter(IInspectable const&, IInspectable const&)
        {
            RightFilter = fb11r().Text().c_str();
            Refresh(L"Right_List");
        }

        void LeftChanged(IInspectable, IInspectable)
        {
            if (MustTC1)
                return;

            if (!ViewingConstraint)
                return;

            std::string LookFor = "Name";
            if (WhatLeftVisible == WHATVISIBLE::ACTIVITIES)
				LookFor = "Id";
			
            auto lv = Left_ListView();
            LeftSelections[(int)WhatLeftVisible].clear();
            auto lvm = lv.SelectionMode();
            Current_Right_Entry = 0;
            _IsActiveVisible = false;
            _IsActiveChecked = false;
            _IsPercentageVisible = false;

            right_entries.clear();
            if (lvm == Microsoft::UI::Xaml::Controls::ListViewSelectionMode::Single)
            {
                auto sel = lv.SelectedItem();
                if (sel)
                {
                    auto it = sel.as<winrt::WuiFET::Item>();
                    if (it)
                    {
                        XML3::XMLElement* ekx = (XML3::XMLElement*)it.ptr();
                        if (ekx)
                        {
                            LEFT_SELECTION ls;
                            ls.ptr = ekx;
							ls.name = trim(ekx->FindElementZ(LookFor.c_str(), true)->GetContent()).c_str();
                            LeftSelections[(int)WhatLeftVisible].push_back(ls);
                        }
                    }
                }
            }
            else 
            if (lvm == Microsoft::UI::Xaml::Controls::ListViewSelectionMode::Multiple)
            {
                auto sels = lv.SelectedItems();
                for (uint32_t i = 0; i < sels.Size(); i++)
                {
                    auto it = sels.GetAt(i).as<winrt::WuiFET::Item>();
                    if (it)
                    {
                        XML3::XMLElement* ekx = (XML3::XMLElement*)it.ptr();
                        if (ekx)
                        {
							LEFT_SELECTION ls;
							ls.ptr = ekx;
							ls.name = trim(ekx->FindElementZ(LookFor.c_str(), true)->GetContent()).c_str();
                            LeftSelections[(int)WhatLeftVisible].push_back(ls);
                        }
                    }
                }
			}

            if (LeftSelections[(int)WhatLeftVisible].size() == 0 && ViewingConstraint->CountShow() > 0)
            {
                _RightVisible  = false;
				Refresh(L"IsRightVisible");
                return;
            }


            // Special Load
            ystring y;
            y.Format(L"%s", s(83));
            str3(y.c_str());




            // Find the entry in the XML if it exists for the selected items
            auto r0 = RootOfConstraint();
            right_entries.clear();
            for (auto& c0 : *r0)
            {
                if (c0.GetElementName() != ViewingConstraint->FETXMLEntry.a_str())
                    continue;

                // If this element has
                int CorrectCount = 0;
                for (auto& WhatLeft : LeftSelections)
                {
                    std::string SearchElement = WhatVisibleText((WHATVISIBLE)WhatLeft.first).a_str();
                    int ExtraNF = 0;
                    for (auto& c1 : c0)
                    {
                        if (c1.GetElementName() != SearchElement)
                            continue;

                        auto content = trim(c1.GetContent());
                        // See if this content is in the selected items
                        bool F = 0;
                        for (auto& sel : WhatLeft.second)
                        {
                            if (content == sel.name)
                            {
                                sel.Found = 1;
                                F = 1;
                            }
                        }
                        if (F == 0)
                            ExtraNF++;
                    }

                    bool AllFound = 1;
                    for (auto& a : WhatLeft.second)
                    {
                        if (a.Found == 0)
                        {
                            AllFound = 0;
                            break;
                        }
                    }
                    if (AllFound && ExtraNF == 0)
                    {
                        CorrectCount++;
                    }
                    for (auto& a : WhatLeft.second)
                    {
                        a.Found = 0;
                    }
                }

				if (CorrectCount == LeftSelections.size())
                    right_entries.push_back(&c0);

            }


            if (right_entries.size())
            {
                Current_Right_Entry = right_entries[0];
                _Percentage = _wtoi(trim(Current_Right_Entry->FindElementZ("Weight_Percentage", true)->GetContent()));
                Refresh(L"Percentage");
                ystring y1;
                y1.Format(L"%s #1", s(82));
                str3(y1.c_str());
            }


			_RightVisible = true;
			_IsPercentageVisible = ViewingConstraint->AllowPerc();
            _IsMultiple = ViewingConstraint->CanMultiple;
            Refresh(L"IsRightVisible");
			Refresh(L"IsMultiple");

            if (ViewingConstraint->SpecialView == 0)
            {
                // Data List
                _IsGridRightVisible = true;
				_IsListRightVisible = false;
                _IsActiveChecked = Current_Right_Entry;
                _IsActiveVisible = true;
                Refresh(L"zz1_is");
                Refresh(L"zz1_ch");
            }
            if (ViewingConstraint->SpecialView == 1)
            {
                // Special View
                _IsGridRightVisible = true;
                _IsListRightVisible = false;
                _IsActiveChecked = Current_Right_Entry;
                _IsActiveVisible = true;
                Refresh(L"zz1_is");
                Refresh(L"zz1_ch");
            }
            if (ViewingConstraint->SpecialView == 2)
            {
                // Right List
                _IsGridRightVisible = false;
                _IsListRightVisible = true;
                _IsActiveChecked = Current_Right_Entry;
                _IsActiveVisible = true;
                Refresh(L"Right_List");
            }

            Refresh(L"IsGridRightVisible");
            Refresh(L"IsListRightVisible");
			Refresh(L"IsPercentageVisible");
            Refresh(L"IsActiveChecked");
            Refresh(L"IsActiveVisible");
            RightRefresh();
            return;
        }


        void zz1_Click(IInspectable sender, IInspectable)
        {
            auto button = sender.as<DropDownButton>();
            MenuFlyout flyout;
            flyout.Placement(Microsoft::UI::Xaml::Controls::Primitives::FlyoutPlacementMode::Bottom);
            auto r0 = RootOfConstraint();


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
                        Current_Right_Entry = 0;
                        _Percentage = 100;
						_IsActiveChecked = false;
                        _IsActiveVisible = true;
						Refresh(L"IsActiveVisible");
						Refresh(L"IsActiveChecked");
                        Refresh(L"Right_List");
                        Refresh(L"Percentage");
                        Refresh(L"zz1_is");
                        Refresh(L"zz1_ch");
                        str3(s(83));
                    });
                flyout.Items().Append(item);
            }


            int yy = 0;
            for (auto& r1 : right_entries)
            {
                yy++;
                ystring y;
                auto whatper = _wtoi(trim(r1->FindElementZ("Weight_Percentage", true)->GetContent()).c_str());
                y.Format(L"%s #%i (%i%%)", s(82), yy, whatper);

                MenuFlyoutItem item;
                item.Text(y.c_str());
                item.Tag(winrt::box_value((long long)r1));
                item.Click([&](IInspectable const& sender, RoutedEventArgs const&)
                    {
                        auto it = sender.as<MenuFlyoutItem>();
                        Current_Right_Entry = (XML3::XMLElement*)unbox_value<long long>(it.Tag());
                        _Percentage = _wtoi(trim(Current_Right_Entry->FindElementZ("Weight_Percentage", true)->GetContent()).c_str());
                        _IsActiveChecked = true;
						Refresh(L"IsActiveChecked");
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


        void RightChanged(IInspectable, IInspectable)
        {
            if (MustTC0)
                return;

            auto lv = Right_ListView();
            auto lvm = lv.SelectionMode();
            std::vector<XML3::XMLElement*> rights;
            if (lvm == Microsoft::UI::Xaml::Controls::ListViewSelectionMode::Single)
            {
                auto sel = lv.SelectedItem();
				XML3::XMLElement* ekx = 0;
                if (sel)
                {
                    auto it = sel.as<winrt::WuiFET::Item>();
                    if (it)
                    {
                        ekx = (XML3::XMLElement*)it.ptr();
                        if (ekx)
                        {
                            rights.push_back(ekx);
                        }
                    }
                }
            }
			else
            {
                auto sels = lv.SelectedItems();
                for (uint32_t i = 0; i < sels.Size(); i++)
                {
                    auto it = sels.GetAt(i).as<winrt::WuiFET::Item>();
                    if (it)
                    {
                        XML3::XMLElement* ekx = (XML3::XMLElement*)it.ptr();
                        if (ekx)
                        {
                            rights.push_back(ekx);
                        }
                    }
                }
            }

            if (!Current_Right_Entry)
                return;

			std::string LookFor = "";
            std::string LookFor2 = "";
            if (WhatRightVisible == WHATVISIBLE::ROOMS)
            {
                LookFor = "Preferred_Room";
				LookFor2 = "Number_of_Preferred_Rooms";
            }

            // Remove all
            for (int i = (int)Current_Right_Entry->GetChildrenNum() - 1; i >= 0; i--)
            {
                auto& el = Current_Right_Entry->GetChildren().at(i);
                if (el->GetElementName() == LookFor)
                    Current_Right_Entry->RemoveElement(i);
            }
            if (rights.size() == 0)
            {
                // Remove it
                for (size_t i = 0; i < right_entries.size(); i++)
                {
                    if (right_entries[i] == Current_Right_Entry)
                    {
                        right_entries.erase(right_entries.begin() + i);
                        break;
                    }
                }

                auto par = Current_Right_Entry->GetParent(&project->x->GetRootElement());
                if (par)
                {
                    auto wi = WindowFromPage(*this).as<winrt::WuiFET::MainWindow>();
                    wi.Dirty();
                    par->RemoveElement(Current_Right_Entry);
                    Current_Right_Entry = nullptr;
                    _IsActiveChecked = false;
                    _Percentage = 100;
                    RightRefresh();
                }
            }
            else
            {
                // Add all selected
                for (auto el : rights)
                {
                    auto name = trim(el->FindElementZ("Name", true)->GetContent());
                    auto& e2 = Current_Right_Entry->AddElement("Preferred_Room");
                    e2.SetContent(name);
                }
                Current_Right_Entry->FindElementZ(LookFor2.c_str(), true)->SetContent(std::to_string(rights.size()).c_str());
            }


        }

        void GenericContentChanging(IInspectable const& sender,
            winrt::Microsoft::UI::Xaml::Controls::ContainerContentChangingEventArgs const& args)
        {
            if (args.InRecycleQueue())
                return;

            auto lv = sender.as<winrt::Microsoft::UI::Xaml::Controls::ListView>();
            if (!lv)
                return;
            auto lvm = lv.SelectionMode();
            if (lvm == Microsoft::UI::Xaml::Controls::ListViewSelectionMode::Single)
            {
                auto it = args.Item().try_as<winrt::WuiFET::Item>();
                if (!it)
                    return;

				auto sel = lv.SelectedItem();

                if (it.Sel())
                {
                    if (sel != it)
                    {
						lv.SelectedItem(it);
                    }
                }
            }
            else
            {
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
                    {
                        sels.Append(it);
                    }
                }
                else if (exists)
                {
                    sels.RemoveAt(idx);
                }
            }
        }

        winrt::Windows::Foundation::Collections::IObservableVector<winrt::WuiFET::Item> Right_List()
        {
            auto items = winrt::single_threaded_observable_vector<winrt::WuiFET::Item>();
            auto x = project->x;
            if (!IsActiveChecked())
                return items;
            if (WhatRightVisible == WHATVISIBLE::ROOMS)
            {
                // Rooms
                std::vector<std::wstring> sel;
                if (Current_Right_Entry)
                {
                    for (auto& el : *Current_Right_Entry)
                    {
                        if (el.GetElementName() != "Preferred_Room")
                            continue;
                        sel.push_back(trim(el.GetContent()).c_str());
                    }
				}
                return GetList(x->GetRootElement()["Rooms_List"], RightFilter.c_str(), &sel);
            }
            return items;
        }

        winrt::Windows::Foundation::Collections::IObservableVector<DGRowModel> zz1_is()
        {
            auto rows = winrt::single_threaded_observable_vector<DGRowModel>();
            if (!project->x)
                return rows;
            if (!ViewingConstraint)
                return rows;
            if (!IsActiveChecked())
				return rows;
            if (ViewingConstraint->SpecialView == 0)
            {
				// Parameter List
                for (size_t row = 0; row < ViewingConstraint->Params.size(); row++)
                {
                    auto& param = ViewingConstraint->Params[row];
                    auto cells = winrt::single_threaded_observable_vector<DGCellModel>();

                    // Name 
                    DGCellModel c1((int)row, 0, DGCellType::Block, param.Description);
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
                        c2.ValueN(param.val);
                        if (param.type == 1)
                            c2.ValueBB(param.val);

                        // If exists
                        if (Current_Right_Entry)
                        {
                            auto el = Current_Right_Entry->FindElementZ(param.FETXMLEntry.c_str(), true);
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
                    if (param.type == 4) // string
                    {
                        DGCellModel c2((int)row, 1, DGCellType::Text, param.sval);
                        c2.Ptr((long long)&param);
                        c2.Ptr3((long long)ViewingConstraint.get());
                        c2.pi2(WindowFromPage(*this));

                        // If exists
                        if (Current_Right_Entry)
                        {
                            auto el = Current_Right_Entry->FindElementZ(param.FETXMLEntry.c_str(), true);
                            c2.Ptr2((long long)el.get());
                            auto cont = trim(el->GetContent());
                            c2.Value(cont);
                        }
                        cells.Append(c2);
                    }
                    DGRowModel rr((int)row, cells);
                    rows.Append(rr);
                }
            }

            if (ViewingConstraint->SpecialView == 1)
            {
                // Showing the Not Available 
                ystring LookFor = "Not_Available_Time";
                if (WhatLeftVisible == WHATVISIBLE::ACTIVITIES)
                    LookFor = "Preferred_Starting_Time";
                if (ViewingConstraint->SpecialSubView == SUBVIEWX::SetActivities_Preferred_Starting_Times)
					LookFor = "Preferred_Starting_Time";
                if (ViewingConstraint->SpecialSubView == SUBVIEWX::SetActivities_Preferred_TimeSlots)
					LookFor = "Preferred_Time_Slot";

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
                        c1.WhatX(WhatSpecialX);

                        c1.pi1(zz1_gr());
                        c1.pi2(WindowFromPage(*this));

                        if (Current_Right_Entry) // ConstraintTeacherNotAvailableTimes
                        {
                            bool Put = 0;
                            c1.Ptr4((long long)Current_Right_Entry);
                            for (auto& el : *Current_Right_Entry)
                            {
                                if (el.GetElementName() != LookFor.a_str())
									continue;
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

                        if (LeftSelections[(int)WhatLeftVisible].size() == 1)
                        {
                            auto& sel = LeftSelections[(int)WhatLeftVisible][0];
                            c1.Ptr3((long long)sel.ptr);
                        }

                        c1.Ptr2((long long)root);
                        c1.Ptr5((long long)project);
                        cells.Append(c1);
                    }

                    DGRowModel rr(i, cells);
                    rows.Append(rr);
                }

            }


            ly1 = 1;
            return rows;
        }

        bool ly1 = 0;
        void gr1_ly(IInspectable, IInspectable)
        {
            if (ly1)
            {
              //  zz1_gr().AutoMeasure(1, 1);
                ly1 = 0;
            }

        }

        winrt::Windows::Foundation::Collections::IObservableVector<winrt::WuiFET::DGHeaderModel> zz1_ch()
        {
            auto headers = winrt::single_threaded_observable_vector<winrt::WuiFET::DGHeaderModel>();
            if (!ViewingConstraint)
				return headers;

            if (ViewingConstraint->SpecialView == 1)
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

        int Percentage()
        {
            return _Percentage;
		}
        void Percentage(int value)
        {
            if (_Percentage != value)
            {
                _Percentage = value;
                m_propertyChanged(*this, winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"Percentage"));

                if (Current_Right_Entry)
                {
                    ystring v;
                    v.Format(L"%i", value);
                    Current_Right_Entry->FindElementZ("Weight_Percentage", true)->SetContent(v);
                }
                else
                {
                    _NewPutGlobalPercNext = _Percentage;
                }

            }
		}

        bool IsPercentageVisible()
        {
            return _IsPercentageVisible && IsActiveChecked();
		}
        bool IsMultiple()
        {
            return _IsMultiple;
        }
        bool IsActiveChecked()
        {
            return _IsActiveChecked;
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
            Refresh(L"rightsm");
            Refresh(L"zz1_ch");
            Refresh(L"zz1_is");
        }

        void IsActiveChecked(bool value)
        {
            if (_IsActiveChecked != value)
            {
                _IsActiveChecked = value;
                if (!_IsActiveChecked)
                {
                    // We remove the constraint 
                    if (Current_Right_Entry)
                    {
                        auto par = Current_Right_Entry->GetParent(&project->x->GetRootElement());
                        if (par)
                        {
                            auto wi = WindowFromPage(*this).as<winrt::WuiFET::MainWindow>();
                            wi.Dirty();
                            par->RemoveElement(Current_Right_Entry);

                            // remove it from right_entries
                            for (size_t i = 0; i < right_entries.size(); i++)
                            {
                                if (right_entries[i] == Current_Right_Entry)
                                {
                                    right_entries.erase(right_entries.begin() + i);
                                    break;
                                }
							}


                            Current_Right_Entry = nullptr;


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
                        auto r0 = RootOfConstraint();
                        auto& newel = r0->AddElement(ViewingConstraint->FETXMLEntry.a_str());
                        newel.AddElement("Active").SetContent("True");
                        newel.AddElement("Weight_Percentage").SetContent("100");
                        right_entries.push_back(&newel);
                        Current_Right_Entry = &newel;
                        auto& selections = LeftSelections[(int)WhatLeftVisible];
                        for (auto& s : selections)
                        {
							auto eln = WhatVisibleText(WhatLeftVisible);
                            newel.AddElement(eln.a_str()).SetContent(s.name.a_str());

                        }
                     
                        // And the parameters
                        for (auto& param : ViewingConstraint->Params)
                        {
                            newel.AddElement(param.FETXMLEntry.c_str()).SetContent(std::to_string(param.val).c_str());
                        }
                    }
                }
                LoadExistingEntriesButton();
                Refresh(L"IsPercentageVisible");
                Refresh(L"Right_List");
                Refresh(L"IsGridRightVisible");
                Refresh(L"IsListRightVisible");
                Refresh(L"zz1_is");
                Refresh(L"zz1_ch");

            }
        }
        bool IsActiveVisible()
        {
            return _IsActiveVisible;
        }
        bool IsListRightVisible()
        {
            return _IsListRightVisible;
        }
        bool IsGridRightVisible()
        {
            return _IsGridRightVisible;
		}
        bool IsLeftSBVisible()
        {
			return _IsLeftSBVisible;
        }
        bool IsRightSBVisible()
        {
            return _IsRightSBVisible;
        };

        bool IsExistingExtriesVisible()
        {
            return ExistingEntries.size() > 0;
		}


        void bb1_Click(IInspectable sender, IInspectable)
        {
            auto button = sender.as<DropDownButton>();
            MenuFlyout flyout;
            flyout.Placement(Microsoft::UI::Xaml::Controls::Primitives::FlyoutPlacementMode::Bottom);
            auto r0 = RootOfConstraint();


            int yy = 0;
            for (auto& r1 : ExistingEntries)
            {
                yy++;
                ystring y;
                y.Format(L"%s #%03i | ", s(82), yy);
                auto search1 = WhatVisibleText(WhatLeftVisible);
                for (auto& r2 : *r1)
                {
                    if (r2.GetElementName() != search1.a_str())
						continue;
                    auto content = trim(r2.GetContent());
                    y += content;
                    y += L" ";
                }

                MenuFlyoutItem item;
                item.Text(y.c_str());
                item.Tag(winrt::box_value((long long)r1));
                item.Click([&](IInspectable const& sender, RoutedEventArgs const&)
                    {
                        auto it = sender.as<MenuFlyoutItem>();
						auto ekx = (XML3::XMLElement*)unbox_value<long long>(it.Tag());
                        LoadSelectedFrom(ekx);
                    });
                flyout.Items().Append(item);

            }
            button.Flyout(flyout);
        }


        void SelectedEntryInLeft(XML3::XMLElement* sel,const char* wherelook,bool Clear,int what)
        {
            if (sel)
            {
                std::string look = "Name";
                if (strcmp(wherelook, "Activities_List") == 0)
					look = "Id";
                   auto tn = trim(sel->GetContent());
                auto j30 = project->FindSubElementByName(&project->x->GetRootElement()[wherelook], tn.c_str(),look.c_str());
                if (strcmp(wherelook,"Students_List") == 0)
					j30 = project->FindSubElementByName2(&project->x->GetRootElement()[wherelook], tn.c_str());
                if (j30)
                {

                    LEFT_SELECTION ls;
                    ls.ptr = j30;
                    ls.name = trim(j30->FindElementZ(look.c_str(), true)->GetContent()).c_str();
                    LeftSelections[what].push_back(ls);

                    if (what != (int)WhatLeftVisible)
						return;

                    auto lv = Left_ListView();
                    auto items = lv.Items();
                    auto lvm = lv.SelectionMode();
                    if (lvm == Microsoft::UI::Xaml::Controls::ListViewSelectionMode::Single)
                    {
                        lv.SelectedItem(nullptr);
                        for (uint32_t i = 0; i < items.Size(); i++)
                        {
                            auto it = items.GetAt(i).as<winrt::WuiFET::Item>();
                            if (it)
                            {
                                XML3::XMLElement* ekx = (XML3::XMLElement*)it.ptr();
                                if (ekx == j30)
                                {
                                    lv.SelectedItem(it);
                                    lv.ScrollIntoView(it);
                                    break;
                                }
                            }
                        }
                    }
                    else
                    {
                        if (Clear)
                            lv.SelectedItems().Clear();
                        bool s1 = 0;
                        for (uint32_t i = 0; i < items.Size(); i++)
                        {
                            auto it = items.GetAt(i).as<winrt::WuiFET::Item>();
                            if (it)
                            {
                                XML3::XMLElement* ekx = (XML3::XMLElement*)it.ptr();
                                if (ekx == j30)
                                {
                                    lv.SelectedItems().Append(it);
                                    if(s1 == 0)
                                        lv.ScrollIntoView(it);
                                    s1 = 1;
                                }
                            }
                        }

                    }

                }
            }
        }

        void LoadSelectedFrom(XML3::XMLElement* ex)
        {
            if (!ex || !ViewingConstraint)
                return;
            bool Clear = true;
            LeftSelections.clear();
            for (auto& e : *ex)
            {
                if (e.GetElementName() == "Teacher")
                {
                    SelectedEntryInLeft(&e, "Teachers_List", Clear,(int)WHATVISIBLE::TEACHERS);
                    Clear = false;
                }
                if (e.GetElementName() == "Subject")
                {
                    SelectedEntryInLeft(&e, "Subjects_List", Clear, (int)WHATVISIBLE::LESSONS);
					Clear = false;
                }
                if (e.GetElementName() == "Room")
                {
                    SelectedEntryInLeft(&e, "Rooms_List",Clear, (int)WHATVISIBLE::ROOMS);
					Clear = false;
                }
                if (e.GetElementName() == "Students")
                {
                    SelectedEntryInLeft(&e, "Students_List",Clear, (int)WHATVISIBLE::CLASSES);
					Clear = false;
                }
                if (e.GetElementName() == "Tag")
                {
                    SelectedEntryInLeft(&e, "Activity_Tags_List", Clear, (int)WHATVISIBLE::TAGS);
					Clear = false;
                }
                if (e.GetElementName() == "Activity_Id")
                {
                    SelectedEntryInLeft(&e, "Activities_List",Clear, (int)WHATVISIBLE::ACTIVITIES);
					Clear = false;
                }

            }

        }





        // DATA
        ystring RightFilter;
        bool _OnlyShowEntries = false;
        std::wstring _str1, _str2, _str3, _str4; // Str1 : Time Constraints or Space Constraints // Str2 : Info bar at right
        bool MustTC0 = 0;
        bool MustTC1 = 0;
        int _LeftVisible = 0;
        int _RightVisible = 0;
        int _Percentage = 100;
        std::vector<XML3::XMLElement*> ExistingEntries;
        ystring LeftFilter;
        bool _IsPercentageVisible = false;
        bool _IsMultiple = false;
        bool _IsActiveChecked = false;
        bool _IsActiveVisible = false;
        bool _IsListRightVisible = false;
        bool _IsGridRightVisible = false;
		bool _IsLeftSBVisible = false;
		bool _IsRightSBVisible = false;

       
		WHATVISIBLE WhatLeftVisible = WHATVISIBLE::TEACHERS;
        WHATVISIBLE WhatRightVisible = WHATVISIBLE::TEACHERS;
        int WhatSpecialX = 0; // for Cell use in _whatx 
        /*
			1 - WhatSpecialX = ConstraintTeacherNotAvailableTimes
			2 - WhatSpecialX = ConstraintStudentsSetNotAvailableTimes
			5 - WhatSpecialX = ConstraintActivityPreferredStartingTimes

        */


        ystring WhatVisibleText(WHATVISIBLE wh)
        {
            ystring SearchElement;
            if (wh == WHATVISIBLE::TEACHERS)  SearchElement = "Teacher";
            if (wh == WHATVISIBLE::LESSONS)  SearchElement = "Subject";
            if (wh == WHATVISIBLE::ROOMS)  SearchElement = "Room";
            if (wh == WHATVISIBLE::CLASSES)  SearchElement = "Students";
            if (wh == WHATVISIBLE::ACTIVITIES)  SearchElement = "Activity_Id";
            if (wh == WHATVISIBLE::TAGS)  SearchElement = "Tag";
			return SearchElement;
        }

        struct LEFT_SELECTION
        {
            XML3::XMLElement* ptr = 0;
            ystring name;
            int Found = 0;
        };
		std::map<int, std::vector<LEFT_SELECTION>> LeftSelections;
        std::vector<XML3::XMLElement*> right_entries;
        XML3::XMLElement* Current_Right_Entry = 0;


#include "constraintclass.hpp"

        enum SUBVIEWX
        {
            SetActivities_Preferred_Starting_Times = 1,
            SetActivities_Preferred_TimeSlots = 2
        };

        struct CONSTRAINT
        {
            int TimeOrSpace = 0; // 0 Time 1 Space 2 output features
            ystring Description; // used for str2
            int LeftTeachers = 0; // 0 none, 1 yes, 2 multiple
            int LeftLessons = 0; // 0 none, 1 yes, 2 multiple
            int LeftRooms = 0; // 0 none, 1 yes, 2 multiple
			int LeftClasses = 0; // 0 none, 1 yes, 2 multiple
			int LeftActivities = 0; // 0 none, 1 yes, 2 multiple
			int LeftTags = 0; // 0 none, 1 yes, 2 multiple
            ystring FETXMLEntry; // e.g. ConstraintTeacherMaxGapsPerDay
            bool CanMultiple = 0;

            int SpecialView = 0; // 0 Parameter List, 1 Timetable, 2 Lists
            int SpecialSubView = 0;

            int RightRooms = 0; // 0 none, 1 yes, 2 multiple
            int RightClasses = 0; // 0 none, 1 yes, 2 multiple
            std::vector<CONSTRAINT_PARAM> Params;

            int Percentage_From = 100;
            int Percentage_To = 100;

            bool AllowPerc()
            {
				return (Percentage_From != Percentage_To) || (Percentage_From != 100);
            }

            int CountShow()
            {
                int c = 0;
                if (LeftTeachers)
                    c++;
                if (LeftLessons)
                    c++;
                if (LeftRooms)
					c++;
                if (LeftClasses)
                    c++;
                if (LeftActivities)
                    c++;
                if (LeftTags)
                    c++;
				return c;
            }



        };
		std::shared_ptr<CONSTRAINT> ViewingConstraint = 0;


        void LoadExistingEntriesButton()
        {            
            ExistingEntries.clear();
            if (!ViewingConstraint)
                return;
            
            // Check the existing entries
            auto r0 = RootOfConstraint();
            for (auto& el : *r0)
            {
                if (el.GetElementName() != ViewingConstraint->FETXMLEntry.a_str())
                    continue;
                ExistingEntries.push_back(&el);
            }
            ystring yy;
            yy.Format(L"%zi %s", ExistingEntries.size(), ExistingEntries.size() == 1 ? s(195) : s(194));
            str4(yy.c_str());
            Refresh(L"IsExistingExtriesVisible");
        }

        void LoadConstraint(CONSTRAINT& c)
        {
            _Percentage = 100;
            if (c.TimeOrSpace == 0)
				str1(s(19));
            if (c.TimeOrSpace == 1)
                str1(s(20));
            if (c.TimeOrSpace == 2)
				str1(s(187));
			str2(c.Description.c_str());
            LeftFilter.clear();
            RightFilter.clear();
            LeftSelections.clear();
            Current_Right_Entry = 0;
            right_entries.clear();
            _IsGridRightVisible = false;
            _IsListRightVisible = false;

            // What to visible from left
            int CountShowingLeft = c.CountShow();

            if (CountShowingLeft == 0)
                _LeftVisible = 0;
			else
                _LeftVisible = 1;


            // fill the left items
            WhatLeftVisible = WHATVISIBLE::TEACHERS;
            _IsLeftSBVisible = false;
            WhatRightVisible = WHATVISIBLE::TEACHERS;
            _IsRightSBVisible = 0;
            auto sbl = Left_SB();
			sbl.Items().Clear();
            if (c.LeftTags)
            {
                SelectorBarItem it;
                it.Text(s(29));
                it.Tag(winrt::box_value<int>(5));
                sbl.Items().InsertAt(0, it);
                WhatLeftVisible = WHATVISIBLE::TAGS;
            }
            if (c.LeftActivities)
            {
                SelectorBarItem it;
                it.Text(s(25));
                it.Tag(winrt::box_value<int>(4));
                sbl.Items().InsertAt(0, it);
                WhatLeftVisible = WHATVISIBLE::ACTIVITIES;
            }
            if (c.LeftClasses)
            {
                SelectorBarItem it;
                it.Text(s(55));
                it.Tag(winrt::box_value<int>(3));
                sbl.Items().InsertAt(0, it);
                WhatLeftVisible = WHATVISIBLE::CLASSES;
            }
            if (c.LeftRooms)
            {
                SelectorBarItem it;
                it.Text(s(51));
                it.Tag(winrt::box_value<int>(2));
                sbl.Items().InsertAt(0, it);
                WhatLeftVisible = WHATVISIBLE::ROOMS;
            }
            if (c.LeftLessons)
            {
                SelectorBarItem it;
                it.Text(s(26));
                it.Tag(winrt::box_value<int>(1));
                sbl.Items().InsertAt(0, it);
                WhatLeftVisible = WHATVISIBLE::LESSONS;
            }
            if (c.LeftTeachers)
            {
                SelectorBarItem it;
				it.Text(s(24));
				it.Tag(winrt::box_value<int>(0));
				sbl.Items().InsertAt(0,it);
				WhatLeftVisible = WHATVISIBLE::TEACHERS;
            }
            if (sbl.Items().Size() > 1)
            {
                _IsLeftSBVisible = true;
			}


            if (CountShowingLeft == 0)
            {
                LeftChanged({},{});
            }


            auto sbr = Right_SB();
            sbr.Items().Clear();
            if (c.RightClasses)
            {
                SelectorBarItem it;
                it.Text(s(55));
                it.Tag(winrt::box_value<int>(3));
                sbl.Items().InsertAt(0, it);
                WhatRightVisible = WHATVISIBLE::CLASSES;
            }
            if (c.RightRooms)
            {
                SelectorBarItem it;
                it.Text(s(51));
                it.Tag(winrt::box_value<int>(2));
                sbl.Items().InsertAt(0, it);
                WhatRightVisible = WHATVISIBLE::ROOMS;
            }
            if (sbr.Items().Size() > 1)
            {
                _IsRightSBVisible = true;
            }


           
            LoadExistingEntriesButton();
            Refresh(L"leftsm");
            Refresh(L"Left_List");
            Refresh(L"IsLeftVisible");
            Refresh(L"IsLeftSBVisible");
			Refresh(L"IsRightSBVisible");
        }


        // Time Constraints
        void TS_None_Single1(ystring desc, const char* f1, const char* pn1, int from = 0, int to = 0, int val = 0, int percfrom = 100, int percto = 100, bool mult = 0)
        {
            ViewingConstraint = std::make_shared<CONSTRAINT>();
            ViewingConstraint->TimeOrSpace = 0;
            ViewingConstraint->Description = desc;
            ViewingConstraint->FETXMLEntry = f1;
            ViewingConstraint->SpecialView = 0; // Parameter List
            ViewingConstraint->CanMultiple = mult;
            ViewingConstraint->Percentage_From = percfrom;
            ViewingConstraint->Percentage_To = percto;

            if (pn1)
            {
                CONSTRAINT_PARAM p1;
                p1.FETXMLEntry = pn1;
                p1.Description = desc;
                p1.from = from;
                p1.to = to;
                p1.val = val;
                p1.type = 0;
                ViewingConstraint->Params.push_back(p1);
            }

            LoadConstraint(*ViewingConstraint);
        }

        // Teacher

        void TS_Teacher_NotAvailableTimes(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            ViewingConstraint = std::make_shared<CONSTRAINT>();
            ViewingConstraint->TimeOrSpace = 0;
            ViewingConstraint->Description = s(77);
            ViewingConstraint->LeftTeachers = 1;
            ViewingConstraint->FETXMLEntry = "ConstraintTeacherNotAvailableTimes";
            ViewingConstraint->SpecialView = 1;
            WhatSpecialX = 1;
            ViewingConstraint->CanMultiple = true;
            ViewingConstraint->Percentage_From = 0;
            ViewingConstraint->Percentage_To = 100;
            LoadConstraint(*ViewingConstraint);
        }

        void TS_Teacher_Single1(ystring desc,const char* f1,const char* pn1,int from = 0,int to = 0,int val = 0,int percfrom = 100,int percto = 100,bool mult = 0)
        {
            ViewingConstraint = std::make_shared<CONSTRAINT>();
            ViewingConstraint->TimeOrSpace = 0;
            ViewingConstraint->Description = desc;
            ViewingConstraint->LeftTeachers = 1;
            ViewingConstraint->FETXMLEntry = f1;
            ViewingConstraint->SpecialView = 0; // Parameter List
            ViewingConstraint->CanMultiple = mult;
            ViewingConstraint->Percentage_From = percfrom;
            ViewingConstraint->Percentage_To = percto;

            if (pn1)
            {
                CONSTRAINT_PARAM p1;
                p1.FETXMLEntry = pn1;
                p1.Description = desc;
                p1.from = from;
                p1.to = to;
                p1.val = val;
                p1.type = 0;
                ViewingConstraint->Params.push_back(p1);
            }

			LoadConstraint(*ViewingConstraint);
        }
        void TS_Teacher_MaxGapsDay(IInspectable const&, IInspectable const&)
        {
            TS_Teacher_Single1(s(96),"ConstraintTeacherMaxGapsPerDay", "Max_Gaps",0, project->HowManyHours(), project->HowManyHours(),100,100,false);
		}
        void TS_Teacher_MinHoursDay(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            TS_Teacher_Single1(s(104), "ConstraintTeacherMinHoursDaily", "Minimum_Hours_Daily", 0, project->HowManyHours(), project->HowManyHours(), 100, 100, false);

        }
        void TS_Teacher_MaxHoursDay(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            TS_Teacher_Single1(s(99), "ConstraintTeacherMaxHoursDaily", "Maximum_Hours_Daily", 0, project->HowManyHours(), project->HowManyHours(), 100, 100, false);

        }
        void TS_Teacher_MaxSpanDay(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            TS_Teacher_Single1(s(102), "ConstraintTeacherMaxSpanPerDay", "Max_Span", 0, project->HowManyHours(), project->HowManyHours(), 100, 100, false);
            CONSTRAINT_PARAM p2;
            p2.Description = s(103);
            p2.type = 1;
            p2.FETXMLEntry = "Allow_One_Day_Exception_of_Plus_One";
            p2.from = 0;
            p2.to = 1;
            p2.val = 0;
            ViewingConstraint->Params.push_back(p2);
            LoadConstraint(*ViewingConstraint);

        }
        void TS_Teacher_MaxContinuousHours(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            TS_Teacher_Single1(s(105), "ConstraintTeacherMaxHoursContinuously", "Maximum_Hours_Continuously", 0, project->HowManyHours(), project->HowManyHours(), 100, 100, false);

        }
        void TS_Teacher_MinDaysWeek(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            TS_Teacher_Single1(s(93), "ConstraintTeacherMinDaysPerWeek", "Minimum_Days_Per_Week", 0, project->HowManyDays(), project->HowManyDays(), 100, 100, false);

        }
        void TS_Teacher_MaxDaysWeek(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            TS_Teacher_Single1(s(94), "ConstraintTeacherMaxDaysPerWeek", "Max_Days_Per_Week", 0, project->HowManyDays(), project->HowManyDays(), 100, 100, false);

        }
        void TS_Teacher_MaxGapsWeek(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            TS_Teacher_Single1(s(97), "ConstraintTeacherMaxGapsPerWeek", "Max_Gaps", 0, project->HowManyHours() * project->HowManyDays(), project->HowManyHours() * project->HowManyDays(), 100, 100, false);

        }
        void TS_Teacher_NoTwoDays(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            TS_Teacher_Single1(s(162), "ConstraintTeacherNoTwoConsecutiveDays", 0, 0, project->HowManyDays(), project->HowManyDays(), 100, 100, false);
        }


        // Teachers


        void TS_Teachers_MaxGapsDay(IInspectable const&, IInspectable const&)
        {
            TS_None_Single1(s(96), "ConstraintTeachersMaxGapsPerDay", "Max_Gaps", 0, project->HowManyHours(), project->HowManyHours(), 100, 100, false);
        }
        void TS_Teachers_MinHoursDay(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            TS_None_Single1(s(104), "ConstraintTeachersMinHoursDaily", "Minimum_Hours_Daily", 0, project->HowManyHours(), project->HowManyHours(), 100, 100, false);

        }
        void TS_Teachers_MaxHoursDay(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            TS_None_Single1(s(99), "ConstraintTeachersMaxHoursDaily", "Maximum_Hours_Daily", 0, project->HowManyHours(), project->HowManyHours(), 100, 100, false);

        }
        void TS_Teachers_MaxSpanDay(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            TS_None_Single1(s(102), "ConstraintTeachersMaxSpanPerDay", "Max_Span", 0, project->HowManyHours(), project->HowManyHours(), 100, 100, false);
            CONSTRAINT_PARAM p2;
            p2.Description = s(103);
            p2.type = 1;
            p2.FETXMLEntry = "Allow_One_Day_Exception_of_Plus_One";
            p2.from = 0;
            p2.to = 1;
            p2.val = 0;
            ViewingConstraint->Params.push_back(p2);
            LoadConstraint(*ViewingConstraint);
        }
        void TS_Teachers_MaxContinuousHours(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            TS_None_Single1(s(105), "ConstraintTeachersMaxHoursContinuously", "Maximum_Hours_Continuously", 0, project->HowManyHours(), project->HowManyHours(), 100, 100, false);

        }
        void TS_Teachers_MinDaysWeek(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            TS_None_Single1(s(93), "ConstraintTeachersMinDaysPerWeek", "Minimum_Days_Per_Week", 0, project->HowManyDays(), project->HowManyDays(), 100, 100, false);

        }
        void TS_Teachers_MaxDaysWeek(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            TS_None_Single1(s(94), "ConstraintTeachersMaxDaysPerWeek", "Max_Days_Per_Week", 0, project->HowManyDays(), project->HowManyDays(), 100, 100, false);

        }
        void TS_Teachers_MaxGapsWeek(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            TS_None_Single1(s(97), "ConstraintTeachersMaxGapsPerWeek", "Max_Gaps", 0, project->HowManyHours() * project->HowManyDays(), project->HowManyHours() * project->HowManyDays(), 100, 100, false);
        }


        // Class
        void TS_Class_NotAvailableTimes(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            ViewingConstraint = std::make_shared<CONSTRAINT>();
            ViewingConstraint->TimeOrSpace = 0;
            ViewingConstraint->Description = s(77);
            ViewingConstraint->LeftClasses = 1;
            ViewingConstraint->FETXMLEntry = "ConstraintStudentsSetNotAvailableTimes";
            ViewingConstraint->SpecialView = 1;
            WhatSpecialX = 2;
            ViewingConstraint->CanMultiple = true;
            ViewingConstraint->Percentage_From = 0;
            ViewingConstraint->Percentage_To = 100;
            LoadConstraint(*ViewingConstraint);
        }

        // Activity

        void TS_Activity_BeginClassDay(auto, auto)
        {
            ViewingConstraint = std::make_shared<CONSTRAINT>();
            ViewingConstraint->TimeOrSpace = 0;
            ViewingConstraint->Description = s(205);
            ViewingConstraint->LeftActivities  = 1;
            ViewingConstraint->FETXMLEntry = "ConstraintActivityBeginsStudentsDay";
            ViewingConstraint->SpecialView = 0;
            WhatSpecialX = 0;
            ViewingConstraint->CanMultiple = 0;
            ViewingConstraint->Percentage_From = 0;
            ViewingConstraint->Percentage_To = 100;
            LoadConstraint(*ViewingConstraint);
        }
        void TS_Activity_BeginTeacherDay(auto, auto)
        {
            ViewingConstraint = std::make_shared<CONSTRAINT>();
            ViewingConstraint->TimeOrSpace = 0;
            ViewingConstraint->Description = s(205);
            ViewingConstraint->LeftActivities = 1;
            ViewingConstraint->FETXMLEntry = "ConstraintActivityBeginsTeacherDay";
            ViewingConstraint->SpecialView = 0;
            WhatSpecialX = 0;
            ViewingConstraint->CanMultiple = 0;
            ViewingConstraint->Percentage_From = 0;
            ViewingConstraint->Percentage_To = 100;
            LoadConstraint(*ViewingConstraint);
        }
        void TS_Activity_EndClassDay(auto, auto)
        {
            ViewingConstraint = std::make_shared<CONSTRAINT>();
            ViewingConstraint->TimeOrSpace = 0;
            ViewingConstraint->Description = s(206);
            ViewingConstraint->LeftActivities = 1;
            ViewingConstraint->FETXMLEntry = "ConstraintActivityEndsStudentsDay";
            ViewingConstraint->SpecialView = 0;
            WhatSpecialX = 0;
            ViewingConstraint->CanMultiple = 0;
            ViewingConstraint->Percentage_From = 0;
            ViewingConstraint->Percentage_To = 100;
            LoadConstraint(*ViewingConstraint);
        }
        void TS_Activity_EndTeacherDay(auto, auto)
        {
            ViewingConstraint = std::make_shared<CONSTRAINT>();
            ViewingConstraint->TimeOrSpace = 0;
            ViewingConstraint->Description = s(206);
            ViewingConstraint->LeftActivities = 1;
            ViewingConstraint->FETXMLEntry = "ConstraintActivityEndsTeacherDay";
            ViewingConstraint->SpecialView = 0;
            WhatSpecialX = 0;
            ViewingConstraint->CanMultiple = 0;
            ViewingConstraint->Percentage_From = 0;
            ViewingConstraint->Percentage_To = 100;
            LoadConstraint(*ViewingConstraint);
        }

        void TS_Activities_MaxDaysBetween(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            ViewingConstraint = std::make_shared<CONSTRAINT>();
            ViewingConstraint->TimeOrSpace = 0;
            ViewingConstraint->Description = s(203);
            ViewingConstraint->FETXMLEntry = "ConstraintMaxDaysBetweenActivities";
            ViewingConstraint->SpecialView = 0;
            ViewingConstraint->LeftActivities = 2;
            ViewingConstraint->CanMultiple = true;
            ViewingConstraint->Percentage_From = 0;
            ViewingConstraint->Percentage_To = 100;

            if (1)
            {
                CONSTRAINT_PARAM p1;
                p1.Description = s(203);
                p1.type = 0;
                p1.FETXMLEntry = "MaxDays";
                p1.from = 0;
                p1.to = project->HowManyDays();
                p1.val = p1.to;
                ViewingConstraint->Params.push_back(p1);
            }

            LoadConstraint(*ViewingConstraint);
        }
        void TS_Activities_MinDaysBetween(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            ViewingConstraint = std::make_shared<CONSTRAINT>();
            ViewingConstraint->TimeOrSpace = 0;
            ViewingConstraint->Description = s(177);
            ViewingConstraint->FETXMLEntry = "ConstraintMinDaysBetweenActivities";
            ViewingConstraint->SpecialView = 0;
            ViewingConstraint->LeftActivities = 2;
            ViewingConstraint->CanMultiple = true;
            ViewingConstraint->Percentage_From = 0;
            ViewingConstraint->Percentage_To = 100;

            if (1)
            {
                CONSTRAINT_PARAM p1;
                p1.Description = s(177);
                p1.type = 0;
                p1.FETXMLEntry = "MinDays";
                p1.from = 0;
                p1.to = project->HowManyDays();
                p1.val = p1.to;
                ViewingConstraint->Params.push_back(p1);
            }

            LoadConstraint(*ViewingConstraint);
        }

        void TS_Activity_PreferredStartingTimes(auto,auto)
        {
            ViewingConstraint = std::make_shared<CONSTRAINT>();
            ViewingConstraint->TimeOrSpace = 0;
            ViewingConstraint->Description = s(177);
            ViewingConstraint->FETXMLEntry = "ConstraintActivityPreferredStartingTimes";
            ViewingConstraint->SpecialView = 1;
            WhatSpecialX = 5;
            ViewingConstraint->LeftActivities = 1;
            ViewingConstraint->CanMultiple = true;
            ViewingConstraint->Percentage_From = 0;
            ViewingConstraint->Percentage_To = 100;
            LoadConstraint(*ViewingConstraint);
        }

        void TS_Activity_PreferredTimeSlots(auto, auto)
        {
            ViewingConstraint = std::make_shared<CONSTRAINT>();
            ViewingConstraint->TimeOrSpace = 0;
            ViewingConstraint->Description = s(177);
            ViewingConstraint->FETXMLEntry = "ConstraintActivityPreferredTimeSlots";
            ViewingConstraint->SpecialView = 1;
            WhatSpecialX =6;
            ViewingConstraint->LeftActivities = 1;
            ViewingConstraint->CanMultiple = true;
            ViewingConstraint->Percentage_From = 0;
            ViewingConstraint->Percentage_To = 100;
            LoadConstraint(*ViewingConstraint);
        }

        // Activities
        void TS_Activities_PreferredStartingTimes(auto, auto)
        {
            ViewingConstraint = std::make_shared<CONSTRAINT>();
            ViewingConstraint->TimeOrSpace = 0;
            ViewingConstraint->Description = s(163);
            ViewingConstraint->FETXMLEntry = "ConstraintActivitiesPreferredStartingTimes";
            ViewingConstraint->SpecialView = 1;
			ViewingConstraint->SpecialSubView = SUBVIEWX::SetActivities_Preferred_Starting_Times;
            WhatSpecialX = 15;
            ViewingConstraint->LeftTeachers = 1;
            ViewingConstraint->LeftClasses = 1;
            ViewingConstraint->LeftLessons = 1;
            ViewingConstraint->LeftTags = 1;
            ViewingConstraint->CanMultiple = true;
            ViewingConstraint->Percentage_From = 0;
            ViewingConstraint->Percentage_To = 100;
            LoadConstraint(*ViewingConstraint);
        }

        void TS_Activities_PreferredTimeSlots(auto, auto)
        {
            ViewingConstraint = std::make_shared<CONSTRAINT>();
            ViewingConstraint->TimeOrSpace = 0;
            ViewingConstraint->Description = s(168);
            ViewingConstraint->FETXMLEntry = "ConstraintActivitiesPreferredTimeSlots";
            ViewingConstraint->SpecialView = 1;
			ViewingConstraint->SpecialSubView = SUBVIEWX::SetActivities_Preferred_TimeSlots;
            WhatSpecialX = 16;
            ViewingConstraint->LeftTeachers = 1;
            ViewingConstraint->LeftClasses = 1;
            ViewingConstraint->LeftLessons = 1;
            ViewingConstraint->LeftTags = 1;
            ViewingConstraint->CanMultiple = true;
            ViewingConstraint->Percentage_From = 0;
            ViewingConstraint->Percentage_To = 100;
            LoadConstraint(*ViewingConstraint);
        }
        // Space Constraints


        // Room

        void SS_Room_NotAvailableTimes(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            ViewingConstraint = std::make_shared<CONSTRAINT>();
            ViewingConstraint->TimeOrSpace = 1;
            ViewingConstraint->Description = s(77);
            ViewingConstraint->LeftRooms = 1;
            ViewingConstraint->FETXMLEntry = "ConstraintRoomNotAvailableTimes";
            ViewingConstraint->SpecialView = 1;
            WhatSpecialX = 7;
            ViewingConstraint->CanMultiple = true;
            ViewingConstraint->Percentage_From = 0;
            ViewingConstraint->Percentage_To = 100;
            LoadConstraint(*ViewingConstraint);
        }

        // Teacher
        void SS_Teacher_Single1(ystring desc, const char* f1, const char* pn1, int from = 0, int to = 0, int val = 0, int percfrom = 100, int percto = 100, bool mult = 0)
        {
            ViewingConstraint = std::make_shared<CONSTRAINT>();
            ViewingConstraint->TimeOrSpace = 1;
            ViewingConstraint->Description = desc;
            ViewingConstraint->LeftTeachers = 1;
            ViewingConstraint->FETXMLEntry = f1;
            ViewingConstraint->SpecialView = 0; // Parameter List
            ViewingConstraint->CanMultiple = mult;
            ViewingConstraint->Percentage_From = percfrom;
            ViewingConstraint->Percentage_To = percto;

            if (pn1)
            {
                CONSTRAINT_PARAM p1;
                p1.FETXMLEntry = pn1;
                p1.Description = desc;
                p1.from = from;
                p1.to = to;
                p1.val = val;
                p1.type = 0;
                ViewingConstraint->Params.push_back(p1);
            }
			LoadConstraint(*ViewingConstraint);
        }


        void SS_Teacher_MaxRoomChangesDay(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            SS_Teacher_Single1(s(139), "ConstraintTeacherMaxRoomChangesPerDay", "Max_Room_Changes_Per_Day", 0, project->HowManyHours(), project->HowManyHours(), 100, 100, false);
        }
        void SS_Teacher_MaxRoomChangesWeek(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            SS_Teacher_Single1(s(139), "ConstraintTeacherMaxRoomChangesPerWeek", "Max_Room_Changes_Per_Week", 0, project->HowManyHours(), project->HowManyHours(), 100, 100, false);
        }

        void SS_Teacher_MaxRoomChangesDayInterval(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            SS_Teacher_Single1(s(140), "ConstraintTeacherMaxRoomChangesPerDayInInterval", "Max_Room_Changes_Per_Day", 0, project->HowManyHours(), project->HowManyHours(), 100, 100, false);
            if (1)
            {
                CONSTRAINT_PARAM p1;
                p1.Description = s(141);
                p1.type = 0;
                p1.FETXMLEntry = "Interval_Start_Hour";
                p1.from = 0;
                p1.to = project->HowManyHours();
                p1.val = 0;
                ViewingConstraint->Params.push_back(p1);
            }

            if (1)
            {
                CONSTRAINT_PARAM p1;
                p1.Description = s(142);
                p1.type = 0;
                p1.FETXMLEntry = "Interval_End_Hour";
                p1.from = 0;
                p1.to = project->HowManyHours();
                p1.val = p1.to;
                ViewingConstraint->Params.push_back(p1);
            }
        }
        void SS_Teacher_MaxBuildingChangesDay(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            SS_Teacher_Single1(s(143), "ConstraintTeacherMaxBuildingChangesPerDay", "Max_Building_Changes_Per_Day", 0, project->HowManyHours(), project->HowManyHours(), 100, 100, false);
        }
        void SS_Teacher_MaxBuildingChangesWeek(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            SS_Teacher_Single1(s(143), "ConstraintTeacherMaxBuildingChangesPerWeek", "Max_Building_Changes_Per_Week", 0, project->HowManyHours(), project->HowManyHours(), 100, 100, false);
        }
        void SS_Teacher_MaxBuildingChangesDayInterval(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            SS_Teacher_Single1(s(143), "ConstraintTeacherMaxBuildingChangesPerDayInInterval", "Max_Building_Changes_Per_Day", 0, project->HowManyHours(), project->HowManyHours(), 100, 100, false);

            if (1)
            {
                CONSTRAINT_PARAM p1;
                p1.Description = s(141);
                p1.type = 0;
                p1.FETXMLEntry = "Interval_Start_Hour";
                p1.from = 0;
                p1.to = project->HowManyHours();
                p1.val = 0;
                ViewingConstraint->Params.push_back(p1);
            }

            if (1)
            {
                CONSTRAINT_PARAM p1;
                p1.Description = s(142);
                p1.type = 0;
                p1.FETXMLEntry = "Interval_End_Hour";
                p1.from = 0;
                p1.to = project->HowManyHours();
                p1.val = p1.to;
                ViewingConstraint->Params.push_back(p1);
            }
        }

        void SS_Teacher_PreferredRooms(IInspectable, IInspectable)
        {
            ViewingConstraint = std::make_shared<CONSTRAINT>();
            ViewingConstraint->TimeOrSpace = 1;
            ViewingConstraint->Description = s(146);
            ViewingConstraint->LeftTeachers = 1;
            ViewingConstraint->FETXMLEntry = "ConstraintTeacherPreferredRooms";
            ViewingConstraint->SpecialView = 2;
            ViewingConstraint->RightRooms = 2; 
            ViewingConstraint->CanMultiple = true;
            ViewingConstraint->Percentage_From = 0;
            ViewingConstraint->Percentage_To = 100;
            LoadConstraint(*ViewingConstraint);

        }


        // Teachers
        void SS_None_Single1(ystring desc, const char* f1, const char* pn1, int from = 0, int to = 0, int val = 0, int percfrom = 100, int percto = 100, bool mult = 0)
        {
            ViewingConstraint = std::make_shared<CONSTRAINT>();
            ViewingConstraint->TimeOrSpace = 1;
            ViewingConstraint->Description = desc;
            ViewingConstraint->FETXMLEntry = f1;
            ViewingConstraint->SpecialView = 0; // Parameter List
            ViewingConstraint->CanMultiple = mult;
            ViewingConstraint->Percentage_From = percfrom;
            ViewingConstraint->Percentage_To = percto;

            if (pn1)
            {
                CONSTRAINT_PARAM p1;
                p1.FETXMLEntry = pn1;
                p1.Description = desc;
                p1.from = from;
                p1.to = to;
                p1.val = val;
                p1.type = 0;
                ViewingConstraint->Params.push_back(p1);
            }

            LoadConstraint(*ViewingConstraint);
        }



        void SS_Teachers_MaxRoomChangesDay(auto, auto)
        {
            SS_None_Single1(s(139), "ConstraintTeachersMaxRoomChangesPerDay", "Max_Room_Changes_Per_Day", 0, project->HowManyHours(), project->HowManyHours(), 100, 100, false);

        }
        void SS_Teachers_MaxRoomChangesDayInterval(auto, auto)
        {
            SS_None_Single1(s(140), "ConstraintTeachersMaxRoomChangesPerDayInInterval", "Max_Room_Changes_Per_Day", 0, project->HowManyHours(), project->HowManyHours(), 100, 100, false);
            if (1)
            {
                CONSTRAINT_PARAM p1;
                p1.Description = s(141);
                p1.type = 0;
                p1.FETXMLEntry = "Interval_Start_Hour";
                p1.from = 0;
                p1.to = project->HowManyHours();
                p1.val = 0;
                ViewingConstraint->Params.push_back(p1);
            }

            if (1)
            {
                CONSTRAINT_PARAM p1;
                p1.Description = s(142);
                p1.type = 0;
                p1.FETXMLEntry = "Interval_End_Hour";
                p1.from = 0;
                p1.to = project->HowManyHours();
                p1.val = p1.to;
                ViewingConstraint->Params.push_back(p1);
            }

        }
        void SS_Teachers_MaxBuildingChangesDay(auto, auto)
        {
            SS_None_Single1(s(143), "ConstraintTeachersMaxBuildingChangesPerDay", "Max_Building_Changes_Per_Day", 0, project->HowManyHours(), project->HowManyHours(), 100, 100, false);

        }
        void SS_Teachers_MaxRoomChangesWeek(auto, auto)
        {
            SS_None_Single1(s(139), "ConstraintTeachersMaxRoomChangesPerWeek", "Max_Room_Changes_Per_Week", 0, project->HowManyHours(), project->HowManyHours(), 100, 100, false);
        }
        void SS_Teachers_MaxRoomChangesWeekInterval(auto, auto)
        {

        }
        void SS_Teachers_MaxBuildingChangesWeek(auto, auto)
        {
            SS_None_Single1(s(143), "ConstraintTeachersMaxBuildingChangesPerWeek", "Max_Building_Changes_Per_Week", 0, project->HowManyHours(), project->HowManyHours(), 100, 100, false);

        }

        // Class
        void SS_Class_PreferredRooms(IInspectable, IInspectable)
        {
            ViewingConstraint = std::make_shared<CONSTRAINT>();
            ViewingConstraint->TimeOrSpace = 1;
            ViewingConstraint->Description = s(146);
            ViewingConstraint->LeftClasses = 1;
            ViewingConstraint->FETXMLEntry = "ConstraintStudentsSetPreferredRooms";
            ViewingConstraint->SpecialView = 2;
            ViewingConstraint->RightRooms = 2;
            ViewingConstraint->CanMultiple = true;
            ViewingConstraint->Percentage_From = 0;
            ViewingConstraint->Percentage_To = 100;
            LoadConstraint(*ViewingConstraint);

        }

        void SS_Class_Single1(ystring desc, const char* f1, const char* pn1, int from = 0, int to = 0, int val = 0, int percfrom = 100, int percto = 100, bool mult = 0)
        {
            ViewingConstraint = std::make_shared<CONSTRAINT>();
            ViewingConstraint->TimeOrSpace = 1;
            ViewingConstraint->Description = desc;
            ViewingConstraint->LeftClasses = 1;
            ViewingConstraint->FETXMLEntry = f1;
            ViewingConstraint->SpecialView = 0; // Parameter List
            ViewingConstraint->CanMultiple = mult;
            ViewingConstraint->Percentage_From = percfrom;
            ViewingConstraint->Percentage_To = percto;

            if (pn1)
            {
                CONSTRAINT_PARAM p1;
                p1.FETXMLEntry = pn1;
                p1.Description = desc;
                p1.from = from;
                p1.to = to;
                p1.val = val;
                p1.type = 0;
                ViewingConstraint->Params.push_back(p1);
            }
            LoadConstraint(*ViewingConstraint);
        }

        void SS_Class_MaxRoomChangesDay(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            SS_Class_Single1(s(139), "ConstraintStudentsSetMaxRoomChangesPerDay", "Max_Room_Changes_Per_Day", 0, project->HowManyHours(), project->HowManyHours(), 100, 100, false);
        }
        void SS_Class_MaxRoomChangesWeek(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            SS_Class_Single1(s(139), "ConstraintStudentsSetMaxRoomChangesPerWeek", "Max_Room_Changes_Per_Week", 0, project->HowManyHours(), project->HowManyHours(), 100, 100, false);
        }

        void SS_Class_MaxRoomChangesDayInterval(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            SS_Class_Single1(s(140), "ConstraintStudentsSetMaxRoomChangesPerDayInInterval", "Max_Room_Changes_Per_Day", 0, project->HowManyHours(), project->HowManyHours(), 100, 100, false);
            if (1)
            {
                CONSTRAINT_PARAM p1;
                p1.Description = s(141);
                p1.type = 0;
                p1.FETXMLEntry = "Interval_Start_Hour";
                p1.from = 0;
                p1.to = project->HowManyHours();
                p1.val = 0;
                ViewingConstraint->Params.push_back(p1);
            }

            if (1)
            {
                CONSTRAINT_PARAM p1;
                p1.Description = s(142);
                p1.type = 0;
                p1.FETXMLEntry = "Interval_End_Hour";
                p1.from = 0;
                p1.to = project->HowManyHours();
                p1.val = p1.to;
                ViewingConstraint->Params.push_back(p1);
            }
        }
        void SS_Class_MaxBuildingChangesDay(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            SS_Class_Single1(s(143), "ConstraintStudentsSetMaxBuildingChangesPerDay", "Max_Building_Changes_Per_Day", 0, project->HowManyHours(), project->HowManyHours(), 100, 100, false);
        }

        void SS_Class_MaxBuildingChangesDayInterval(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            SS_Class_Single1(s(143), "ConstraintStudentsSetMaxBuildingChangesPerDayInInterval", "Max_Building_Changes_Per_Day", 0, project->HowManyHours(), project->HowManyHours(), 100, 100, false);

            if (1)
            {
                CONSTRAINT_PARAM p1;
                p1.Description = s(141);
                p1.type = 0;
                p1.FETXMLEntry = "Interval_Start_Hour";
                p1.from = 0;
                p1.to = project->HowManyHours();
                p1.val = 0;
                ViewingConstraint->Params.push_back(p1);
            }

            if (1)
            {
                CONSTRAINT_PARAM p1;
                p1.Description = s(142);
                p1.type = 0;
                p1.FETXMLEntry = "Interval_End_Hour";
                p1.from = 0;
                p1.to = project->HowManyHours();
                p1.val = p1.to;
                ViewingConstraint->Params.push_back(p1);
            }
        }


        // Lesson
        void SS_Lesson_PreferredRooms(IInspectable, IInspectable)
        {
            ViewingConstraint = std::make_shared<CONSTRAINT>();
            ViewingConstraint->TimeOrSpace = 1;
            ViewingConstraint->Description = s(146);
            ViewingConstraint->LeftLessons = 1;
            ViewingConstraint->FETXMLEntry = "ConstraintSubjectPreferredRooms";
            ViewingConstraint->SpecialView = 2;
            ViewingConstraint->RightRooms = 2;
            ViewingConstraint->CanMultiple = true;
            ViewingConstraint->Percentage_From = 0;
            ViewingConstraint->Percentage_To = 100;
            LoadConstraint(*ViewingConstraint);

        }


        // Activity
        void SS_Activity_PreferredRooms(IInspectable, IInspectable)
        {
            ViewingConstraint = std::make_shared<CONSTRAINT>();
            ViewingConstraint->TimeOrSpace = 1;
            ViewingConstraint->Description = s(146);
            ViewingConstraint->LeftActivities = 1;
            ViewingConstraint->FETXMLEntry = "ConstraintActivityPreferredRooms";
            ViewingConstraint->SpecialView = 2;
            ViewingConstraint->RightRooms = 2;
            ViewingConstraint->CanMultiple = true;
            ViewingConstraint->Percentage_From = 0;
            ViewingConstraint->Percentage_To = 100;
            LoadConstraint(*ViewingConstraint);
        }

        // Tag
        void SS_Tag_PreferredRooms(IInspectable, IInspectable)
        {
            ViewingConstraint = std::make_shared<CONSTRAINT>();
            ViewingConstraint->TimeOrSpace = 1;
            ViewingConstraint->Description = s(146);
            ViewingConstraint->LeftTags = 1;
            ViewingConstraint->FETXMLEntry = "ConstraintTagPreferredRooms";
            ViewingConstraint->SpecialView = 2;
            ViewingConstraint->RightRooms = 2;
            ViewingConstraint->CanMultiple = true;
            ViewingConstraint->Percentage_From = 0;
            ViewingConstraint->Percentage_To = 100;
            LoadConstraint(*ViewingConstraint);
        }


        // Output Features

        // Class
        void ES_Class_Combinations(IInspectable, IInspectable)
        {
            ViewingConstraint = std::make_shared<CONSTRAINT>();
            ViewingConstraint->TimeOrSpace = 2;
            ViewingConstraint->Description = s(188);
            ViewingConstraint->FETXMLEntry = "Combo";
            ViewingConstraint->SpecialView = 0;
            ViewingConstraint->LeftClasses = 2;
            ViewingConstraint->CanMultiple = false;
            ViewingConstraint->Percentage_From = 100;
            ViewingConstraint->Percentage_To = 100;


            if (1)
            {
                CONSTRAINT_PARAM p1;
                p1.Description = s(42);
                p1.type = 4;
                p1.FETXMLEntry = "Name";
                ViewingConstraint->Params.push_back(p1);
            }

			LoadConstraint(*ViewingConstraint);
        }
            
    };
}

namespace winrt::WuiFET::factory_implementation
{
    struct Constraints : ConstraintsT<Constraints, implementation::Constraints>
    {
    };
}
