#ifndef SINTABLE_H
#define SINTABLE_H

#include <vector>
#include <jibbs/math/MathSupport.h>
#include <cmath>

class SinTable
{
    SinTable()
    {
        if( _sinTable.size() == 0)
        {
            _sinTable.resize(361);
            _cosTable.resize(361);
            for (int i = 0; i <= 360; i++)
            {
                float fRad = DegreesToRadians(static_cast<float>(i));

                _sinTable[i] = std::sin(fRad);
                _cosTable[i] = std::cos(fRad);
            }
        }
    }

public:

    static const SinTable& get()
    {
        static SinTable table;
        return table;
    }

    template<typename T>
    float sin(T angle) const
    {
        return _sinTable[MathSupport<T>::normAng(angle)];
    }

    template<typename T>
    float cos(T angle) const
    {
        return _cosTable[MathSupport<T>::normAng(angle)];
    }

private:
    std::vector<float> _sinTable;
    std::vector<float> _cosTable;
};

#endif // SINTABLE_H
