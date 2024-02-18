#ifndef QHDGTABLE_H
#define QHDGTABLE_H

#include <jibbs/math/Quarternion.h>
#include <vector>

#define QHDG(hdg) QHdgTable::get().Hdg(hdg)

class QHdgTable
{
public:

    static QHdgTable& get();

    const QuarternionF& Hdg(int hdg);

private:
    QHdgTable();

    std::vector<QuarternionF> _qTable;
};

#endif // QHDGTABLE_H
