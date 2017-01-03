#pragma once

#include <cstdint>

struct Partial {
    uint64_t p, q;
    uint8_t permutations[64];
    uint8_t pos;
    uint64_t carry[64];
    uint8_t goal[64];
    Partial(uint64_t goal) : p{1}, q{1}, pos{1}, permutations{0}, carry{0}
    {
        int i = 0;
        while(i <= 63) {
            this->goal[i] = (uint8_t) (goal & 1);
            goal >>= 1;
            i++;
        }
    };
};

struct Solution {
    uint64_t p;
    uint64_t q;
    uint64_t carry[64];
    Solution(uint64_t p, uint64_t q, uint64_t carry[64]) : p{p}, q{q}
    {
        for(int i=0; i<64; i++) this->carry[i]=carry[i];
    }

    Solution(uint64_t p, uint64_t q) : p{p}, q{q}
    {
        for(int i=0; i<64; i++) this->carry[i]=0;
    }
};

Solution solve(uint64_t product);
