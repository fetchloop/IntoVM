#include <iostream> // Input/Output.
#include <csignal> // Signal Handler.

// Headers.
#include "header/blacklist.h"
#include "header/spoofer.h"
#include "header/scheduler.h"

// Config.
#include "cfg/Config.h"

// Declare global tracking vectors.
std::vector<std::wstring> added_keys;
std::vector<std::wstring> started_processes;

// Forward Declaration.
void cleanup();
void signal_handler(int);

int main()
{

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
			Spoofer::add_regkey(key);
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
			
			Scheduler::run_process(process);
			started_processes.push_back(process);
		}
	}

	// Setup Cleanup Handler.
	signal(SIGINT, signal_handler);
	
	// KeepAlive
	std::cout << "IntoVM successfully set up.\nPress Ctrl+C to exit..." << std::endl;
	while (true)
	{
		Sleep(1000);
	}

	return 0;
}

void cleanup()
{
	// Remove added registry keys.
	for (const auto& key : added_keys)
	{
		// Remove the key.
		Spoofer::remove_regkey(key);
		std::wcout << "[Cleanup] " << "removed Registry Key '" << key << "'\n";
	}
	added_keys.clear(); // Clear the added keys array.


	for (const auto& process : started_processes)
	{
		Scheduler::close_process(process);
		std::wcout << "[Cleanup] " << "terminated process '" << process << "'\n";
	}
	started_processes.clear(); // Ditto.
}

void signal_handler(int signal)
{
	std::cout << "\n[IntoVM] Ctrl+C detected, cleaning up..." << std::endl;
	cleanup(); // Clean registry and processes before exiting.
	exit(0); // Graceful Exit.
}