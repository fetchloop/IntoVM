#include <iostream>

#include <Windows.h>
#include <TlHelp32.h>
#include <string>

namespace Scheduler
{
    // Helper to get process id from string.
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

        // Store process information of snapshot.
        PROCESSENTRY32W entry = {};
        entry.dwSize = sizeof(decltype(entry));

        if (Process32FirstW(snap_shot, &entry) == TRUE) // First process in snapshot
        {
            // Iterate all processes in the snapshot.
            do
            {
                // Check if the process name matches the query.
                if (_wcsicmp(process_name.c_str(), entry.szExeFile) == 0)
                {
                    process_id = entry.th32ProcessID;
                    break;
                }
            } while (Process32NextW(snap_shot, &entry) == TRUE);
        }

        return process_id;
    }

    // Returns whether a process of x name is currently running or not.
    bool is_process_running(const std::wstring& process)
    {
        return get_process_id(process) != 0;
    }

    void run_process(const std::wstring& process)
    {
        // Get the directory where IntoVM is located.
        wchar_t exe_path[MAX_PATH];
        GetModuleFileNameW(NULL, exe_path, MAX_PATH);

        // Remove the executable name to get just the directory.
        std::wstring exe_dir = exe_path;
        size_t last_slash = exe_dir.find_last_of(L"\\/");
        if (last_slash != std::wstring::npos)
            exe_dir = exe_dir.substr(0, last_slash + 1);

        // Build full path to the process.
        std::wstring full_path = exe_dir + L"processes\\" + process;

        // Build command line with parent PID.
        std::wstring cmdLine = full_path + L" " + std::to_wstring(GetCurrentProcessId());

        STARTUPINFOW si = {};
        PROCESS_INFORMATION pi = {};
        si.cb = sizeof(si);

        CreateProcessW(NULL, cmdLine.data(), NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);

        // Clean up handles.
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        std::wcout << L"[Scheduler] Started process: " << process << std::endl;
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