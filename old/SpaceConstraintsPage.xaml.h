#pragma once

#include "SpaceConstraintsPage.g.h"
#include "constraintclass.hpp"
winrt::Windows::Foundation::IInspectable WindowFromPage(winrt::Windows::Foundation::IInspectable pg);

namespace winrt::WuiFET::implementation
{
    using namespace winrt;
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Controls;
    using namespace Microsoft::UI::Xaml::Input;
    struct SpaceConstraintsPage : SpaceConstraintsPageT<SpaceConstraintsPage>
    {
        SpaceConstraintsPage()
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent

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


        PROJECT* project = 0;
        void OnNavigatedTo(winrt::Microsoft::UI::Xaml::Navigation::NavigationEventArgs const& e)
        {
            if (e.Parameter())
            {
                auto param = e.Parameter().as<long long>();
                project = (PROJECT*)param;
            }
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


        int _WhatShow1 = 0;
        int _WhatShow2 = 0;

        // Menu
        void TS_Teacher_HomeRoom(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            _WhatShow1 = 1; // Teachers
            _WhatShow2 = 1; // HomeRoom
            _SelectedTeacher = 0;
            _SelectedTeacherEntry = 0;
            Refresh();
        }
        void TS_TimeTable_BasicCompulsoryTime(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            _WhatShow1 = 2; // TT
			_WhatShow2 = 1; // Basic Compulsory Time
            _SelectedTeacher = 0;
            _SelectedTeacherEntry = 0;
            Refresh();
        }


        winrt::Microsoft::UI::Xaml::Visibility WhatX(int j)
        {
            if (j == _WhatShow1)
                return winrt::Microsoft::UI::Xaml::Visibility::Visible;
            if (j == 1 && _WhatShow1 == 4) // all teachers
                return winrt::Microsoft::UI::Xaml::Visibility::Visible;
            return winrt::Microsoft::UI::Xaml::Visibility::Collapsed;
        }
        winrt::Microsoft::UI::Xaml::Visibility WhatFX(int j)
        {
            if (j == _WhatShow1)
                return winrt::Microsoft::UI::Xaml::Visibility::Visible;
            return winrt::Microsoft::UI::Xaml::Visibility::Collapsed;
        }
        winrt::Microsoft::UI::Xaml::Visibility WhatY(int j)
        {
            if (j == _WhatShow2)
            {
                if (_WhatShow1 == 1 && _SelectedTeacher)
                    return winrt::Microsoft::UI::Xaml::Visibility::Visible;
                if (_WhatShow1 == 4) // All teachers
                    return winrt::Microsoft::UI::Xaml::Visibility::Visible;
            }
            return winrt::Microsoft::UI::Xaml::Visibility::Collapsed;
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
                auto grid = top.as<Microsoft::UI::Xaml::FrameworkElement>().FindName(L"TC_SingleTeacher").as<Microsoft::UI::Xaml::Controls::Grid>();
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

        // WhatX 1: Teacher
        winrt::Windows::Foundation::Collections::IObservableVector<winrt::WuiFET::Item> Teachers_List()
        {
            auto items = winrt::single_threaded_observable_vector<winrt::WuiFET::Item>();
            auto x = project->x;
            if (!x)
                return items;

            auto& r = x->GetRootElement()["Teachers_List"];
            for (auto& rr : r)
            {
                std::shared_ptr<XML3::XMLElement> ee = rr.FindElementZ("Name", true);
                ystring name = trim(ee->GetContent());

                winrt::WuiFET::Item it;
                it.Name1(name.c_str());
                it.ptr((long long)&rr);
                items.Append(it);
            }
            return items;
        }
        XML3::XMLElement* _SelectedTeacher = 0;
        XML3::XMLElement* _SelectedTeacherEntry = 0;
        void TeacherChanged(IInspectable, IInspectable)
        {
            auto lv = Teachers_ListView();
            auto sel = lv.SelectedItem().as<winrt::WuiFET::Item>();
            if (sel && sel.ptr())
                _SelectedTeacher = (XML3::XMLElement*)sel.ptr();
            _SelectedTeacherEntry = nullptr;

            if (_SelectedTeacher)
            {

                // Select the first if it exists
                auto x = project->x;
                ystring SelectedTeacherName = trim(_SelectedTeacher->FindElementZ("Name", true)->GetContent());
                XML3::XMLElement* root = &x->GetRootElement();

                if (_WhatShow2 == 2)
                {
                    zz21_LeftChanged(lv);
                    return;
                }

                // HomeRoom 
                if (_WhatShow2 == 1)
                {
                    auto r0 = root->FindElementZ("Space_Constraints_List", true);

                    // New Entry default
                    ystring y;
                    y.Format(L"%s", s(83));
                    _gr1_n1 = y;

                    for (auto& r1 : *r0)
                    {
                        if (r1.GetElementName() != "ConstraintTeacherHomeRoom")
                            continue;
                        // This teacher?
                        auto tn = trim(r1.FindElementZ("Teacher", true)->GetContent());
                        if (tn != SelectedTeacherName)
                            continue;
                        _SelectedTeacherEntry = &r1; // default editable the first one

                        int p = 100;
                        p = _wtoi(trim(_SelectedTeacherEntry->FindElementZ("Weight_Percentage", true)->GetContent()).c_str());
                        _gr1_p = p;
                        Refresh(L"gr1_p");
                        Refresh(L"gr1_rl");

                        // There is one found, change y
                        y.Format(L"%s #%i", s(82), 1);
                        _gr1_n1 = y;
                        break;
                    }
                }
            }
            Refresh(L"WhatY");
            Refresh(L"gr1_n1");
            Refresh(L"gr1_p");
            Refresh(L"gr1_rl");
        }

        // WhatY 1: HomeRoom
        #include "SpaceConstraintsPage_TeacherHomeRoom.hpp"

        std::shared_ptr<A_CONSTRAINT> ViewingConstraint;


        // WhatX 2: Class
        winrt::Windows::Foundation::Collections::IObservableVector<winrt::WuiFET::Item> Classes_List()
        {
            auto items = winrt::single_threaded_observable_vector<winrt::WuiFET::Item>();
            auto x = project->x;
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
        XML3::XMLElement* _SelectedClassEntry = 0;
        void ClassChanged(IInspectable, IInspectable)
        {
            auto lv = Classes_ListView();
            auto sel = lv.SelectedItem().as<winrt::WuiFET::Item>();
            _SelectedClass = 0;
            if (sel && sel.ptr())
                _SelectedClass = (XML3::XMLElement*)sel.ptr();
            _SelectedClassEntry = nullptr;

            // Select the first if it exists
            if (_SelectedClass)
            {
                auto x = project->x;
                ystring SelectedClassName = trim(_SelectedClass->FindElementZ("Name", true)->GetContent());
//                XML3::XMLElement* root = &x->GetRootElement();

                if (_WhatShow2 == 2)
                {
                    zz21_LeftChanged(lv);
                    return;
                }

            }
            Refresh(L"WhatY");
            Refresh(L"gr2_n1");
            Refresh(L"gr2_p");
            Refresh(L"gr2_is");
            Refresh(L"gr2_ch");
        }


        // WhatX 4: Subject
        winrt::Windows::Foundation::Collections::IObservableVector<winrt::WuiFET::Item> Subjects_List()
        {
            auto items = winrt::single_threaded_observable_vector<winrt::WuiFET::Item>();
            auto x = project->x;
            if (!x)
                return items;
            auto& r = x->GetRootElement()["Subjects_List"];
            for (auto& rr : r)
            {
                std::shared_ptr<XML3::XMLElement> ee = rr.FindElementZ("Name", true);
                ystring name = trim(ee->GetContent());
                winrt::WuiFET::Item it;
                it.Name1(name.c_str());
                it.ptr((long long)&rr);
                items.Append(it);
            }
            return items;
        }

        XML3::XMLElement* _SelectedSubject = 0;
        XML3::XMLElement* _SelectedSubjectEntry = 0;
        void SubjectChanged(IInspectable, IInspectable)
        {
            auto lv = Subjects_ListView();
            auto sel = lv.SelectedItem().as<winrt::WuiFET::Item>();
            _SelectedSubject = 0;
            if (sel && sel.ptr())
                _SelectedSubject = (XML3::XMLElement*)sel.ptr();
            _SelectedSubjectEntry = nullptr;

            // Select the first if it exists
            if (_SelectedSubject)
            {
                auto x = project->x;
                ystring SelectedSubjectName = trim(_SelectedSubject->FindElementZ("Name", true)->GetContent());
                //                XML3::XMLElement* root = &x->GetRootElement();

                if (_WhatShow2 == 2)
                {
                    zz21_LeftChanged(lv);
                    return;
                }

            }
            Refresh(L"WhatY");
            Refresh(L"gr2_n1");
            Refresh(L"gr2_p");
            Refresh(L"gr2_is");
            Refresh(L"gr2_ch");
        }


        // WhatY 2 : Generic
#include "constraintclassfunctions.hpp"




        void TS_Teacher_MaxRoomChangesDay(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            _SelectedTeacher = 0;
            _SelectedTeacherEntry = 0;
            _WhatShow1 = 1;
            _WhatShow2 = 2;
            ViewingConstraint = std::make_shared<A_CONSTRAINT>();
            ViewingConstraint->d0 = s(139);
            ViewingConstraint->x0 = "ConstraintTeacherMaxRoomChangesPerDay";
            ViewingConstraint->Type = 1;
            ViewingConstraint->SubType = 1;
            ViewingConstraint->HasTeacher = 1;
            ViewingConstraint->w_from = 100;
            ViewingConstraint->w_to = 100;
            ViewingConstraint->SupportsMultiple = false;
            CONSTRAINT_PARAM p1;
            p1.d1 = s(139);
            p1.type = 0;

            p1.x1 = "Max_Room_Changes_Per_Day";
            p1.from = 0;
            p1.to = HowManyHours();
            p1.def = p1.to;
            ViewingConstraint->params.push_back(p1);
            Refresh();
        }

        void TS_Teachers_MaxRoomChangesDay(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            _SelectedTeacher = 0;
            _SelectedTeacherEntry = 0;
            _WhatShow1 = 4;
            _WhatShow2 = 2;
            ViewingConstraint = std::make_shared<A_CONSTRAINT>();
            ViewingConstraint->d0 = s(139);
            ViewingConstraint->x0 = "ConstraintTeachersMaxRoomChangesPerDay";
            ViewingConstraint->Type = 1;
            ViewingConstraint->SubType = 0;
            ViewingConstraint->w_from = 100;
            ViewingConstraint->w_to = 100;
            ViewingConstraint->SupportsMultiple = false;
            CONSTRAINT_PARAM p1;
            p1.d1 = s(139);
            p1.type = 0;

            p1.x1 = "Max_Room_Changes_Per_Day";
            p1.from = 0;
            p1.to = HowManyHours();
            p1.def = p1.to;
            ViewingConstraint->params.push_back(p1);
            zz21_NoLeftInit();
            Refresh();
        }


        void TS_Teacher_MaxBuildingChangesDay(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            _SelectedTeacher = 0;
            _SelectedTeacherEntry = 0;
            _WhatShow1 = 1;
            _WhatShow2 = 2;
            ViewingConstraint = std::make_shared<A_CONSTRAINT>();
            ViewingConstraint->d0 = s(143);
            ViewingConstraint->x0 = "ConstraintTeacherMaxBuildingChangesPerDay";
            ViewingConstraint->Type = 1;
            ViewingConstraint->SubType = 1;
            ViewingConstraint->HasTeacher = 1;
            ViewingConstraint->w_from = 100;
            ViewingConstraint->w_to = 100;
            ViewingConstraint->SupportsMultiple = false;
            CONSTRAINT_PARAM p1;
            p1.d1 = s(143);
            p1.type = 0;

            p1.x1 = "Max_Building_Changes_Per_Day";
            p1.from = 0;
            p1.to = HowManyHours();
            p1.def = p1.to;
            ViewingConstraint->params.push_back(p1);
            Refresh();
        }


        void TS_Teacher_MaxRoomChangesWeek(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            _SelectedTeacher = 0;
            _SelectedTeacherEntry = 0;
            _WhatShow1 = 1;
            _WhatShow2 = 2;
            ViewingConstraint = std::make_shared<A_CONSTRAINT>();
            ViewingConstraint->d0 = s(139);
            ViewingConstraint->x0 = "ConstraintTeacherMaxRoomChangesPerWeek";
            ViewingConstraint->Type = 1;
            ViewingConstraint->SubType = 1;
            ViewingConstraint->HasTeacher = 1;
            ViewingConstraint->w_from = 100;
            ViewingConstraint->w_to = 100;
            ViewingConstraint->SupportsMultiple = false;
            CONSTRAINT_PARAM p1;
            p1.d1 = s(139);
            p1.type = 0;

            p1.x1 = "Max_Room_Changes_Per_Week";
            p1.from = 0;
            p1.to = HowManyHours();
            p1.def = p1.to;
            ViewingConstraint->params.push_back(p1);
            Refresh();
        }

        void TS_Teachers_MaxRoomChangesWeek(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            _SelectedTeacher = 0;
            _SelectedTeacherEntry = 0;
            _WhatShow1 = 4;
            _WhatShow2 = 2;
            ViewingConstraint = std::make_shared<A_CONSTRAINT>();
            ViewingConstraint->d0 = s(139);
            ViewingConstraint->x0 = "ConstraintTeachersMaxRoomChangesPerWeek";
            ViewingConstraint->Type = 1;
            ViewingConstraint->SubType = 0;
            ViewingConstraint->w_from = 100;
            ViewingConstraint->w_to = 100;
            ViewingConstraint->SupportsMultiple = false;
            CONSTRAINT_PARAM p1;
            p1.d1 = s(139);
            p1.type = 0;

            p1.x1 = "Max_Room_Changes_Per_Week";
            p1.from = 0;
            p1.to = HowManyHours();
            p1.def = p1.to;
            ViewingConstraint->params.push_back(p1);
            zz21_NoLeftInit();
            Refresh();
        }

        void TS_Teacher_MaxBuildingChangesWeek(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            _SelectedTeacher = 0;
            _SelectedTeacherEntry = 0;
            _WhatShow1 = 1;
            _WhatShow2 = 2;
            ViewingConstraint = std::make_shared<A_CONSTRAINT>();
            ViewingConstraint->d0 = s(143);
            ViewingConstraint->x0 = "ConstraintTeacherMaxBuildingChangesPerWeek";
            ViewingConstraint->Type = 1;
            ViewingConstraint->SubType = 1;
            ViewingConstraint->HasTeacher = 1;
            ViewingConstraint->w_from = 100;
            ViewingConstraint->w_to = 100;
            ViewingConstraint->SupportsMultiple = false;
            CONSTRAINT_PARAM p1;
            p1.d1 = s(143);
            p1.type = 0;

            p1.x1 = "Max_Building_Changes_Per_Week";
            p1.from = 0;
            p1.to = HowManyHours();
            p1.def = p1.to;
            ViewingConstraint->params.push_back(p1);
            Refresh();
        }

        void TS_Teacher_MaxRoomChangesDayInterval(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            _SelectedTeacher = 0;
            _SelectedTeacherEntry = 0;
            _WhatShow1 = 1;
            _WhatShow2 = 2;
            ViewingConstraint = std::make_shared<A_CONSTRAINT>();
            ViewingConstraint->d0 = s(141);
            ViewingConstraint->x0 = "ConstraintTeacherMaxRoomChangesPerDayInInterval";
            ViewingConstraint->Type = 1;
            ViewingConstraint->SubType = 1;
            ViewingConstraint->HasTeacher = 1;
            ViewingConstraint->w_from = 100;
            ViewingConstraint->w_to = 100;
            ViewingConstraint->SupportsMultiple = false;

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


            if (1)
            {
                CONSTRAINT_PARAM p1;
                p1.d1 = s(139);
                p1.type = 0;
                p1.x1 = "Max_Room_Changes_Per_Day";
                p1.from = 0;
                p1.to = HowManyHours();
                p1.def = p1.to;
                ViewingConstraint->params.push_back(p1);
            }
            Refresh();
        }

        void TS_Teacher_MaxBuildingChangesDayInterval(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            _SelectedTeacher = 0;
            _SelectedTeacherEntry = 0;
            _WhatShow1 = 1;
            _WhatShow2 = 2;
            ViewingConstraint = std::make_shared<A_CONSTRAINT>();
            ViewingConstraint->d0 = s(144);
            ViewingConstraint->x0 = "ConstraintTeacherMaxBuildingChangesPerDayInInterval";
            ViewingConstraint->Type = 1;
            ViewingConstraint->SubType = 1;
            ViewingConstraint->HasTeacher = 1;
            ViewingConstraint->w_from = 100;
            ViewingConstraint->w_to = 100;
            ViewingConstraint->SupportsMultiple = false;

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


            if (1)
            {
                CONSTRAINT_PARAM p1;
                p1.d1 = s(144);
                p1.type = 0;
                p1.x1 = "Max_Building_Changes_Per_Day";
                p1.from = 0;
                p1.to = HowManyHours();
                p1.def = p1.to;
                ViewingConstraint->params.push_back(p1);
            }
            Refresh();
        }


        void TS_Subject_PreferredRooms(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs)
        {
            Refresh();
		}

        XML3::XMLElement* _SelectedRoom = 0;
        XML3::XMLElement* _SelectedRoomEntry = 0;
        void RoomChanged(IInspectable, IInspectable)
        {
            auto lv = Rooms_ListView();
            auto sel = lv.SelectedItem().as<winrt::WuiFET::Item>();
            if (sel && sel.ptr())
                _SelectedRoom = (XML3::XMLElement*)sel.ptr();
            Refresh(L"gr2_is");
            Refresh(L"gr2_ch");
		}

        winrt::Windows::Foundation::Collections::IObservableVector<winrt::WuiFET::Item> Rooms_List()
        {
            auto items = winrt::single_threaded_observable_vector<winrt::WuiFET::Item>();
            return items;
        }

    };
}

namespace winrt::WuiFET::factory_implementation
{
    struct SpaceConstraintsPage : SpaceConstraintsPageT<SpaceConstraintsPage, implementation::SpaceConstraintsPage>
    {
    };
}
