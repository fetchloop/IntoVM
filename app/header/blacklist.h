#pragma once

#include <Windows.h>
#include <vector>
#include <string>

namespace blacklist
{

	// Registry Keys to spoof or add
    inline const std::vector<std::wstring> vm_keys =
    {
        L"SOFTWARE\\VMware, Inc.\\VMware Tools",
        L"SOFTWARE\\Oracle\\VirtualBox Guest Additions",
        L"SYSTEM\\ControlSet001\\Services\\VBoxGuest",
        L"SYSTEM\\ControlSet001\\Services\\VBoxMouse",
        L"SYSTEM\\ControlSet001\\Services\\VBoxService",
        L"SYSTEM\\ControlSet001\\Services\\VBoxSF",
        L"SYSTEM\\ControlSet001\\Services\\VBoxVideo",
        L"SYSTEM\\ControlSet001\\Services\\vmci",
        L"SYSTEM\\ControlSet001\\Services\\vmhgfs",
        L"SYSTEM\\ControlSet001\\Services\\vmmouse",
        L"SYSTEM\\ControlSet001\\Services\\vmusb",
        L"SYSTEM\\ControlSet001\\Services\\vpci",
        L"HARDWARE\\ACPI\\DSDT\\VBOX__", // VirtualBox ACPI
        L"HARDWARE\\ACPI\\FADT\\VBOX__",
        L"HARDWARE\\ACPI\\RSDT\\VBOX__"
    };

	// Services to fake
	inline const std::vector<std::wstring> vm_processes=
    {
        L"vmtoolsd.exe",      // VMware Tools
        L"vmwaretray.exe",
        L"vmwareuser.exe",
        L"vmacthlp.exe",

        L"vboxservice.exe",   // VirtualBox
        L"vboxtray.exe",

        L"xenservice.exe",    // Xen

        L"qemu-ga.exe",       // QEMU Guest Agent
	};

}