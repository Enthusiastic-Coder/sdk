#include <jibbs/math/qhdgtable.h>
#include <jibbs/math/MathSupport.h>

QHdgTable &QHdgTable::get()
{
    static QHdgTable table;
    return table;
}

const QuarternionF &QHdgTable::Hdg(int hdg)
{
    return _qTable[MathSupport<int>::normAng(hdg)];
}

QHdgTable::QHdgTable()
{
    for( int i=0; i <= 360; ++i)
        _qTable.push_back(MathSupport<float>::MakeQHeading(i));
}
