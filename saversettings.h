#ifndef SAVERSETTINGS_H
#define SAVERSETTINGS_H

#include "saver.h"
#include <map>

struct Config
{
    int precision;
    bool spacingAfterEqual;
    int fontSize;
    int letterSpacing;
};

class SaverSettings : ISaver
{
private:
    Config config;

public:
    SaverSettings();
    ~SaverSettings();

    void save(std::vector<std::string> buff = {}, std::string path = "") override;
    void load(std::string path = "") override;

    Config getConfig() const { return this->config; };
    void setConfig(Config config) { this->config = config; }
};

#endif // SAVERSETTINGS_H
