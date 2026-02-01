#include "pch.h"
#include "App.xaml.h"
#include "MainWindow.xaml.h"




bool is_light_theme();
extern ystring dbimportfile;

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Controls;



WNDPROC wProc = 0;
HICON hIcon1 = 0;
std::map<HWND, winrt::Windows::Foundation::IInspectable> windows;
LRESULT CALLBACK cbx(HWND hh, UINT mm, WPARAM ww, LPARAM ll)
{
    if (mm == WM_CLOSE)
    {
        for(auto& j : windows)
        {
            if (j.first == hh)
            {
				bool v = j.second.as<::WuiFET::MainWindow>().Closing();
                if (!v)
					return 0;
                break;
            }
		}
    }
    return CallWindowProc(wProc, hh, mm, ww, ll);
}


winrt::Windows::Foundation::IInspectable WindowFromPage(winrt::Windows::Foundation::IInspectable pg)
{
    for (auto& wi : windows)
    {
        auto the_window = wi.second.try_as<winrt::WuiFET::MainWindow>();
        if (!the_window)
            return nullptr;

        // Find the MainPage for it
        auto tnv = the_window.Content().try_as<winrt::Microsoft::UI::Xaml::Controls::NavigationView>();
        if (!tnv)
            return nullptr;
        auto fr = tnv.FindName(L"contentFrame").as<winrt::Microsoft::UI::Xaml::Controls::Frame>();
        if (!fr)
            return nullptr;
        auto main_page = fr.Content();
        if (!main_page)
            return nullptr;
        if (main_page == pg)
            return wi.second;
    }
    return nullptr;
}

HWND HWNDFromPage(winrt::Windows::Foundation::IInspectable pg)
{
	auto wi = WindowFromPage(pg);
    if (!wi)
		return 0;
	auto w2 = wi.as<::WuiFET::MainWindow>();
    return (HWND)w2.wnd();
}


void PostThemeChange()
{
    int Theme = SettingsX->GetRootElement().vv("Theme").GetValueInt();
    if (Theme == 0)
    {
        if (is_light_theme())
            Theme = 1;
        else
            Theme = 2;
    }

    auto et = winrt::Microsoft::UI::Xaml::ElementTheme::Dark;
	if (Theme == 1)
		et = winrt::Microsoft::UI::Xaml::ElementTheme::Light;   


    for (auto& w : windows)
    {
        BOOL value = true;
        if (Theme == 1)
            value = false;
        DwmSetWindowAttribute(w.first, DWMWA_USE_IMMERSIVE_DARK_MODE, &value, sizeof(value));
		auto w2 = w.second.as<::WuiFET::MainWindow>();

        // and any navigation page
		auto topnv = w2.Content().try_as<FrameworkElement>();
        if (topnv)
        {
			topnv.RequestedTheme(et);   

            // and the frame
            auto fr = topnv.FindName(L"contentFrame");
            if (fr)
            {
                auto fr2 = fr.try_as<Frame>();
                if (fr2)
                {
                    fr2.RequestedTheme(et);
                    auto c = fr2.Content();
                    if (c)
                    {
                        // and the content
                        auto c2 = c.try_as<FrameworkElement>();
                        if (c2)
                            c2.RequestedTheme(et);
                    }
                }
            }
            topnv.UpdateLayout();

        }
    }
}

#ifndef _DEBUG
#define NEW_WINDOW_APP
#endif

std::wstring fil;
winrt::WuiFET::MainWindow CreateWi()
{
#ifdef NEW_WINDOW_APP
    if (windows.size() > 0)
    {
        // spawn new process
        STARTUPINFO si = { sizeof(si) };
        PROCESS_INFORMATION pi = {};
        wchar_t a[1000] = {};
        GetModuleFileName(0, a, 1000);
        if (!fil.empty())
            swprintf_s(a + wcslen(a), 1000 - wcslen(a), L" \"%s\"", fil.c_str());
        fil.clear();
        if (CreateProcess(0, a, 0, 0, false, 0, 0, 0, &si, &pi))
        {
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }
        return nullptr;
    }
#endif
    winrt::WuiFET::MainWindow j;
    j.Activate();
    j.ExtendsContentIntoTitleBar(true);
    j.file(fil.c_str());
    fil.clear();
    static int One = 0;

    auto n = j.as<::IWindowNative>();
    if (n)
    {
        HWND hh;
        n->get_WindowHandle(&hh);
        if (hh)
        {
            j.wnd((int64_t)hh);


            windows[hh] = j;
            hIcon1 = LoadIcon(GetModuleHandle(0), L"ICON_1");

            wProc = (WNDPROC)GetWindowLongPtr(hh, GWLP_WNDPROC);
            SetWindowLongPtr(hh, GWLP_WNDPROC, (LONG_PTR)cbx);


            SetWindowText(hh, ttitle);
            if (One == 0)
                ShowWindow(hh, SW_SHOWMAXIMIZED);
            One = 1;
#define GCL_HICONSM         (-34)
#define GCL_HICON           (-14)
            SetClassLongPtr(hh, GCL_HICONSM, (LONG_PTR)hIcon1);
            SetClassLongPtr(hh, GCL_HICON, (LONG_PTR)hIcon1);

            if (1)
            {
                BOOL value = true;

                if (SettingsX)
                {
                    int Theme = SettingsX->GetRootElement().vv("Theme").GetValueInt();
                    if (Theme == 0)
                    {
						if (is_light_theme())
							Theme = 1;
						else
							Theme = 2;
                    }
                    if (Theme == 1)
                        value = false;
                    ::DwmSetWindowAttribute(hh, DWMWA_USE_IMMERSIVE_DARK_MODE, &value, sizeof(value));

                }
            }
        }
    }
    return j;
}








