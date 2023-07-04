#ifndef SAVERMATH_H
#define SAVERMATH_H

#include "saver.h"
#include <QString>
#include <QStringList>

class SaverMath : public ISaver
{
private:
    std::vector<std::string> buff;

public:
    SaverMath();
    ~SaverMath();

    void save(std::vector<std::string> buff = {}, std::string path = "") override;
    void load(std::string path = "") override;

    std::vector<std::string> getBuff() const { return this->buff; }
};

#endif // SAVERMATH_H
