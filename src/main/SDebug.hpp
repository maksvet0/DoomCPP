#pragma once

#include <string>
#include <vector>

class SDebug {
public:
    SDebug();
    ~SDebug();

    std::vector<char*> getValidationLayers();
    void info(const std::string& from, const std::string& message);
    void warn(const std::string& from, const std::string& message);
    void err(const std::string& from, const std::string& message);
    void ferr(const std::string& from, const std::string& message);

private:
    // something inner...
};
