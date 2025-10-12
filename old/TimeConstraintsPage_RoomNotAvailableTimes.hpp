
// WhatY 1: NotAvailableTimes
void dd3Click(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs)
{
    if (!_SelectedRoom)
        return;
    auto button = sender.as<DropDownButton>();
    MenuFlyout flyout;
    flyout.Placement(Microsoft::UI::Xaml::Controls::Primitives::FlyoutPlacementMode::Bottom);

    ystring SelectedRoomName = trim(_SelectedRoom->FindElementZ("Name", true)->GetContent());
    auto x = project->x;
    XML3::XMLElement* root = &x->GetRootElement();
    auto r0 = root->FindElementZ("Space_Constraints_List", true);

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
                _SelectedRoomEntry = 0;
                _gr3_p = p;
                Refresh(L"gr3_p");
                Refresh(L"gr3_is");
                Refresh(L"gr3_ch");
                _gr3_n1 = s(83);
                Refresh(L"gr3_n1");
            });
        flyout.Items().Append(item);
    }

    int yy = 0;
    for (auto& r1 : *r0)
    {
        if (r1.GetElementName() != "ConstraintRoomNotAvailableTimes")
            continue;
        // This Room?
        auto tn = trim(r1.FindElementZ("Room", true)->GetContent());
        if (tn != SelectedRoomName)
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
                _SelectedRoomEntry = (XML3::XMLElement*)unbox_value<long long>(it.Tag());
                _gr3_p = _wtoi(trim(_SelectedRoomEntry->FindElementZ("Weight_Percentage", true)->GetContent()).c_str());
                Refresh(L"gr3_p");
                Refresh(L"gr3_is");
                Refresh(L"gr3_ch");
                ystring y = it.Text().c_str();
                wchar_t buf[356] = {};
				wcscpy_s(buf, 356, y.c_str());
				auto ch = wcschr(buf, L'(');
				if (ch)
					*ch = 0;
				_gr3_n1 = buf;
                Refresh(L"gr3_n1");
            });
        flyout.Items().Append(item);
    }

    button.Flyout(flyout);
}

winrt::Windows::Foundation::Collections::IObservableVector<winrt::WuiFET::DGHeaderModel> gr3_ch()
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

int _gr3_p = 100;
int gr3_p()
{
    return _gr3_p;
}
void gr3_p(int value)
{
    if (_gr3_p != value)
    {
        _gr3_p = value;
        m_propertyChanged(*this, winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"gr3_p"));

        if (_SelectedRoomEntry)
        {
            ystring v;
            v.Format(L"%i", value);
            _SelectedRoomEntry->FindElementZ("Weight_Percentage", true)->SetContent(v);
        }
    }
}
std::wstring _gr3_n1;
winrt::hstring gr3_n1()
{
    return _gr3_n1.c_str();
}
void gr3_n1(winrt::hstring value)
{
    if (_gr3_n1 != value)
    {
        _gr3_n1 = value;
    }
}



winrt::Windows::Foundation::Collections::IObservableVector<DGRowModel> gr3_is()
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
            c1.WhatX(3);
            c1.pi1(gr3());
            c1.pi3(WindowFromPage(*this));

			if (_SelectedRoomEntry) // ConstraintRoomNotAvailableTimes
            {
                bool Put = 0;
                c1.Ptr4((long long)_SelectedRoomEntry);
                for (auto& el : *_SelectedRoomEntry)
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

            c1.Ptr3((long long)_SelectedRoom); // Room
            c1.Ptr2((long long)root);
			c1.Ptr5((long long)project);
            cells.Append(c1);
        }

        DGRowModel rr(i, cells);
        rows.Append(rr);
    }
    return rows;
}