namespace winrt::WuiFET::implementation
{
    /// <summary>
    /// Initializes the singleton application object.  This is the first line of authored code
    /// executed, and as such is the logical equivalent of main() or WinMain().
    /// </summary>
    App::App()
    {
        // Xaml objects should not call InitializeComponent during construction.
        // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
        if (SettingsX)
        {
            int Theme = SettingsX->GetRootElement().vv("Theme").GetValueInt();
            if (Theme == 1)
                RequestedTheme(ApplicationTheme::Light);
            if (Theme == 2)
                RequestedTheme(ApplicationTheme::Dark);
        }

#if defined _DEBUG && !defined DISABLE_XAML_GENERATED_BREAK_ON_UNHANDLED_EXCEPTION
        UnhandledException([](IInspectable const&, UnhandledExceptionEventArgs const& e)
        {
            if (IsDebuggerPresent())
            {
                auto errorMessage = e.Message();
                __debugbreak();
            }
        });
#endif
    }

    /// <summary>
    /// Invoked when the application is launched.
    /// </summary>
    /// <param name="e">Details about the launch request and process.</param>
    void App::OnLaunched([[maybe_unused]] LaunchActivatedEventArgs const& e)
    {
        if (__argc <= 1)
            window = CreateWi();
        else
        {
            for (int i = 1; i < __argc; i++)
            {
                fil = __wargv[i];
                window = CreateWi();
            }
        }
    }
}

// Helper
CComPtr<IWICBitmap> LoadWic(const wchar_t* o);
void SaveWic(IWICBitmapSource* src, const wchar_t* out);
extern CComPtr<IWICImagingFactory> wbfact;

void ChangeAssets(const wchar_t* src, const wchar_t* dir_dest)
{
    // Wic Bitmap Loader
	auto png = LoadWic(src);
    for (const auto& entry : std::filesystem::directory_iterator(dir_dest))
    {
        auto png2 = LoadWic(entry.path().c_str());
        if (!png2)
            continue;
        UINT wi = 0, he = 0;
        png2->GetSize(&wi, &he);

		CComPtr<IWICBitmapScaler> pScaler;  
		wbfact->CreateBitmapScaler(&pScaler);
		pScaler->Initialize(png, wi, he, WICBitmapInterpolationModeHighQualityCubic);

        png2 = 0;
        DeleteFile(entry.path().c_str());
		SaveWic(pScaler, entry.path().c_str());
    }
}

wchar_t MainDirectory[MAX_PATH] = {};
#include "rdstore.hpp"
int __stdcall wWinMain(HINSTANCE h, HINSTANCE, [[maybe_unused]] PWSTR t, int)
{
    SetPriorityClass(GetCurrentProcess(), IDLE_PRIORITY_CLASS);
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_IDLE);
    if (1)
    {
        GetModuleFileName(0, MainDirectory, MAX_PATH);
        wchar_t* s = wcsrchr(MainDirectory, L'\\');
        if (s)
            *s = 0;
    }

    CoInitializeEx(0, COINIT_APARTMENTTHREADED);
#ifdef _DEBUG
//    ChangeAssets(L"f:\\wuitools\\WuiFET\\app.png", L"f:\\wuitools\\WuiFET\\assets");
    //int trsmain2();        trsmain2();
#endif
    hIcon1 = LoadIcon(h, L"ICON_1");
    {
        void (WINAPI * pfnXamlCheckProcessRequirements)();
        auto module = ::LoadLibrary(L"Microsoft.ui.xaml.dll");
        if (module)
        {
            pfnXamlCheckProcessRequirements = reinterpret_cast<decltype(pfnXamlCheckProcessRequirements)>(GetProcAddress(module, "XamlCheckProcessRequirements"));
            if (pfnXamlCheckProcessRequirements)
            {
                (*pfnXamlCheckProcessRequirements)();
            }

            ::FreeLibrary(module);
        }
    }

    PWSTR p = 0;
    SHGetKnownFolderPath(FOLDERID_ProgramData, 0, 0, &p);
    std::wstring de = p;
    CoTaskMemFree(p);

    de += L"\\29410c69-7a2b-4a2b-9893-9de712a5cd77";
    SHCreateDirectory(0, de.c_str());
    datafolder = de.c_str();
    std::wstring sf = de + L"\\settings.xml";
    SettingsX = std::make_shared<XML3::XML>(sf.c_str());


    StoreRedirect();

    winrt::init_apartment(winrt::apartment_type::single_threaded);
    ::winrt::Microsoft::UI::Xaml::Application::Start(
        [](auto&&)
        {
            ::winrt::make<::winrt::WuiFET::implementation::App>();
        });

    SettingsX->Save();
#ifndef _DEBUG
    if (!dbimportfile.empty())
        DeleteFile(dbimportfile.c_str());
#endif
    ExitProcess(0);
    return 0;
}

