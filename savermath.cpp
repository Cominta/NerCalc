#include "savermath.h"

SaverMath::SaverMath()
{

}

SaverMath::~SaverMath()
{

}

void SaverMath::save(std::vector<std::string> buff, std::string path)
{
    std::ofstream stream;
    stream.open(path);

    if (!stream.is_open())
    {
        throw CantOpenFile();
    }

    for (auto str : buff)
    {
        stream << str << "\n";
    }

    stream.close();
}

void SaverMath::load(std::string path)
{
    this->buff.clear();

    std::fstream stream;
    stream.open(path);

    if (!stream.is_open())
    {
        throw CantOpenFile();
    }

    std::string str;

    while (std::getline(stream, str))
    {
        this->buff.push_back(str);
    }

    stream.close();
}
