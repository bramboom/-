#include "datadecimator.h"

DataDecimator::storage_t DataDecimator::douglas_peucker(const DataDecimator::storage_t& storage, double d)
{
    storage_t dec_storage(new QCPGraphDataContainer);
    if(!storage->isEmpty()) {
        dec_storage->add(*storage->begin());
        std::stack<std::pair<const_iterator, const_iterator>> stack;
        stack.push({ storage->constBegin(), storage->constEnd() });
        while(!stack.empty()) {
            auto range = stack.top();
            auto last = std::prev(range.second);
            double k1 = last->value - range.first->value;
            double k2 = last->key - range.first->key;
            double k3 = last->key * range.first->value - range.first->key * last->value;
            double maxDist = 0;
            const_iterator maxIt;
            for(auto it = std::next(range.first); it < last; ++it) {
                auto dist = std::abs(k1*it->key - k2*it->value + k3);
                if(dist > maxDist) {
                    maxDist = dist;
                    maxIt = it;
                }
            }
            stack.pop();
            if(maxDist / std::sqrt(k1*k1 + k2*k2) >= d) {
                stack.push({ maxIt, range.second });
                dec_storage->add(*maxIt);
                stack.push({ range.first, std::next(maxIt) });
            }
        }
        dec_storage->add(*std::prev(storage->end()));
    }
    return dec_storage;
}
