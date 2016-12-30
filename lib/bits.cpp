#include <stdint.h>
#include "bits.h"

// http://stackoverflow.com/a/746203
uint64_t reverse(uint64_t input) {
    // TODO Use http://stackoverflow.com/a/24058332
    uint64_t c =
            (BIT_REVERSE_TABLE[ input        & 0xff] << 24) |
            (BIT_REVERSE_TABLE[(input >>  8) & 0xff] << 16) |
            (BIT_REVERSE_TABLE[(input >> 16) & 0xff] <<  8) |
            (BIT_REVERSE_TABLE[(input >> 24) & 0xff]);
    return c;
}


uint8_t diagonal_multiply(uint64_t p, uint64_t q, uint8_t pos) {
    // TODO | bake this into a lookup table.
    // TODO | key will probably be (pos,p,q) where p,q are uint16_t
    // sum(1 -> pos, p_i * q_(pos+1-i))
    // reverse bits in q, then and p, q together
    q = reverse(q);
    return __builtin_popcount(p & keep_left(q, pos) & mask(pos));
}

const BitMaskFunction PERMUTE_FUNCTIONS[] = {
    zero_at, // 000
    zero_at, // 001
    one_at,  // 010
    one_at,  // 011
    zero_at, // 100
    one_at,  // 101
    one_at,  // 110
    zero_at  // 111
};