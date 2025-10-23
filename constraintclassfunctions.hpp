
XML3::XMLElement* _SelectedZZ21 = 0;
XML3::XMLElement* _SelectedZZ21Entry = 0;

void zz21_LeftChanged(ListView lv)
{
    if (!ViewingConstraint)
        return;
    auto sel = lv.SelectedItem().as<winrt::WuiFET::Item>();
    _SelectedZZ21 = 0;
    if (sel)
        _SelectedZZ21 = (XML3::XMLElement*)sel.ptr();
    _SelectedZZ21Entry = nullptr;

    _zz21_n2 = ViewingConstraint->d0;
    _zz21_HasP = (ViewingConstraint->w_from < 100);
	_zz21_HasM = ViewingConstraint->SupportsMultiple;
    _zz21_A = 0;


    if (_SelectedZZ21)
    {
        // Select the first if it exists
        auto x = project->x;
        ystring SelectedItemName = trim(_SelectedZZ21->FindElementZ("Name", true)->GetContent());
        XML3::XMLElement* root = &x->GetRootElement();

        const char* s1 = "Time_Constraints_List";
        if (ViewingConstraint->Type == 1)
			s1 = "Space_Constraints_List";
        auto r0 = root->FindElementZ(s1, true);


        // New Entry default
        ystring y;
        y.Format(L"%s", s(83));
        _zz21_n1 = y;
        _zz21_p = 100;

        for (auto& r1 : *r0)
        {
            if (r1.GetElementName() != ViewingConstraint->x0)
                continue;
            // This teacher?
			const char* s2 = "Teacher";
			if (ViewingConstraint->SubType == 2)
				s2 = "Students";
			if (ViewingConstraint->SubType == 3)
				s2 = "Room";

            auto tn = trim(r1.FindElementZ(s2, true)->GetContent());
            if (tn != SelectedItemName)
                continue;
            _SelectedZZ21Entry = &r1; // default editable the first one

            // Is Active?
            if (_SelectedZZ21Entry->FindElementZ("Active", true))
            {
                auto ac = trim(_SelectedZZ21Entry->FindElementZ("Active", true)->GetContent());
                if (ac == L"1" || ac == L"True" || ac == L"true")
                    _zz21_A = 1;
			}

            int p = 100;
            p = _wtoi(trim(_SelectedZZ21Entry->FindElementZ("Weight_Percentage", true)->GetContent()).c_str());
            _zz21_p = p;
            Refresh(L"zz21_A");
            Refresh(L"zz21_p");
            Refresh(L"zz21_is");
            Refresh(L"zz21_ch");

            // There is one found, change y
            y.Format(L"%s #%i", s(82), 1);
            _zz21_n1 = y;
            break;
        }

    }

    Refresh(L"WhatY");
    Refresh(L"zz21_n1");
    Refresh(L"zz21_n2");
    Refresh(L"zz21_p");
    Refresh(L"zz21_A");
    Refresh(L"zz21_HasP");
    Refresh(L"zz21_HasM");
    Refresh(L"zz21_is");
    Refresh(L"zz21_ch");

}

void zz21_NoLeftInit()
{
    _zz21_n2 = ViewingConstraint->d0;
    _zz21_HasP = (ViewingConstraint->w_from < 100);
    _zz21_HasM = ViewingConstraint->SupportsMultiple;
    _zz21_A = 0;
    _SelectedZZ21Entry = 0;

    auto x = project->x;
    XML3::XMLElement* root = &x->GetRootElement();
    const char* s1 = "Time_Constraints_List";
    if (ViewingConstraint->Type == 1)
        s1 = "Space_Constraints_List";
    auto r0 = root->FindElementZ(s1, true);

    // Search for the entry
    // New Entry default
    ystring y;
    y.Format(L"%s", s(83));
    _zz21_n1 = y;

    for (auto& r1 : *r0)
    {
        if (r1.GetElementName() != ViewingConstraint->x0)
            continue;
        _SelectedZZ21Entry = &r1; // default editable the first one

        // Is Active?
        if (_SelectedZZ21Entry->FindElementZ("Active", true))
        {
            auto ac = trim(_SelectedZZ21Entry->FindElementZ("Active", true)->GetContent());
            if (ac == L"1" || ac == L"True" || ac == L"true")
                _zz21_A = 1;
        }

        int p = 100;
        p = _wtoi(trim(_SelectedZZ21Entry->FindElementZ("Weight_Percentage", true)->GetContent()).c_str());
        _zz21_p = p;
        Refresh(L"zz21_A");
        Refresh(L"zz21_p");
        Refresh(L"zz21_is");
        Refresh(L"zz21_ch");

        // There is one found, change y
        y.Format(L"%s #%i", s(82), 1);
        _zz21_n1 = y;
        break;
    }



}

