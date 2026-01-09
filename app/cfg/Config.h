#include <iostream>

#include <string>
#include <fstream>
#include <sstream>

struct Config
{
    bool spoof_registry_keys = false;
    bool spoof_processes = true;
};

bool parse_bool_from_key(const std::string& content, const std::string& key)
{
    size_t pos_reg = content.find("\""+key+"\"");
    if (pos_reg != std::string::npos)
    {
        // Find the colon ":" after key.
        size_t c_pos = content.find(":", pos_reg);

        // Quote '"' start and end.
        size_t q_start = content.find("\"", c_pos);
        size_t q_end = content.find("\"", q_start + 1);

        // Extract string between the quotes.
        std::string value = content.substr(q_start + 1, q_end - q_start - 1);

        // Bool conversion
        return value == "true";
    }

    return false;
}

Config read_config()
{

    Config config {};  // Default Initializer

    // Open config file
    std::ifstream file("config.json");

    if (!file.is_open())
    {
        std::cout << "[Config] config.json not found, using defaults: " <<
            "'spoof_registry_keys': " << config.spoof_registry_keys << "\n" <<
            "'spoof_processes': " << config.spoof_processes << std::endl;
        return config;
    }

    // Read file into a string.
    std::stringstream buf;
    buf << file.rdbuf();
    std::string content = buf.str();

    // Close file as we have extracted what we want.
    file.close();

    // Locate config values from string.

    // Get the values from config.json using the helper method.
    config.spoof_registry_keys = parse_bool_from_key(content, "spoof_registry_keys");
    config.spoof_processes = parse_bool_from_key(content, "spoof_processes");

    return config;
}