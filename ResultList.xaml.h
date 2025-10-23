#pragma once

#include "ResultList.g.h"

extern ystring dbimportfile;

winrt::Windows::Foundation::IInspectable WindowFromPage(winrt::Windows::Foundation::IInspectable pg);
namespace winrt::WuiFET::implementation
{
    struct ResultList : ResultListT<ResultList>
    {
        ResultList()
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
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

        void Refresh(const wchar_t* s)
        {
            m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{ s });
        }

        void Refresh()
        {
            m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{ L"" });
        }
        void PushUndo()
        {
            auto wi = WindowFromPage(*this);
            if (!wi) return;
            wi.as<winrt::WuiFET::MainWindow>().PushUndo();
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
        winrt::hstring Loaded_File()
        {
            return project->file.c_str();
        }


        void OnResultSelected_ExportFET(IInspectable, IInspectable)
        {
            auto lv = ResultsList();
            if (!project->x)
                return;
            auto x = project->x;
            auto sels = lv.SelectedItems();
            for (auto s1 : sels)
            {
                auto it = s1.as<winrt::WuiFET::Item>();
                if (it && it.ptr())
                {
                    auto result = (XML3::XMLElement*)it.ptr();
                    if (result->GetChildrenNum() < 4)
                        return;


                    auto fet = result->GetChildren()[3];
                    OPENFILENAME of = { 0 };
                    of.lStructSize = sizeof(of);
                    of.hwndOwner = (HWND)0;
                    wchar_t filt[100] = {};
                    swprintf_s(filt, 100, L"%s\0*.fet\0\0", s(127));
                    of.lpstrFilter = filt;
                    std::vector<wchar_t> fnx(10000);
                    of.lpstrFile = fnx.data();
                    of.nMaxFile = 10000;
                    of.lpstrDefExt = L"fet";
                    of.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
                    if (!GetSaveFileName(&of))
                        return;

					DeleteFile(fnx.data());
                    XML3::XML x5;
					x5.SetRootElement(*fet);
					x5.Save(fnx.data());
                    Locate(fnx.data());


                }
            }
        }

        void OnResultSelected_PostMSAAPPS(IInspectable, IInspectable)
        {
            auto lv = ResultsList();
            if (!project->x)
                return;
            auto x = project->x;
            auto sels = lv.SelectedItems();
            for (auto s1 : sels)
            {
                auto it = s1.as<winrt::WuiFET::Item>();
                if (it && it.ptr())
                {
                    auto result = (XML3::XMLElement*)it.ptr();
                    if (result->GetChildrenNum() < 4)
                        return;

                    std::string sx1,sx2,sx3,sx4;

                    sx1 = result->GetChildren()[3]->Serialize(); // fet
					sx2 = result->GetChildren()[2]->Serialize(); //activities
					sx3 = result->GetChildren()[1]->Serialize(); // teachers
					sx4 = result->GetChildren()[0]->Serialize(); // subgroups


                    void BuildNewVersion(const char* dbf, std::string groups, std::string teachers, std::string activities);
                    if (dbimportfile.empty())
                    {
                        OPENFILENAME of = { 0 };
                        of.lStructSize = sizeof(of);
                        of.hwndOwner = (HWND)0;
                        wchar_t filt[100] = {};
                        swprintf_s(filt, 100, L"*.db\0*.db\0\0");
                        of.lpstrFilter = filt;
                        std::vector<wchar_t> fnx(10000);
                        of.lpstrFile = fnx.data();
                        of.nMaxFile = 10000;
                        of.lpstrDefExt = L"db";
                        of.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
                        if (!GetOpenFileName(&of))
                            return;
						dbimportfile = fnx.data();

                    }
                    BuildNewVersion(dbimportfile.a_str(), sx4, sx3, sx2);

                }
            }
        }

        void OnResultSelected_View(IInspectable, IInspectable)
        {
            auto lv = ResultsList();
            if (!project->x)
                return;
            auto x = project->x;
            auto sels = lv.SelectedItems();
            for (auto s : sels)
            {
                auto it = s.as<winrt::WuiFET::Item>();
                if (it && it.ptr())
                {
                    auto result = (XML3::XMLElement*)it.ptr();

                    std::thread thread1([=]()
                        {
                            Sleep(250);
                            DispatcherQueue().TryEnqueue([=]()
                                {
                                    winrt::WuiFET::ViewResult j;
                                    j.ResultX((long long)result);
                                    j.ResultP((long long)project);
                                    j.Activate();
                                    j.ExtendsContentIntoTitleBar(true);
                                });
                        });
                    thread1.detach();

                }
            }

        }

        void DoubleClick1(IInspectable sender, IInspectable e)
        {
			OnResultSelected_View(sender, e);
        }

        void OnResultSelected_Delete(IInspectable sender, IInspectable)
        {
            auto lv = ResultsList();
            if (!project->x)
                return;
            PushUndo();
            auto x = project->x;
            auto sels = lv.SelectedItems();

            if (sels.Size() == 0)
            {
                // Get the right tapped item
                auto menuItem = sender.as<Microsoft::UI::Xaml::Controls::MenuFlyoutItem>();
                auto target = menuItem.DataContext().as<winrt::WuiFET::Item>();
                if (target)
                    sels.Append(target);
            }

            for (auto s : sels)
            {
                auto it = s.as<winrt::WuiFET::Item>();
                if (it && it.ptr())
                {
                    auto par2 = ((XML3::XMLElement*)it.ptr())->GetParent(&x->GetRootElement());
                    if (par2)
                    {
                        for (size_t i = 0; i < par2->GetChildren().size(); i++)
                        {
                            if (par2->GetChildren()[i].get() == (XML3::XMLElement*)it.ptr())
                            {
                                // Delete 
                                par2->RemoveElement(i);
                                break;
                            }
                        }
                    }
                }
            }

            Refresh(L"Results_List");
        }

        winrt::Windows::Foundation::Collections::IObservableVector<winrt::WuiFET::Item> Results_List()
        {
            auto items = winrt::single_threaded_observable_vector<winrt::WuiFET::Item>();
            if (!project)
                return items;
            if (!project->x)
                return items;
            auto x = project->x;
            auto& r = x->GetRootElement()[MY_FET_TAG]["Results"];
            for (auto& rr : r)
            {
                ULARGE_INTEGER ul = {};
				ul.QuadPart = rr.vv("ts").GetValueULongLong();
				long long seconds_finished = rr.vv("finishseconds").GetValueLongLong();
				SYSTEMTIME st = {};
				FILETIME ft = {};
				ft.dwLowDateTime = ul.LowPart;
				ft.dwHighDateTime = ul.HighPart;
				FileTimeToSystemTime(&ft, &st);
				wchar_t t[1000] = {};
				swprintf_s(t, 1000, L"%04d-%02d-%02d %02d:%02d:%02d (%02zi:%02zi:%02zi)", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond,
					seconds_finished / 3600, (seconds_finished % 3600) / 60, (seconds_finished % 60)
                    );
                winrt::WuiFET::Item it;
                it.Name1(t);
                swprintf_s(t, 1000, L"%llu %llu %llu %llu %llu %llu",
                    rr.vv("seed10").GetValueULongLong(),
                    rr.vv("seed11").GetValueULongLong(),
                    rr.vv("seed12").GetValueULongLong(),
                    rr.vv("seed20").GetValueULongLong(),
                    rr.vv("seed21").GetValueULongLong(),
                    rr.vv("seed22").GetValueULongLong()
                );
                it.Name2(t);
                it.ptr((long long)&rr);
                items.Append(it);
            }
            return items;
        }

     
    };
}

namespace winrt::WuiFET::factory_implementation
{
    struct ResultList : ResultListT<ResultList, implementation::ResultList>
    {
    };
}
