#pragma once

#include <cstdint>
#include "reverse.h"


typedef uint64_t (*BitMaskFunction) (uint64_t buffer, uint8_t pos);


/// Set a bit in the buffer to 1;
/// \param buffer
/// \param pos index from right to set to 1
inline uint64_t one_at(uint64_t buffer, uint8_t pos) { return buffer | (1ull << pos); }


/// Set a bit in the buffer to 0;
/// \param buffer
/// \param pos index from right to set to 0
inline uint64_t zero_at(uint64_t buffer, uint8_t pos) { return buffer & ~(1ull << pos); }


/// 3bit index: xyz
///
/// Step 1: compute x
/// ┏━━━━━━━━━━━━━━━━━━━━━━┓
/// ┃  x= R_i ^ c_i ^ s_i  ┃
/// ┣━━━━━┳━━━━━┳━━━━━┳┳━━━┫
/// ┃ R_i ┃ c_i ┃ s_i ┃┃ x ┃
/// ┣━━━━━╋━━━━━╋━━━━━╋╋━━━┫
/// ┃  0  ┃  0  ┃  0  ┃┃ 0 ┃
/// ┃  0  ┃  0  ┃  1  ┃┃ 1 ┃
/// ┃  0  ┃  1  ┃  0  ┃┃ 1 ┃
/// ┃  0  ┃  1  ┃  1  ┃┃ 0 ┃
/// ┃  1  ┃  0  ┃  0  ┃┃ 1 ┃
/// ┃  1  ┃  0  ┃  1  ┃┃ 0 ┃
/// ┃  1  ┃  1  ┃  0  ┃┃ 0 ┃
/// ┃  1  ┃  1  ┃  1  ┃┃ 1 ┃
/// ┗━━━━━┻━━━━━┻━━━━━┻┻━━━┛
///
/// Step 2: shift x, y, z
/// y = permutation index (first or second)
/// z = factor index (p or q)
/// index = (x&1)<<2 | y<<1 | z
///
/// ┏━━━┳━━━┳━━━┳━━━┓
/// ┃ x ┃ y ┃ z ┃ F ┃
/// ┣━━━╋━━━╋━━━╋━━━┫
/// ┃ 0 ┃ 0 ┃ 0 ┃ 0 ┃
/// ┃ 0 ┃ 0 ┃ 1 ┃ 0 ┃
/// ┃ 0 ┃ 1 ┃ 0 ┃ 1 ┃
/// ┃ 0 ┃ 1 ┃ 1 ┃ 1 ┃
/// ┃ 1 ┃ 0 ┃ 0 ┃ 0 ┃
/// ┃ 1 ┃ 0 ┃ 1 ┃ 1 ┃
/// ┃ 1 ┃ 1 ┃ 0 ┃ 1 ┃
/// ┃ 1 ┃ 1 ┃ 1 ┃ 0 ┃
/// ┗━━━┻━━━┻━━━┻━━━┛
///
/// Step 3: apply function at index
/// PERMUTE_FUNCTIONS[(x&1)<<2 | y<<1 | 0](&p, pos)
/// PERMUTE_FUNCTIONS[(x&1)<<2 | y<<1 | 1](&q, pos)
const BitMaskFunction PERMUTE_FUNCTIONS[8] = {
                 //            x = R_i ^ c_i ^ s_i
        zero_at, // 000
        zero_at, // 001  (0,0) for x = 0 (adds 0 to R_i)

        one_at,  // 010
        one_at,  // 011  (1,1) for x = 0 (adds 2 to R_i)

        zero_at, // 100
        one_at,  // 101  (0,1) for x = 1 (adds 1 to R_i)

        one_at,  // 110
        zero_at  // 111  (1,0) for x = 1 (adds 1 to R_i)
};

/// for an index into PERMUTE_FUNCTIONS, use index>>1 for PERMUTE_OFFSET
/// so if index = [(x&1)<<2 | y<<1 | 1] then offset_index
const uint64_t PERMUTE_OFFSET[4] = {0, 2, 1, 1};


/// LSB -> MSB for an input that may be < 32 bits.
/// \param input number to reverse
/// \return the reversed int
/// \example 00..00010110111 -> 11101101000..00
inline uint64_t reverse(uint64_t input) {
    // TODO Use http://stackoverflow.com/a/24058332 when p, q > 64 bits
    return
            (BIT_REVERSE_TABLE[ input        & 0xffff] << 48) |
            (BIT_REVERSE_TABLE[(input >> 16) & 0xffff] << 32) |
            (BIT_REVERSE_TABLE[(input >> 32) & 0xffff] << 16) |
            (BIT_REVERSE_TABLE[(input >> 48) & 0xffff]        )
    ;
}


/// keep the leftmost [0:pos] bits
/// \example (1010100..00, 3) -> 00..001010
inline uint64_t keep_left(uint64_t x, uint8_t pos) { return x >> (63 - pos); }


/// Sum of the products of the diagonals in the first pos bits of two numbers.
/// \param p first partial
/// \param q second partial
/// \param pos number of bits used.
/// \return [0, pos] depending on input values.
/// \example (0b011101101, 0b101010110, 7) -> 2
///          note that the left most bit was discarded since only 8 are used.
inline uint8_t diagonal_multiply(uint64_t p, uint64_t q, uint8_t pos) {
    // sum(1 -> pos, p_i * q_(pos+1-i))
    // reverse bits in q, then and p, q together
    q = reverse(q);
    return __builtin_popcount(p & keep_left(q, pos));
}