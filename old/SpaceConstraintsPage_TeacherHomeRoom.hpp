


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
        if (_SelectedTeacherEntry)
			_SelectedTeacherEntry->FindElementZ("Weight_Percentage", true)->SetContent(std::to_string(_gr1_p).c_str());

    }
}


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
                _SelectedTeacherEntry = 0;
                _gr1_p = p;
                Refresh(L"gr1_p");
                Refresh(L"gr1_rl");
                _gr1_n1 = s(83);
                Refresh(L"gr1_n1");
            });
        flyout.Items().Append(item);
    }

    int yy = 0;
    for (auto& r1 : *r0)
    {
        if (r1.GetElementName() != "ConstraintTeacherHomeRoom")
            continue;
        // This teacher?
        auto tn = trim(r1.FindElementZ("Teacher", true)->GetContent());
        if (tn != SelectedTeacherName)
            continue;
        ystring y;
        auto whatper = _wtoi(trim(r1.FindElementZ("Weight_Percentage", true)->GetContent()).c_str());
        yy++;
        y.Format(L"%s #%i (%i%%)", s(82), yy, whatper);


        MenuFlyoutItem item;
        item.Text(y.c_str());
        item.Tag(winrt::box_value((long long)&r1));
        item.Click([&](IInspectable const& sender, RoutedEventArgs const&)
            {
                auto it = sender.as<MenuFlyoutItem>();
                _SelectedTeacherEntry = (XML3::XMLElement*)unbox_value<long long>(it.Tag());
                _gr1_p = _wtoi(trim(_SelectedTeacherEntry->FindElementZ("Weight_Percentage", true)->GetContent()).c_str());
                Refresh(L"gr1_p");
                Refresh(L"gr1_rl");
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

bool MustTC1 = 0;
void gr1_changing(IInspectable, ContainerContentChangingEventArgs args)
{
    if (!args.InRecycleQueue() && args.ItemIndex() == 0)
    {
    }
    else
        return;
    if (MustTC1)
    {
        auto lv = gr1_rooms();
        for (auto it : lv.Items())
        {
            auto mit = it.as<winrt::WuiFET::Item>();
            if (mit.Sel())
            {
				lv.SelectedItem(mit);
                break;
            }
        }
        MustTC1 = 0;
    }
}

winrt::Windows::Foundation::Collections::IObservableVector<winrt::WuiFET::Item> gr1_rl()
{
    auto items = winrt::single_threaded_observable_vector<winrt::WuiFET::Item>();

    if (!project->x)
        return items;
    auto x = project->x;

    auto& r = x->GetRootElement()["Rooms_List"];
    for (auto& rr : r)
    {
        std::shared_ptr<XML3::XMLElement> ee = rr.FindElementZ("Name", true);
        ystring name = trim(ee->GetContent());

        winrt::WuiFET::Item it;
        it.Name1(name.c_str());

        // If selected
        if (_SelectedTeacherEntry) // ConstraintTeacherNotAvailableTimes
        {
            it.Ptr4((long long)_SelectedTeacherEntry);
            for (auto& el : *_SelectedTeacherEntry)
            {
                if (el.GetElementName() != "Room")
                    continue;
				auto val = trim(el.GetContent());
                if (val != name)
					continue;
				it.ptr((long long)&rr);
                it.Sel(true);
                break;
            }
        }
        items.Append(it);
    }
    MustTC1 = 1;
    return items;
}

void gr1_selectionchanged(IInspectable, IInspectable)
{
    if (MustTC1 == 1)
        return;
    auto lv = gr1_rooms();
    auto sids = lv.SelectedItem();
    if (!sids)
    {
        // Clear the ptr
        if (_SelectedTeacherEntry)
        {
			auto wi = WindowFromPage(*this).as<winrt::WuiFET::MainWindow>();
            wi.Dirty();
			auto par = _SelectedTeacherEntry->GetParent(&project->x->GetRootElement());
            if (par)
				par->RemoveElement(_SelectedTeacherEntry);
            _SelectedTeacherEntry = 0;
            _gr1_p = 100;
            Refresh(L"gr1_p");
        }
        else
        {
            // Nothing, its not there anyway
        }
    }
    else
    {
        if (!_SelectedTeacherEntry)
        {
			auto root = &project->x->GetRootElement();
            auto r0 = root->FindElementZ("Space_Constraints_List", true);
			auto& el = r0->AddElement("ConstraintTeacherHomeRoom");
			el.AddElement("Teacher").SetContent(trim(_SelectedTeacher->FindElementZ("Name", true)->GetContent()).a_str());
			el.AddElement("Weight_Percentage").SetContent(std::to_string(_gr1_p).c_str());
			el.AddElement("Room").SetContent(trim(sids.as<winrt::WuiFET::Item>().Name1().c_str()).a_str());
			el.AddElement("Active").SetContent("true");
            _SelectedTeacherEntry = &el;
        }
        if (_SelectedTeacherEntry)
        {
            auto wi = WindowFromPage(*this).as<winrt::WuiFET::MainWindow>();
            wi.Dirty();
			_SelectedTeacherEntry->FindElementZ("Room", true)->SetContent(trim(sids.as<winrt::WuiFET::Item>().Name1().c_str()).a_str());
        }
    }
}
