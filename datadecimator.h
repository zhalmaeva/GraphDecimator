#pragma once
#include "point.h"
#include <stack>
#include <cmath>

template <class Container>
struct DataDecimator
{
    using value_type        = typename Container::value_type;
    using storage_t         = Container;
    using iterator          = typename storage_t::iterator;
    using const_iterator    = typename storage_t::const_iterator;


    static storage_t douglas_peucker(const storage_t& storage, double d)
    {
        if(d == 0) {
            return storage;
        }
        if(storage.empty()) {
            return {};
        }
        storage_t dec_storage;
        dec_storage.push_back(storage.front());
        std::stack<std::pair<const_iterator, const_iterator>> q;
        q.push({ static_cast<const_iterator>(storage.begin()), static_cast<const_iterator>(storage.end()) });
        while(!q.empty()) {
            auto range = q.top();
            auto last = std::prev(range.second);
            double k[] = {
                last->y - range.first->y,
                last->x - range.first->x,
                last->x * range.first->y - range.first->x * last->y
            };
            double maxDist = 0;
            const_iterator maxIt;
            for(auto it = std::next(range.first); it < last; ++it) {
                auto dist = std::abs(k[0]*it->x - k[1]*it->y + k[2]);
                if(dist > maxDist) {
                    maxDist = dist;
                    maxIt = it;
                }
            }
            q.pop();
            if(maxDist / std::sqrt(k[0]*k[0] + k[1]*k[1]) >= d) {
                q.push({ maxIt, range.second });
                dec_storage.push_back(*maxIt);
                q.push({ range.first, std::next(maxIt) });
            }
        }
        dec_storage.push_back(storage.back());
    return dec_storage;
    }
};