void zz21_Click(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs)
{
    if (!ViewingConstraint)
		return;
    if (!ViewingConstraint->SupportsMultiple)
        return;

    if (!_SelectedZZ21)
        return;
    auto button = sender.as<DropDownButton>();
    MenuFlyout flyout;
    flyout.Placement(Microsoft::UI::Xaml::Controls::Primitives::FlyoutPlacementMode::Bottom);

    ystring SelectedZZ21Name = trim(_SelectedZZ21->FindElementZ("Name", true)->GetContent());
    auto x = project->x;
    XML3::XMLElement* root = &x->GetRootElement();

    const char* s1 = "Time_Constraints_List";
    if (ViewingConstraint->Type == 1)
        s1 = "Space_Constraints_List";
    auto r0 = root->FindElementZ(s1, true);

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
                _SelectedZZ21Entry = 0;
                _zz21_p = p;
                Refresh(L"zz21_p");
                Refresh(L"zz21_is");
                Refresh(L"zz21_ch");
                _zz21_n1 = s(83);
                Refresh(L"zz21_n1");
            });
        flyout.Items().Append(item);
    }



    int yy = 0;
    for (auto& r1 : *r0)
    {
        if (r1.GetElementName() != ViewingConstraint->x0)
            continue;
        // This ZZ21?

        const char* s2 = "Teacher";
        if (ViewingConstraint->SubType == 2)
            s2 = "Students";
        if (ViewingConstraint->SubType == 3)
            s2 = "Room";

        auto tn = trim(r1.FindElementZ(s2, true)->GetContent());
        if (tn != SelectedZZ21Name)
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
                _SelectedZZ21Entry = (XML3::XMLElement*)unbox_value<long long>(it.Tag());
                _zz21_p = _wtoi(trim(_SelectedZZ21Entry->FindElementZ("Weight_Percentage", true)->GetContent()).c_str());
                Refresh(L"zz21_p");
                Refresh(L"zz21_is");
                Refresh(L"zz21_ch");
                ystring y = it.Text().c_str();
                wchar_t buf[256] = {};
                wcscpy_s(buf, 256, y.c_str());
                auto ch = wcschr(buf, L'(');
                if (ch)
                    *ch = 0;
                _zz21_n1 = buf;
                Refresh(L"zz21_n1");
            });
        flyout.Items().Append(item);
    }

    button.Flyout(flyout);
}

winrt::Windows::Foundation::Collections::IObservableVector<winrt::WuiFET::DGHeaderModel> zz21_ch()
{
    auto headers = winrt::single_threaded_observable_vector<winrt::WuiFET::DGHeaderModel>();
    return headers;
}

int _zz21_p = 100;
int zz21_p()
{
    return _zz21_p;
}
void zz21_p(int value)
{
    if (_zz21_p != value)
    {
        _zz21_p = value;
        m_propertyChanged(*this, winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"zz21_p"));

        if (_SelectedZZ21Entry)
        {
            ystring v;
            v.Format(L"%i", value);
            _SelectedZZ21Entry->FindElementZ("Weight_Percentage", true)->SetContent(v);
        }
        
    }
}
std::wstring _zz21_n1;
winrt::hstring zz21_n1()
{
    return _zz21_n1.c_str();
}
void zz21_n1(winrt::hstring value)
{
    if (_zz21_n1 != value)
    {
        _zz21_n1 = value;
    }
}

