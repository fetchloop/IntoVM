#include <iostream> // Input/Output.
#include <csignal> // Signal Handler.

// Sleep
#include <chrono>
#include <thread>

// Headers.
#include "header/blacklist.h"
#include "header/spoofer.h"
#include "header/scheduler.h"

// Config.
#include "cfg/Config.h"

// Declare global tracking vectors.
std::vector<std::wstring> added_keys;
std::vector<std::wstring> started_processes;

// Shell Execution ( Request Elevation ).
#include <shellapi.h>

// Forward Declaration.
void cleanup();
void signal_handler(int);
bool is_elevated();
bool request_elevation();

int main()
{
	// Simple running as admin check.
	if (!is_elevated())
	{
		std::cout << "[IntoVM] Not running as administrator. Requesting elevation..." << std::endl;

		if (request_elevation())
			return 0;  // Exit this instance, elevated one takes over
		else
		{
			std::cerr << "[IntoVM] Failed to elevate. Please run as administrator." << std::endl;
			return 1;
		}
	}
	std::cout << "IntoVM Starting..." << std::endl;

	// Read config values.
	Config cfg = read_config();

	if (cfg.spoof_registry_keys)
	{
		// Loop through blacklist::vm_keys.
		for (const auto& key : blacklist::vm_keys)
		{
			if (Spoofer::regkey_exists(key))
			{
				// Registry key already exists.
				std::wcout << "[IntoVM] " << "Skipping creation of registry key '" << key << "' as it already exists.\n"; // Log it
				continue; // Skip.
			}

			// Create registry key, and add it to the tracker.
			if(bool added = Spoofer::add_regkey(key); added)
				added_keys.push_back(key);
		}
	}

	if (cfg.spoof_processes)
	{
		// Loop through blacklist::vm_processes.
		for (const auto& process : blacklist::vm_processes)
		{

			// Check if process is already running.
			if (Scheduler::is_process_running(process))
			{
				std::wcout << "[IntoVM] " << "Skipping " << process << " as it's already running.\n"; // Log it
				continue; // Skip as it's already running.
			}
			
			if(bool started = Scheduler::run_process(process); started)
				started_processes.push_back(process);
		}
	}

	// Setup Cleanup Handler.
	signal(SIGINT, signal_handler);
	
	// KeepAlive
	std::cout << "IntoVM successfully set up.\nPress Ctrl+C to exit..." << std::endl;
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}

	return 0;
}

void cleanup()
{
	// Loop through all added registry keys and remove them
	for (const auto& key : added_keys)
		Spoofer::remove_regkey(key);

	added_keys.clear(); // Clear the vector.

	// Loop through all started processes and terminate them.
	for (const auto& process : started_processes)
		Scheduler::close_process(process);

	started_processes.clear(); // Clear the vector.
}

void signal_handler(int signal)
{
	std::cout << "\n[IntoVM] Ctrl+C detected, cleaning up..." << std::endl;
	cleanup();
	std::cout << "[IntoVM] Cleanup complete. Exiting in 10 seconds..." << std::endl;
	Sleep(10000);
	exit(0);
}

bool is_elevated()
{
	HANDLE token = NULL;

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &token))
		return false;

	TOKEN_ELEVATION elevation = {};
	DWORD size = sizeof(elevation);

	BOOL success = GetTokenInformation(token, TokenElevation, &elevation, sizeof(elevation), &size);

	CloseHandle(token);

	return success && elevation.TokenIsElevated;
}

bool request_elevation()
{
	wchar_t exe_path[MAX_PATH];

	if (GetModuleFileNameW(NULL, exe_path, MAX_PATH) == 0)
		return false;

	SHELLEXECUTEINFOW sei = {};
	sei.cbSize = sizeof(sei);
	sei.lpVerb = L"runas"; // Request elevation
	sei.lpFile = exe_path; // Path to the app's exe
	sei.lpParameters = nullptr;
	sei.nShow = SW_SHOWNORMAL;
	sei.fMask = SEE_MASK_NOCLOSEPROCESS;

	if (!ShellExecuteExW(&sei))
	{
		DWORD error = GetLastError();

		if (error == ERROR_CANCELLED)
			std::cerr << "[IntoVM] User declined UAC prompt." << std::endl;
		else
			std::cerr << "[IntoVM] ShellExecuteEx failed with error: " << error << std::endl;

		return false;
	}

	return true;
}