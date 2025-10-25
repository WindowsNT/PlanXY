

class PC
{
private:

    // - Close these after CreateProcess of child application with pseudoconsole object.
    HANDLE inputReadSide = 0, outputWriteSide = 0;

    // - Hold onto these and use them for communication with the child through the pseudoconsole.
    HANDLE outputReadSide = 0, inputWriteSide = 0;

    HPCON hPC = 0;


public:


    HANDLE hProcess = 0;
    HANDLE hThread = 0;
    STARTUPINFOEX siEx = {};

    ~PC()
    {
        Off();
	}

    HRESULT SetUpPseudoConsole(COORD size)
    {
        HRESULT hr = S_OK;

        // Create communication channels

        if (!CreatePipe(&inputReadSide, &inputWriteSide, NULL, 0))
        {
            return HRESULT_FROM_WIN32(GetLastError());
        }

        if (!CreatePipe(&outputReadSide, &outputWriteSide, NULL, 0))
        {
            return HRESULT_FROM_WIN32(GetLastError());
        }

        hr = CreatePseudoConsole(size, inputReadSide, outputWriteSide, 0, &hPC);
        if (FAILED(hr))
        {
            return hr;
        }

        return hr;
    }

    HRESULT Run(const wchar_t* childApplication)
    {
        // Create mutable text string for CreateProcessW command line string.
        std::vector<wchar_t> cmdLineMutable(wcslen(childApplication) + 100);
        wcscpy_s(cmdLineMutable.data(), wcslen(childApplication) + 100, childApplication);

        PROCESS_INFORMATION pi = {};

        // Call CreateProcess
        if (!CreateProcessW(NULL,
            cmdLineMutable.data(),
            NULL,
            NULL,0,
            
            EXTENDED_STARTUPINFO_PRESENT,
            NULL,
            NULL,
            (STARTUPINFO*)&siEx.StartupInfo,
            &pi))
        {
            return HRESULT_FROM_WIN32(GetLastError());
        }
        hProcess = pi.hProcess;
        hThread = pi.hThread;

        return S_OK;
    }

    HRESULT PrepareStartupInformation()
    {
		STARTUPINFOEX* psi = &siEx;
        // Prepare Startup Information structure
        STARTUPINFOEX si = {};
        si.StartupInfo.cb = sizeof(STARTUPINFOEX);

        // Discover the size required for the list
        size_t bytesRequired;
        InitializeProcThreadAttributeList(NULL, 1, 0, &bytesRequired);

        // Allocate memory to represent the list
        si.lpAttributeList = (PPROC_THREAD_ATTRIBUTE_LIST)HeapAlloc(GetProcessHeap(), 0, bytesRequired);
        if (!si.lpAttributeList)
        {
            return E_OUTOFMEMORY;
        }

        // Initialize the list memory location
        if (!InitializeProcThreadAttributeList(si.lpAttributeList, 1, 0, &bytesRequired))
        {
            HeapFree(GetProcessHeap(), 0, si.lpAttributeList);
            return HRESULT_FROM_WIN32(GetLastError());
        }

        // Set the pseudoconsole information into the list
        if (!UpdateProcThreadAttribute(si.lpAttributeList,
            0,
            PROC_THREAD_ATTRIBUTE_PSEUDOCONSOLE,
            hPC,
            sizeof(hPC),
            NULL,
            NULL))
        {
            HeapFree(GetProcessHeap(), 0, si.lpAttributeList);
            return HRESULT_FROM_WIN32(GetLastError());
        }

        *psi = si;

        return S_OK;
    }


    // Theoretical event handler function with theoretical
// event that has associated display properties
// on Source property.
    void OnWindowResize(COORD size)
    {
        // Call pseudoconsole API to inform buffer dimension update
        ResizePseudoConsole(hPC, size);
    }


    void Sleep(int ms)
    {
		WaitForSingleObject(hProcess, ms);
    }

    void Break()
    {
		// SEnd a Ctrl+Break event to the pseudoconsole
        DWORD pid = GetProcessId(hProcess);
		GenerateConsoleCtrlEvent(CTRL_BREAK_EVENT, pid);

        DWORD written = 0;
        WriteFile(outputWriteSide, "\x1b[3;5~", 6, &written, NULL);
    }

    void Off()
    {
        if (outputReadSide)
        {
            CloseHandle(outputReadSide);
            outputReadSide = 0;
        }
        if (inputWriteSide)
        {
            CloseHandle(inputWriteSide);
            inputWriteSide = 0;
		}
        if (hThread)
        {
            CloseHandle(hThread);
            hThread = 0;
		}
        if (hProcess)
        {
			TerminateProcess(hProcess, 0);
            CloseHandle(hProcess);
            hProcess = 0;
        }
        if (hPC)
        {
            ClosePseudoConsole(hPC);
            hPC = 0;
		}
    }

};