#pragma once

#include <iostream>

namespace Scheduler
{
	// Processes
	bool is_process_running(const std::wstring& process);
	void run_process(const std::wstring& process);
	void close_process(const std::wstring& process);
}