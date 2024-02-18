#include <jibbs/math/MathSupport.h>
#include <jibbs/math/Quarternion.h>

#include "QVRHdgTable.h"

const float QVRHdgTable::s_Factor = 100.0f;

QVRHdgTable &QVRHdgTable::get()
{
    static QVRHdgTable table;
    return table;
}

Vector3F QVRHdgTable::qvr(int hdg, float Z)
{
    return _qvrTable[MathSupport<int>::normAng(hdg)] * Z / s_Factor;
}

QVRHdgTable::QVRHdgTable()
{
    for( int i=0; i <= 360; ++i)
        _qvrTable.push_back(QVRotate(MathSupport<float>::MakeQHeading(i), Vector3F(0,0,-s_Factor)));
}
