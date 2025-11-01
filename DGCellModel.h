#pragma once
#include "DGCellModel.g.h"
ystring trim(ystring s);
#include "constraintclass.hpp"

namespace winrt::WuiFET::implementation
{
    struct DGCellModel : DGCellModelT<DGCellModel>
    {
        DGCellModel(int row,int col,DGCellType type, hstring const& value)
        {
            m_type = type;
			m_value = value;
			m_row = row;
			m_col = col;
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


        IInspectable _pi1, _pi2, _pi3;
		IInspectable pi1() const { return _pi1; }
		void pi1(IInspectable const& v) { _pi1 = v; }
		IInspectable pi2() const { return _pi2; }
		void pi2(IInspectable const& v) { _pi2 = v; }
		IInspectable pi3() const { return _pi3; }
		void pi3(IInspectable const& v) { _pi3 = v; }


        DGCellType Type() const 
        { 
            return m_type; 
        }
		void Type(DGCellType const& t) { m_type = t; }

        hstring Value() const 
        { 
            return m_value; 
        }
        void Value(hstring const& v) 
        { 
            m_value = v; 

            if (_ptr && _ptr2)
            {
                XML3::XMLElement* el = (XML3::XMLElement*)_ptr2;
                CONSTRAINT_PARAM* p = (CONSTRAINT_PARAM*)_ptr;
                if (el && p)
                {
                    auto mw = _pi2.try_as<winrt::WuiFET::MainWindow>();
                    if (mw)
                        mw.Dirty();

                    // If bool, set true/false
                    el->SetContent(ystring(m_value.c_str()).a_str());
                    m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{ L"Value" });
                }
            }

        }

        bool VisText()
        {
			return m_type == DGCellType::Text;
        }
        bool VisButton()
        {
            return m_type == DGCellType::Button;
        }
        bool VisBlock()
        {
            return m_type == DGCellType::Block;
		}
        bool VisCheck()
        {
            return m_type == DGCellType::Check;
		}
        bool VisNumber()
        {
            return m_type == DGCellType::Number;
		}
        bool ValueBB() const
        {
            return m_valueB;
        }
        void ValueBB(bool v)
        {
            m_valueB = v;
        }

