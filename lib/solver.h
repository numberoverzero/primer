#include <cstdint>
#include "constants.h"

struct Partial {
    uint32_t p, q, pq;
    uint8_t permutations[32];
    uint8_t pos;
    uint32_t carry[32];
    uint32_t goal;
    Partial(uint32_t g) : p{1}, q{1}, pq{1}, pos{1}, permutations{0}, carry{0}, goal{g} {};
};

typedef struct {
    uint32_t p;
    uint32_t q;
} Solution;

Solution solve(uint32_t product);