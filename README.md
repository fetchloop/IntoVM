# 💻 IntoVM

**A defensive cybersecurity tool that turns malware evasion tactics against themselves**

## 🎯 What is IntoVM?

Modern malware is designed to detect when it's being analyzed in virtual machines or sandboxes, and when it detects these environments, it refuses to run. IntoVM flips this defensive behavior into a protection mechanism by making your system look like a VM or sandbox to malware.
Think of it like placing a scarecrow in a field. The birds don't fly away because someone is actively trying to harm them, simply due to the insinuation that something looks like it might. Similarly, IntoVM adds indicators that malware looks for, causing it to terminate before it can cause harm.

## 🛡️ How It Works

IntoVM creates a deception layer on Windows systems by:

1. **Registry Spoofing**: Adding fake VM-related registry keys that malware checks for (VMware Tools, VirtualBox Guest Additions, etc.)
2. **Process Spoofing**: Running decoy processes that mimic VM guest tools (vmtoolsd.exe, vboxservice.exe, etc.)

When malware performs its environment checks, it encounters these indicators and concludes it's running in an analysis environment, triggering its protection logic to terminate.

## ⚠️ Important Warnings

- **Requires Administrator privileges** : modifies system registry and manages processes
- **Defensive use only** : designed to protect systems, not for malicious purposes
- **May interfere with legitimate software** : some applications detect VMs for licensing
- **Cleanup on exit** : automatically removes all modifications when terminated

## 📋 Requirements
- Windows 10/11
- Administrator privileges

## 🔨 Building from Source (optional)
- Visual Studio 2019+ or compatible C++ compiler
- Windows SDK

## 🚀 Usage

**Running:**

Run as administrator (will request elevation if needed)

The application will:
1. Check configuration in `config.json`
2. Add specified registry keys
3. Start decoy processes
4. Run until terminated (Ctrl+C)
5. Automatically clean up all modifications on exit

## ⚙️ Configuration

Edit `config.json` to control behavior:

```json
{
  "spoof_registry_keys": "true",
  "spoof_processes": "true"
}
```

**Customizing Detection Signatures:**
- `blacklist.h` contains lists of registry keys and processes to spoof
- Add or remove items based on what you want/need.

## 🔧 Architecture

**Core Components:**
- **Spoofer**: Manages registry key creation and deletion
- **Scheduler**: Handles process lifecycle (start, monitor, terminate)
- **Config**: Parses settings from JSON configuration
- **Blacklist**: Defines VM indicators to spoof

## ✨ Key Features

**🔄 Fully Reversible**
All modifications are temporary and automatically undone when IntoVM closes. Your system returns to its original state with zero traces left behind.

**🛡️ Safe & Non-Destructive**
Doesn't modify existing system files or interfere with legitimate software. Only adds temporary decoy indicators that malware looks for.

**📊 Transparent Operation**
Detailed logging shows exactly what's being added, modified, or removed, no hidden actions or mystery changes to your system.

**⚡ Instant Protection**
Protection activates immediately on startup and remains active until you choose to exit. No complex configuration or ongoing maintenance required.

**🔒 Clean Exit Guaranteed**
Even if Windows shuts down unexpectedly or you force-close the application, built-in cleanup ensures all modifications are removed automatically.

**⚙️ Lightweight & Efficient**
Runs quietly in the background using minimal system resources, you won't notice any performance impact during normal use.

<br>
<small><i>Yes this description was written using an LLM, but none of the code has been. Vibe coding is lame, and I do not support it.</i></small>
