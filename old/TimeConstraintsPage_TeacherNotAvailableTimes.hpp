
// WhatY 1: NotAvailableTimes
void dd1Click(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs)
{
    if (!_SelectedTeacher)
        return;
    auto button = sender.as<DropDownButton>();
    MenuFlyout flyout;
    flyout.Placement(Microsoft::UI::Xaml::Controls::Primitives::FlyoutPlacementMode::Bottom);

    ystring SelectedTeacherName = trim(_SelectedTeacher->FindElementZ("Name", true)->GetContent());
    auto x = project->x;
    XML3::XMLElement* root = &x->GetRootElement();
    auto r0 = root->FindElementZ("Time_Constraints_List", true);

    if (1)
    {
        ystring y;
        y.Format(L"%s", s(83));
        MenuFlyoutItem item;
        item.Text(y.c_str());
        item.Tag(winrt::box_value((long long)0));
        item.Click([&](IInspectable const&, RoutedEventArgs const&)
            {
                int p = 100;
                _SelectedTeacherEntry = 0;
                _gr1_p = p;
                Refresh(L"gr1_p");
                Refresh(L"gr1_is");
                Refresh(L"gr1_ch");
                _gr1_n1 = s(83);
                Refresh(L"gr1_n1");
            });
        flyout.Items().Append(item);
    }

    int yy = 0;
    for (auto& r1 : *r0)
    {
        if (r1.GetElementName() != "ConstraintTeacherNotAvailableTimes")
            continue;
        // This teacher?
        auto tn = trim(r1.FindElementZ("Teacher", true)->GetContent());
        if (tn != SelectedTeacherName)
            continue;
        ystring y;
        yy++;
        auto whatper = _wtoi(trim(r1.FindElementZ("Weight_Percentage", true)->GetContent()).c_str());
        y.Format(L"%s #%i (%i%%)", s(82), yy,whatper);


        MenuFlyoutItem item;
        item.Text(y.c_str());
        item.Tag(winrt::box_value((long long)&r1));
        item.Click([&](IInspectable const& sender, RoutedEventArgs const&)
            {
                auto it = sender.as<MenuFlyoutItem>();
                _SelectedTeacherEntry = (XML3::XMLElement*)unbox_value<long long>(it.Tag());
                _gr1_p = _wtoi(trim(_SelectedTeacherEntry->FindElementZ("Weight_Percentage", true)->GetContent()).c_str());
                Refresh(L"gr1_p");
                Refresh(L"gr1_is");
                Refresh(L"gr1_ch");
                ystring y = it.Text().c_str();
                wchar_t buf[256] = {};
				wcscpy_s(buf, 256, y.c_str());
				auto ch = wcschr(buf, L'(');
				if (ch)
					*ch = 0;
				_gr1_n1 = buf;
                Refresh(L"gr1_n1");
            });
        flyout.Items().Append(item);
    }

    button.Flyout(flyout);
}

winrt::Windows::Foundation::Collections::IObservableVector<winrt::WuiFET::DGHeaderModel> gr1_ch()
{
    auto headers = winrt::single_threaded_observable_vector<winrt::WuiFET::DGHeaderModel>();
    if (!project->x)
        return headers;
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

    return headers;
}

int _gr1_p = 100;
int gr1_p()
{
    return _gr1_p;
}
void gr1_p(int value)
{
    if (_gr1_p != value)
    {
        _gr1_p = value;
        m_propertyChanged(*this, winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"gr1_p"));

        if (_SelectedTeacherEntry)
        {
            ystring v;
            v.Format(L"%i", value);
            _SelectedTeacherEntry->FindElementZ("Weight_Percentage", true)->SetContent(v);
        }
    }
}
std::wstring _gr1_n1;
winrt::hstring gr1_n1()
{
    return _gr1_n1.c_str();
}
void gr1_n1(winrt::hstring value)
{
    if (_gr1_n1 != value)
    {
        _gr1_n1 = value;
    }
}



winrt::Windows::Foundation::Collections::IObservableVector<DGRowModel> gr1_is()
{
    auto rows = winrt::single_threaded_observable_vector<DGRowModel>();

    if (!project->x)
        return rows;
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
            c1.WhatX(1);
            c1.pi1(gr1());
            c1.pi2(WindowFromPage(*this));

            if (_SelectedTeacherEntry) // ConstraintTeacherNotAvailableTimes
            {
                bool Put = 0;
                c1.Ptr4((long long)_SelectedTeacherEntry);
                for (auto& el : *_SelectedTeacherEntry)
                {
                    if (el.GetElementName() != "Not_Available_Time")
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

            c1.Ptr3((long long)_SelectedTeacher); // Teacher
            c1.Ptr2((long long)root);
			c1.Ptr5((long long)project);
            cells.Append(c1);
        }

        DGRowModel rr(i, cells);
        rows.Append(rr);
    }
    return rows;
}
