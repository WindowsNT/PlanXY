#pragma once

#include "Statistics.g.h"
#include "timetable.hpp"

namespace winrt::WuiFET::implementation
{
    struct Statistics : StatisticsT<Statistics>
    {
        Statistics()
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


        int _mode = 0;
        void Mode(int value)
        {
            if (_mode != value)
            {
                _mode = value;
                m_propertyChanged(*this, winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"Mode"));
            }
		}
        int Mode()
        {
            return _mode;
		}

        bool IsM1()
        {
            if (!_ResultX)
                return true;
            return false;
        }
        bool IsM2()
        {
            if (_ResultX)
                return true;
            return false;
        }

        winrt::Windows::Foundation::Collections::IObservableVector<winrt::WuiFET::DGHeaderModel> gr1_ch()
        {
            auto headers = winrt::single_threaded_observable_vector<winrt::WuiFET::DGHeaderModel>();
            for (auto& h : hdrs)
            {
                winrt::WuiFET::DGHeaderModel it;
                it.Name(h.c_str());
				headers.Append(it);
            }
            return headers;
        }

        winrt::Windows::Foundation::Collections::IObservableVector<DGRowModel> gr1_is()
        {
            auto rows = winrt::single_threaded_observable_vector<DGRowModel>();
            int iRow = 0;
            for (auto& ro : ros)
            {
                auto cells = winrt::single_threaded_observable_vector<DGCellModel>();

                int iCol = 0;
                for(auto& cell : ro.second)
                {
                    DGCellModel c1(iRow, iCol, DGCellType::Block, cell.c_str());
                    cells.Append(c1);
					iCol++;
				}

                DGRowModel rr((int)iRow, cells);
                rows.Append(rr);
                iRow++;
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


        std::vector<ystring> hdrs;
        std::map<std::wstring, std::vector<ystring>> ros;

        FINISHEDTT ftt;

        void TeachersHours(auto, auto)
        {
            ros.clear();
            hdrs = {s(42),s(44),s(202)};
            auto& TeachersList = _ResultP->x->GetRootElement()["Teachers_List"];
            for (auto& t : TeachersList)
            {
                auto u = trim(t.FindElementZ("Name", true)->GetContent());
                auto name = DisplayName(t);

                int must = _wtoi(trim(t.FindElementZ("Target_Number_of_Hours", true)->GetContent()));;
                int hours = 0;


                // Find their name in Activities
                auto& acts = _ResultP->x->GetRootElement()["Activities_List"];
                for (auto& a : acts)
                {
                    for (auto& a2 : a)
                    {
                        if (a2.GetElementName() != "Teacher")
                            continue;
                        auto aname = trim(a2.GetContent());
                        if (aname != u)
                            continue;

                        auto dur = trim(a.FindElementZ("Duration", true)->GetContent());
                        hours += _wtoi(dur.c_str());
                        break;
                    }
                }

                ros[name].push_back(name);
                ros[name].push_back(std::to_wstring(hours));
                if (must)
                    ros[name].push_back(std::to_wstring(must));
                else
                    ros[name].push_back(L"");
            }
            Refresh();
        }
        void LessonsHours(auto, auto)
        {
            ros.clear();
            hdrs = { s(33),s(44) };
            auto& TeachersList = _ResultP->x->GetRootElement()["Subjects_List"];
            for (auto& t : TeachersList)
            {
                auto u = trim(t.FindElementZ("Name", true)->GetContent());
                auto name = DisplayName(t);
                int hours = 0;

                // Find their name in Activities
                auto& acts = _ResultP->x->GetRootElement()["Activities_List"];
                for (auto& a : acts)
                {
                    for (auto& a2 : a)
                    {
                        if (a2.GetElementName() != "Subject")
                            continue;
                        auto aname = trim(a2.GetContent());
                        if (aname != u)
                            continue;

                        auto dur = trim(a.FindElementZ("Duration", true)->GetContent());
                        hours += _wtoi(dur.c_str());
                        break;
                    }
                }

                ros[name].push_back(name);
                ros[name].push_back(std::to_wstring(hours));
            }
            Refresh();
        }

        void ClassHours(auto, auto)
        {
            hdrs = { s(92),s(44) };
            ros.clear();

            std::vector<XML3::XMLElement*> AllClasses(XML3::XMLElement & r);
            auto TeachersList = AllClasses(_ResultP->x->GetRootElement()["Students_List"]);

            for (auto& t : TeachersList)
            {
                auto u = trim(t->FindElementZ("Name", true)->GetContent());
                auto name = DisplayName(*t);
                int hours = 0;

                // Find their name in Activities
                auto& acts = _ResultP->x->GetRootElement()["Activities_List"];
                for (auto& a : acts)
                {
                    for (auto& a2 : a)
                    {
                        if (a2.GetElementName() != "Students")
                            continue;
                        auto aname = trim(a2.GetContent());
                        if (aname != u)
                            continue;

                        auto dur = trim(a.FindElementZ("Duration", true)->GetContent());
                        hours += _wtoi(dur.c_str());
                        break;
                    }
                }

                ros[name].push_back(name);
                ros[name].push_back(std::to_wstring(hours));
            }
            Refresh();
        }
        void MaxSpan(auto, auto)
        {
            hdrs = { s(31),s(102) };
            ros.clear();
            if (ftt.Empty())
                ftt.BuildTimeTableResult(_ResultP, _ResultX);

            auto& TeachersList = (*_ResultX)["Teachers_Timetable"];
            auto daysx = _ResultP->DaysNames();
            auto hoursx = _ResultP->HoursNames();
            for (auto& t : TeachersList)
            {
                ystring u = t.vv("name").GetValue();
                ystring name;
                auto te = _ResultP->FindSubElementByName(&_ResultP->x->GetRootElement()["Teachers_List"], u.c_str());
                if (te)
                    name = DisplayName(*te);
                int Span = 0;

                std::map<int, int> d1;
                for (auto& t2 : t)
                {
                    if (t2.GetElementName() != "Day")
                        continue;

                    // find day index
                    int day_index = -1;
                    for (size_t di = 0; di < daysx.size(); di++)
                    {
                        if (daysx[di] == ystring(t2.vv("name").GetValue().c_str()))
                        {
                            day_index = (int)di;
                            break;
                        }
                    }
                    if (day_index == -1)
                        continue;

                    int min_hour = -1;
                    int max_hour = -1;
                    for (auto& t3 : t2)
                    {
                        // Hours
                        if (t3.GetElementName() != "Hour")
                            continue;


                        if (t3.GetChildrenNum() == 0)
                            continue;

                        // hour index
                        int hour_index = -1;
                        for (size_t hi = 0; hi < hoursx.size(); hi++)
                        {
                            if (hoursx[hi] == ystring(t3.vv("name").GetValue().c_str()))
                            {
                                hour_index = (int)hi;
                                break;
                            }
                        }
                        if (hour_index == -1)
                            continue;

                        if (min_hour == -1 || hour_index < min_hour)
                            min_hour = hour_index;
                        if (max_hour == -1 || hour_index > max_hour)
                            max_hour = hour_index;


                    }
                    d1[day_index] = (max_hour - min_hour) + 1;
                }

                for (auto& dd : d1)
                {
                    Span = std::max(Span, dd.second);
                }




                ros[name].push_back(name);
                ros[name].push_back(std::to_wstring(Span));

            }
            Refresh();

        }
        void TotalGaps(auto, auto)
        {
            hdrs = { s(31),s(199) };
            ros.clear();
            auto& TeachersList = (*_ResultX)["Teachers_Timetable"];
            auto daysx = _ResultP->DaysNames();
            auto hoursx = _ResultP->HoursNames();
            for (auto& t : TeachersList)
            {
                ystring u = t.vv("name").GetValue();
                ystring name;
                auto te = _ResultP->FindSubElementByName(&_ResultP->x->GetRootElement()["Teachers_List"], u.c_str());
                if (te)
                    name = DisplayName(*te);
                int Gaps = 0;


                for (auto& t2 : t)
                {
                    if (t2.GetElementName() != "Day")
                        continue;

                    // find day index
                    int day_index = -1;
                    for (size_t di = 0; di < daysx.size(); di++)
                    {
                        if (daysx[di] == ystring(t2.vv("name").GetValue().c_str()))
                        {
                            day_index = (int)di;
                            break;
                        }
                    }
                    if (day_index == -1)
                        continue;

                    int Started = 0;
                    int SureGaps = 0;
                    int PendingGaps = 0;
                    for (auto& t3 : t2)
                    {
                        // Hours
                        if (t3.GetElementName() != "Hour")
                            continue;
                        if (t3.GetChildrenNum() == 0)
                        {
                            if (Started)
                                PendingGaps++;
                            continue;
                        }
                        SureGaps += PendingGaps;
                        PendingGaps = 0;
                        Started = 1;
                    }
                    Gaps += SureGaps;
                }


                ros[name].push_back(name);
                ros[name].push_back(std::to_wstring(Gaps));
            }
            Refresh();
        }


    };
}

namespace winrt::WuiFET::factory_implementation
{
    struct Statistics : StatisticsT<Statistics, implementation::Statistics>
    {
    };
}
