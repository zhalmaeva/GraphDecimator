#pragma once

#include "qcustomplot.h"
#include <stack>
#include <cmath>

struct DataDecimator
{
    using storage_t = QSharedPointer<QCPGraphDataContainer>;
    using const_iterator = storage_t::value_type::const_iterator;

    static storage_t douglas_peucker(const storage_t& storage, double d = 0);
    static storage_t test(const storage_t& storage, double d = 0)
    {
        QMessageBox mb;
        mb.show();
        mb.exec();
        return storage_t{ new storage_t::value_type };
    }

};

//              HOWTO
//  To add ur alg u must add it here with this proto
//      @return storage_t           - decimated data
//      @param1 const storage_t&    - input data
//      @param2 double              - u must add it for compatibility
//
//  Also u must add this alg in MainWindow::func        !!!
//  Make Fun!
