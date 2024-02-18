#ifndef RANDOMSTRINGGENERATOR_H
#define RANDOMSTRINGGENERATOR_H

#include <QString>

class RandomStringGenerator
{
public:
    static QString generate(int length);
    static QString generateFilename(int length);
};

#endif // RANDOMSTRINGGENERATOR_H