        bool ValueB() const 
        { 
            return m_valueB; 
        }
        void ValueB(bool v)
        {
            m_valueB = v;
            if (_whatx == 11)
            {
                ValueN(v);
                return;
            }


            if ((_whatx == 1 || _whatx == 2 || _whatx == 3 || _whatx == 4 || _whatx == 5 || _whatx == 6 || _whatx == 7 || _whatx == 8 || _whatx == 15 || _whatx == 16 || _whatx == 17) && _ptr2)
            {
                // pi1 = DGDataGrid
                // pi2 = MainWindow
                // Ptr2 = root
                // Ptr3 = Item
                // Ptr4 = ConstraintTeacherNotAvailableTimes (if existing)
                // Ptr = Not_Available_Time (if existing)
                auto mw = _pi2.try_as<winrt::WuiFET::MainWindow>();

                auto root = (XML3::XMLElement*)_ptr2;
                const char* tofind = "Time_Constraints_List";
                if (_whatx == 7)
					tofind = "Space_Constraints_List";
                auto Time_Constraints_List = root->FindElementZ(tofind, true);


                std::string search_1 = "Not_Available_Time";
                if (_whatx == 6 || _whatx == 16)
                    search_1 = "Preferred_Time_Slot";
                if (_whatx == 5 || _whatx == 15) 
                    search_1 = "Preferred_Starting_Time";
                if (_whatx == 17)
                    search_1 = "Selected_Time_Slot";

                std::string search_2 = "Number_of_Not_Available_Times";
                if (_whatx == 6 || _whatx == 16) 
                    search_2 = "Number_of_Preferred_Time_Slots";
                if (_whatx == 5 || _whatx == 15) 
                    search_2 = "Number_of_Preferred_Starting_Times";
                if (_whatx == 17)
					search_2 = "Number_of_Selected_Time_Slots";

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

/*                if (!_ptr && ptr3)
                {
                XML3::XMLElement* Teacher = (XML3::XMLElement*)_ptr3;

                    if (!v)
                        return;

                    XML3::XMLElement * ConstraintTeacherNotAvailableTimes = (XML3::XMLElement*)_ptr4;
                    if (!ConstraintTeacherNotAvailableTimes)
                    {
                        if (mw) mw.Dirty();
						const char* whatadd = "ConstraintTeacherNotAvailableTimes";
                        if (_whatx == 2)
							whatadd = "ConstraintStudentsSetNotAvailableTimes";
                        if (_whatx == 5)
                            whatadd = "ConstraintActivityPreferredStartingTimes";
                        if (_whatx == 15)
                            whatadd = "ConstraintActivitiesPreferredStartingTimes";
                        if (_whatx == 6)
							whatadd = "ConstraintActivityPreferredTimeSlots";
                        if (_whatx == 16)
                            whatadd = "ConstraintActivitiesPreferredTimeSlots";
                        if (_whatx == 17)
							whatadd = "ConstraintActivitiesOccupyMaxTimeSlotsFromSelection";
                        if (_whatx == 7)
							whatadd = "ConstraintRoomNotAvailableTimes";
						if (_whatx == 8)
							whatadd = "ConstraintTagNotAvailableTimes";
                        auto& t0 = Time_Constraints_List->AddElement(whatadd);
						Ptr4((long long)&t0);

                        // Pass this to all cells
                        if (_pi1)
                        {
							auto datagrid = _pi1.try_as<winrt::WuiFET::DGDataGrid>();
                            if (datagrid)
                            {
                                for (auto row : datagrid.ItemsSource())
                                {
                                    for (auto cell : row.Cells())
                                    {
                                        if (cell.Ptr4() == 0)
                                            cell.Ptr4((long long)&t0);
                                    }
                                }
                            }

                        }

						ConstraintTeacherNotAvailableTimes = &t0;
                        if (_whatx == 1)
                            t0.AddElement("Teacher").SetContent(trim(Teacher->FindElementZ("Name", true)->GetContent()).a_str());
                        if (_whatx == 2)
                            t0.AddElement("Students").SetContent(trim(Teacher->FindElementZ("Name", true)->GetContent()).a_str());
                        if (_whatx == 5 || _whatx == 6 || _whatx == 15 || _whatx == 16 || _whatx == 17)
                            t0.AddElement("Activity_Id").SetContent(trim(Teacher->FindElementZ("Id", true)->GetContent()).a_str());
                        if (_whatx == 7)
							t0.AddElement("Room").SetContent(trim(Teacher->FindElementZ("Name", true)->GetContent()).a_str());
						if (_whatx == 8)
                            t0.AddElement("Tag").SetContent(trim(Teacher->FindElementZ("Name", true)->GetContent()).a_str());

                        t0.AddElement("Weight_Percentage").SetContent(std::to_string(_NewPutGlobalPercNext).c_str());
                    }
                }
 */

               XML3::XMLElement* ConstraintTeacherNotAvailableTimes = (XML3::XMLElement*)_ptr4;
                XML3::XMLElement* Not_Available_Time = 0;
                for (auto& e : *ConstraintTeacherNotAvailableTimes)
                {
                    if (e.GetElementName() != search_1)
                        continue;

                    int YF = 0;
                    for (auto& ee : e)
                    {
                        if (ee.GetElementName() == "Day")
                        {
                            if (trim(ee.GetContent()) == days2[m_col - 1])
                                YF++;
                        }
                        if (ee.GetElementName() == "Hour")
                        {
                            if (trim(ee.GetContent()) == hours2[m_row])
                                YF++;
                        }
                    }
                    if (YF == 2)
                        Not_Available_Time = &e;
                    if (Not_Available_Time)
                        break;
                }

                if (v == 0)
                {
                    if (Not_Available_Time)
                    {
                        if (mw) mw.Dirty();
                        auto par = Not_Available_Time->GetParent(root);
                        if (par)
                        {
                            par->RemoveElement(Not_Available_Time);

                            // Decrease the "Number_of_Not_Available_Times"
                            auto n = par->FindElementZ(search_2.c_str(),true);
							int nc = atoi(trim(n->GetContent().c_str()));
                            nc--;
                            if (nc < 0)
                                nc = 0;
                            if (nc > 0)
    							n->SetContent(std::to_string(nc).c_str());
                            else
                            {
                                // Remove all element, no more constraints
								auto par2 = par->GetParent(root);
                                if (par2)
                                    par2->RemoveElement(par);
								Ptr(0);

                                auto datagrid = _pi1.try_as<winrt::WuiFET::DGDataGrid>();
                                if (datagrid)
                                {
                                    for (auto row : datagrid.ItemsSource())
                                    {
                                        for (auto cell : row.Cells())
                                        {
                                            cell.Ptr((long long)0);
                                            cell.Ptr4((long long)0);
                                        }
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        // Nothing, its not there anyway
                    }
                }
                else
                {
                    if (!Not_Available_Time)
                    {
                        // Create it
                        if (mw) mw.Dirty();
                        auto& t1 = ConstraintTeacherNotAvailableTimes->AddElement(search_1.c_str());
						Not_Available_Time = &t1;
                        Not_Available_Time->AddElement("Day").SetContent(trim(days2[m_col - 1].c_str()));
                        Not_Available_Time->AddElement("Hour").SetContent(trim(hours2[m_row].c_str()));
                        // Increase the "Number_of_Not_Available_Times"
                        auto n = ConstraintTeacherNotAvailableTimes->FindElementZ(search_2.c_str(), true);
                        int nc = atoi(trim(n->GetContent().c_str()));
                        nc++;
                        n->SetContent(std::to_string(nc).c_str());
                    }

                }
            }
        }

        long long Ptr() const 
        { 
            return _ptr;
        }
        void Ptr(long long value) 
        { 
            _ptr = value; 
		}
        long long Ptr2() const
        {
            return _ptr2;
        }
        void Ptr2(long long value)
        {
            _ptr2 = value;
		}
        long long Ptr3() const
        {
            return _ptr3;
		}
        void Ptr3(long long value)
        {
            _ptr3 = value;
        }
        long long _ptr4 = 0;
        long long _ptr5 = 0;
        void Ptr4(long long value)
        {
            _ptr4 = value;
		}
        long long Ptr4() const
        {
            return _ptr4;
        }
        void Ptr5(long long value)
        {
            _ptr5 = value;
        }
        long long Ptr5() const
        {
            return _ptr5;
		}

        int MinimumHeight()
        {
            return _minheight;
		}
        void MinimumHeight(int value)
        {
            if (_minheight != value)
            {
                _minheight = value;
                m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{ L"MinimumHeight" });
            }
		}

        int MinimumWidth()
        {
            return _minwidth;
        }
        void MinimumWidth(int value)
        {
            if (_minwidth != value)
            {
                _minwidth = value;
                m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{ L"MinimumWidth" });
            }
        }

        int ValueN() const 
        { 
            return m_valueN; 
		}
        void ValueN(int value)
        {
            if (m_valueN != value)
            {
                m_valueN = value;
                m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{ L"ValueN" });

                if (_whatx == 11)
                {
					// pi2 = MainWindow
                    // ptr3 = &A_CONSTRAINT
					// ptr = &CONSTRAINT_PARAM
                    // ptr2 = XMLElement* of change
                    if (_ptr && _ptr2)
                    {
						XML3::XMLElement* el = (XML3::XMLElement*)_ptr2;
                        CONSTRAINT_PARAM* p = (CONSTRAINT_PARAM*)_ptr;
                        if (el && p)
                        {
                            if (m_valueN < p->from)
                                m_valueN = p->from;
                            if (m_valueN > p->to)
                                m_valueN = p->to;
                            auto mw = _pi2.try_as<winrt::WuiFET::MainWindow>();
                            if (mw)
                                mw.Dirty();

                            // If bool, set true/false
                            if (p->type == 1)
                                el->SetContent(m_valueN ? "true" : "false");
                            else
                                el->SetContent(std::to_string(m_valueN).c_str());
                            m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{ L"ValueN" });
						}
                    }
                }
            }
        }
        int ValueNMin() const
        {
            return m_valueNMin;
        }
        void ValueNMin(int value)
        {
            if (m_valueNMin != value)
            {
                m_valueNMin = value;
                m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{ L"ValueNMin" });
            }
		}
        int ValueNMax() const
        {
            return m_valueNMax;
		}
        void ValueNMax(int value)
        {
            if (m_valueNMax != value)
            {
                m_valueNMax = value;
                m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{ L"ValueNMax" });
            }
        }

        int WhatX() const 
        { 
            return _whatx; 
		}
        void WhatX(int value) 
        { 
            if (_whatx != value)
            {
                _whatx = value; 
                m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{ L"WhatX" });
            }
		}

        int _whatx = 0; // 1 -> TimeNotAvailable Teacher 2 StudentSet, 3 Room, 7+ Space, 11 Generic
        int _minwidth = 150;
        int _minheight = 50;
        long long _ptr = 0;
        long long _ptr2 = 0;
        long long _ptr3 = 0;
		int m_row = 0;
        int m_col = 0;
        DGCellType m_type = DGCellType::Text;
        hstring m_value = L"Hello";
		int m_valueN = 0;
        int m_valueNMin = 0;
		int m_valueNMax = 100;
		bool m_valueB = false;
    };
};

namespace winrt::WuiFET::factory_implementation
{
    struct DGCellModel : DGCellModelT<DGCellModel, implementation::DGCellModel>
    {
    };
}
