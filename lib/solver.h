#include <cstdint>
#include "constants.h"

struct Partial {
    uint32_t p, q;
    uint8_t permutations[32];
    uint8_t pos;
    uint32_t carry[32];
    uint64_t goal;
    Partial(uint64_t g) : p{1}, q{1}, pos{1}, permutations{0}, carry{0}, goal{g} {};
};

typedef struct {
    uint64_t p;
    uint64_t q;
} Solution;

Solution solve(uint64_t product);