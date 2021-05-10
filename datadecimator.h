#pragma once

#include "qcustomplot.h"
#include <stack>
#include <cmath>

struct DataDecimator
{
    using storage_t = QSharedPointer<QCPGraphDataContainer>;
    using const_iterator = storage_t::value_type::const_iterator;

    static storage_t douglas_peucker(const storage_t& storage, double d = 0);

};
