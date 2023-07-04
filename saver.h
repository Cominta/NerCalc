#ifndef SAVER_H
#define SAVER_H

#include <iostream>
#include <fstream>
#include <ostream>
#include <vector>
#include <exception>

class CantOpenFile : public std::exception
{
public:
    const char* what() const noexcept override { return "Can't open file"; }
};

class ISaver
{
public:
    virtual void save(std::vector<std::string> buff = {}, std::string path = "") = 0;
    virtual void load(std::string path = "") = 0;
};

#endif // SAVER_H
