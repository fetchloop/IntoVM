#include "header/spoofer.h"

#include "Windows.h"
#include <iostream>
#include <string>

using std::wcout;

namespace Spoofer
{
	// Registry Keys
	bool regkey_exists(const std::wstring& key_path)
	{
		HKEY hkey{};

		// RegOpenKeyEx writes the handle to hkey via the pointer &hkey
		LSTATUS result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, key_path.c_str(), 0, KEY_READ, &hkey);

		// Return without closing handle if the key doesn't exist.
		if (result != ERROR_SUCCESS)
			return false;
		
		// Valid registry key, so close the handle.
		RegCloseKey(hkey);
		return true;
	}

	bool add_regkey(const std::wstring& key_path)
	{
		// Attempt to create the registry key of "key_path" parameter.
		HKEY hkey{};
		LSTATUS result = RegCreateKeyEx(HKEY_LOCAL_MACHINE, key_path.c_str(), 0, nullptr, 0, KEY_WRITE, nullptr, &hkey, nullptr);
		
		// Log and close key handle if success.
		if (result == ERROR_SUCCESS)
		{
			wcout << L"[Spoofer] Created Registry Key: " << key_path << std::endl;
			RegCloseKey(hkey);
			return true;
		}

		wcout << L"[Spoofer] Failed to create registry key '" << key_path << "', error: " << result << std::endl;
		return false;
	}

    bool remove_regkey(const std::wstring& key_path)
    {
		// Attempt deleting the specific registry key.
		LSTATUS result = RegDeleteKeyW(HKEY_LOCAL_MACHINE, key_path.c_str());

		// Log if success.
		if (result == ERROR_SUCCESS)
		{
			wcout << L"[Spoofer] Deleted Registry Key: " << key_path << std::endl;
			return true;
		}

		wcout << L"[Spoofer] Failed to delete registry key '" << key_path << "', error: " << result << std::endl;
		return false;
    }

}