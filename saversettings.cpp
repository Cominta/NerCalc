#include "saversettings.h"

SaverSettings::SaverSettings()
{

}

SaverSettings::~SaverSettings()
{

}

void SaverSettings::save(std::vector<std::string> buff, std::string path)
{
    std::ofstream stream;
    stream.open(path, std::ios_base::binary);

    if (!stream.is_open())
    {
        throw CantOpenFile();
    }

    stream.write((char*)&this->config, sizeof(Config));
    stream.close();
}

void SaverSettings::load(std::string path)
{
    std::ifstream stream;
    stream.open(path, std::ios_base::binary);

    if (!stream.is_open())
    {
        throw CantOpenFile();
    }

    stream.read((char*)&this->config, sizeof(Config));
    stream.close();
}
