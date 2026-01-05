#include <iostream>

#include <Windows.h>
#include <TlHelp32.h>

namespace Scheduler
{

	// Processes

    // Helper to get process id from string.
    //TODO: Comment better.
    static DWORD get_process_id(const std::wstring& process_name)
    {
        DWORD process_id = 0; // Default

        // Create a snapshot of all open processes.
        HANDLE snap_shot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
        if (snap_shot == INVALID_HANDLE_VALUE) // If the snapshot is invalid, return early.
            return process_id;

        // RAII Wrapper that'll close our handle as soon as we go out of scope.
        auto close_handle = [](HANDLE h)
        {
            if (h != INVALID_HANDLE_VALUE)
                CloseHandle(h);
        };
        std::unique_ptr<void, decltype(close_handle)> handle_guard(snap_shot, close_handle);

        PROCESSENTRY32W entry = {};
        entry.dwSize = sizeof(decltype(entry));

        if (Process32FirstW(snap_shot, &entry) == TRUE)
        {
            do
            {
                // If the process id matches what we're searching for, set the id var.
                if (_wcsicmp(process_name.c_str(), entry.szExeFile) == 0)
                {
                    process_id = entry.th32ProcessID;
                    break;
                }
            } while (Process32NextW(snap_shot, &entry) == TRUE);
        }

        return process_id;
    }

    bool is_process_running(const std::wstring& process)
    {
        return get_process_id(process) != 0;
    }

	void run_process(const std::wstring& process)
	{
        /*
        STARTUPINFOW startup_info = {};
        PROCESS_INFORMATION process_information = {};
        startup_info.cb = sizeof(startup_info);

        CreateProcessW(process.c_str(), NULL, NULL, NULL, FALSE, 0, NULL, NULL, &startup_info, &process_information);
        */
	}

	void close_process(const std::wstring& process)
	{
        // Get the process id, and return early if it's not found.
        DWORD process_id = get_process_id(process);
        if (process_id == 0) return;

        // Open a handle to the process, and if it's invalid, return.
        HANDLE process_handle = OpenProcess(PROCESS_TERMINATE, FALSE, process_id);
        if (process_handle == NULL) return;

        // Terminate the process.
        TerminateProcess(process_handle, 0);
        CloseHandle(process_handle);

        std::wcout << "[Scheduler] Terminated process: " << process << std::endl;

        return;
	}

}