bool _zz21_A = false;
bool zz21_A()
{
    return _zz21_A;
}
void zz21_A(bool value)
{
    if (_zz21_A != value)
    {
        _zz21_A = value;

        if (_SelectedZZ21Entry && !value)
        {
            // Remove it
			auto wi = WindowFromPage(*this).as<winrt::WuiFET::MainWindow>();
            wi.Dirty();

			auto par = _SelectedZZ21Entry->GetParent(&project->x->GetRootElement());
            if (par)
				par->RemoveElement(_SelectedZZ21Entry);
		}
        if (!_SelectedZZ21Entry && value)
        {
            // Add it
            auto x = project->x;
            XML3::XMLElement* root = &x->GetRootElement();
            const char* s1 = "Time_Constraints_List";
            if (ViewingConstraint->Type == 1)
                s1 = "Space_Constraints_List";
            auto r0 = root->FindElementZ(s1, true);
            auto& newel = r0->AddElement(ViewingConstraint->x0.c_str());
            newel.AddElement("Active").SetContent("True");
            newel.AddElement("Weight_Percentage").SetContent(std::to_string(_zz21_p).c_str());
            _SelectedZZ21Entry = &newel;
            if (_SelectedZZ21 && ViewingConstraint->SubType != 0)
            {
                const char* s2 = "Teacher";
                if (ViewingConstraint->SubType == 2)
                    s2 = "Students";
                if (ViewingConstraint->SubType == 3)
                    s2 = "Room";
                newel.AddElement(s2).SetContent(trim(_SelectedZZ21->FindElementZ("Name", true)->GetContent()));
            }
            // And the parameters
            auto items = zz21_gr().ItemsSource();
            int paridx = -1;
            for (auto it : items)
            {
                paridx++;
                auto row = it.as<DGRowModel>();
                // Each row has two columns
                if (row.Cells().Size() != 2)
                    continue;
                auto cell = row.Cells().GetAt(0);
                if (cell.Type() != DGCellType::Block)
                    continue;
                if (!cell.Ptr())
                    continue;

				auto valuecell = row.Cells().GetAt(1);

                // ptr = &CONSTRAINT_PARAM
                // ptr3 = &A_CONSTRAINT
                // ptr2 = XMLElement* of change
                if (ViewingConstraint->params.size() < paridx)
					continue;

				auto& param = ViewingConstraint->params[paridx];
				valuecell.WhatX(11);
				valuecell.Ptr3((long long)ViewingConstraint.get());
                valuecell.Ptr((long long)&param);
                auto paramel = newel.FindElementZ(param.x1.c_str(), true);
				valuecell.Ptr2((long long)paramel.get());
				paramel->SetContent(std::to_string(valuecell.ValueN()).c_str());
            }
            #ifdef _DEBUG
                auto st2 = newel.Serialize();
            st2;
#endif
        }

    }
}

bool _zz21_HasP = false;
bool zz21_HasP()
{
    return _zz21_HasP;
}
void zz21_HasP(bool value)
{
    if (_zz21_HasP != value)
    {
        _zz21_HasP = value;
        m_propertyChanged(*this, winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"zz21_HasP"));
    }
}
bool _zz21_HasM = false;
bool zz21_HasM()
{
    return _zz21_HasM;
}
void zz21_HasM(bool value)
{
    if (_zz21_HasM != value)
    {
        _zz21_HasM = value;
        m_propertyChanged(*this, winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"zz21_HasM"));
    }
}

std::wstring _zz21_n2;
winrt::hstring zz21_n2()
{
    return _zz21_n2.c_str();
}
void zz21_n2(winrt::hstring value)
{
    if (_zz21_n2 != value)
    {
        _zz21_n2 = value;
    }
}


winrt::Windows::Foundation::Collections::IObservableVector<DGRowModel> zz21_is()
{
    auto rows = winrt::single_threaded_observable_vector<DGRowModel>();
    if (!project->x)
        return rows;
    if (!ViewingConstraint)
        return rows;

 //   auto x = project->x;
//    XML3::XMLElement* root = &x->GetRootElement();

    for (int row = 0; row < ViewingConstraint->params.size(); row++)
    {
		auto& param = ViewingConstraint->params[row];
        auto cells = winrt::single_threaded_observable_vector<DGCellModel>();

        // Name 
        DGCellModel c1(row, 0, DGCellType::Block,param.d1);
		c1.Ptr((long long)&param);
        cells.Append(c1);

        // Value
        if (param.type == 0 || param.type == 1)
        {
            DGCellModel c2(row, 1, param.type == 1 ? DGCellType::Check : DGCellType::Number, L"");
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
            if (_SelectedZZ21Entry)
            {
                auto el = _SelectedZZ21Entry->FindElementZ(param.x1.c_str(), true);
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



        DGRowModel rr(row, cells);
        rows.Append(rr);
    }


    return rows;
}


