#include "RandomStringGenerator.h"

QString RandomStringGenerator::generate(int length)
{
    QString s;
    s.resize(length);

    for(int i=0; i < length; ++i)
        s[i] = QChar(rand() % 57 + 65);

    return s;
}

QString RandomStringGenerator::generateFilename(int length)
{
    QString s;
    s.resize(length);

    for(int i=0; i < length; ++i)
    {
        if( rand() % 2)
            s[i] = QChar(rand()%26 + 'a');
        else
            s[i] = QChar(rand()%26 + 'A');
    }

    return s;
}
