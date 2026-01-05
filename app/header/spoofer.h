#pragma once

#include <string>

namespace spoofer
{
	// Registry Keys
	bool regkey_exists(std::string name);
	void add_regkey(std::string key_name);
	void remove_regkey(std::string key_name);
}