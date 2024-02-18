#ifndef QVRHDGTABLE_H
#define QVRHDGTABLE_H

#include <jibbs/vector/vector3.h>
#include <vector>

#define QVRHDG(hdg,Z) QVRHdgTable::get().qvr(hdg,Z);

class QVRHdgTable
{
public:

    static QVRHdgTable& get();

    Vector3F qvr(int hdg, float Z);

private:
    QVRHdgTable();
    static const float s_Factor;
    std::vector<Vector3F> _qvrTable;
};

#endif // QVRHDGTABLE_H
