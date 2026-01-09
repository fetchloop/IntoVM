#pragma once

#include <string>

namespace Spoofer
{
	// Registry Keys
	bool regkey_exists(const std::wstring& name);
	void add_regkey(const std::wstring& key_name);
	void remove_regkey(const std::wstring& key_name);
}