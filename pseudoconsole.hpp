

class PC
{
private:

    // - Close these after CreateProcess of child application with pseudoconsole object.
    HANDLE inputReadSide = 0, outputWriteSide = 0;

    // - Hold onto these and use them for communication with the child through the pseudoconsole.
    HANDLE outputReadSide = 0, inputWriteSide = 0;

    HPCON hPC = 0;
    bool UsePC = 0;

public:


    HANDLE hProcess = 0;
    DWORD ProcessID = 0;
    DWORD MainThreadID = 0;
    HANDLE hThread = 0;
    STARTUPINFOEX siEx = {};

    ~PC()
    {
        Off(false);
	}

    HRESULT SetUpPseudoConsole(COORD size)
    {
        HRESULT hr = S_OK;
        if (!UsePC)
            return S_FALSE;

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

    ystring workingdir;

    HRESULT Run(ystring workdir,const wchar_t* childApplication,DETACHDATA* dd)
    {
        if (dd)
        {
			hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dd->PID);
			hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, dd->TID);
			ResumeThread(hThread);
			workingdir = workdir;
			ProcessID = dd->PID;
			MainThreadID = dd->TID;
			return S_OK;
        }
        else
        {
			workingdir = workdir;
            // Create mutable text string for CreateProcessW command line string.
            std::vector<wchar_t> cmdLineMutable(wcslen(childApplication) + 100);
            wcscpy_s(cmdLineMutable.data(), wcslen(childApplication) + 100, childApplication);

            PROCESS_INFORMATION pi = {};
            STARTUPINFO si = {};
            si.cb = sizeof(STARTUPINFO);

            // Call CreateProcess
            if (!CreateProcessW(NULL,
                cmdLineMutable.data(),
                NULL,
                NULL, 0,

                UsePC ? EXTENDED_STARTUPINFO_PRESENT : CREATE_NO_WINDOW,
                NULL,
                NULL,
                UsePC ? (STARTUPINFO*)&siEx.StartupInfo : &si,
                &pi))
            {
                return HRESULT_FROM_WIN32(GetLastError());
            }
            hProcess = pi.hProcess;
            hThread = pi.hThread;
        }
        ProcessID = GetProcessId(hProcess);
		MainThreadID = GetThreadId(hThread);
        return S_OK;
    }

    HRESULT PrepareStartupInformation()
    {
        if (!UsePC)
			return S_FALSE;
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
        if (hPC)
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

    void Off(bool Detach)
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
            if (Detach == 0)
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