#pragma once

#include <string>

namespace Spoofer
{
	// Registry Keys
	bool regkey_exists(const std::wstring& name);
	bool add_regkey(const std::wstring& key_name);
	bool remove_regkey(const std::wstring& key_name);
}