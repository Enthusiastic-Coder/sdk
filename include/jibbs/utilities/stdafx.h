
#ifndef __STD_AFX_H__
#define __STD_AFX_H__
#pragma once

#include <QString>

inline QString ResourcePath(const QString& path)
{
#ifdef Q_OS_ANDROID
    return "assets:/" + path;
#endif
    return path;
}


#include <QTimer>
#include <QObjectList>
#include <QDebug>

inline void shutDownAllTimers(QObject* obj)
{
    const QObjectList& list = obj->children();
    for(QObject* o : list)
    {
        QTimer* t = qobject_cast<QTimer*>(o);
        if( t != nullptr )
        {
            if( t->isActive())
            {
#ifdef Q_OS_WIN
                qDebug() << "Stopped Timer : " << t->objectName();
#endif
                t->stop();
            }
        }
    }
}

#include <string>
#include <fstream>
#include <sstream>
#include <QFile>
#include <QString>
namespace BuildVersion
{
    const QString BuildDateTime = "BuildDateTime";
}

inline QString readAll(const QString& sFilename)
{
    QFile file(sFilename);
    if( !file.open(QIODevice::ReadOnly))
        return QLatin1String("");

    return file.readAll();
}
inline std::string readAllRP(const QString& sFilename)
{
    return readAll(ResourcePath(sFilename)).toStdString();
}
inline bool readAll(std::string sFilename, std::string &text)
{
	std::ifstream inputFile;

	inputFile.open(sFilename, std::ifstream::in);

	if (inputFile.is_open() == false)
		return false;

    std::stringstream ss;

    ss << inputFile.rdbuf();
    text = ss.str();
	return true;
}

template < class ContainerT >
void StringTokens(const std::string& str, ContainerT& tokens,
	const std::string& delimiters = " ", bool trimEmpty = false)
{
	std::string::size_type pos, lastPos = 0;

	using value_type = typename ContainerT::value_type;
	using size_type = typename ContainerT::size_type;

	while (true)
	{
		pos = str.find_first_of(delimiters, lastPos);

		if (pos == std::string::npos)
		{
			pos = str.length();

			if (pos != lastPos || !trimEmpty)
				tokens.push_back(value_type(str.data() + lastPos, (size_type)pos - lastPos));

			break;
		}
		else
		{
			if (pos != lastPos || !trimEmpty)
				tokens.push_back(value_type(str.data() + lastPos, (size_type)pos - lastPos));
		}

		lastPos = pos + 1;
	}
}


//#define TEST_FOR_MEMORY_LEAKS

#ifdef TEST_FOR_MEMORY_LEAKS
#define _CRTDBG_MAP_ALLOC_NEW
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#endif


#endif
