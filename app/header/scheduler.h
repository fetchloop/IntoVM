#pragma once

#include <string> // wstring safety.

namespace Scheduler
{
	// Processes
	bool is_process_running(const std::wstring& process);
	bool run_process(const std::wstring& process);
	bool close_process(const std::wstring& process);
}