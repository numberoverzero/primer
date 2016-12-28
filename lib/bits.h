#include <cstdint>
#include "constants.h"
#include "lookups.h"


/// create a mask to drop leftmost [0:pos] bits
/// \example mask(3) -> 00..001111
inline uint32_t mask(uint8_t pos) { return ALL_ONE >> (31 - pos); }

/// keep the leftmost [0:pos] bits
/// \example (1010100..00, 3) -> 00..001010
inline uint32_t keep_left(uint32_t x, uint8_t pos) { return x >> (31 - pos); }

/// returns all but the LSB from x (equiv. x >> 1)
/// note that  x != carry(x) + remainder(x)
/// but rather x == carry(x) << 1 + remainder(x)
/// \example 00..011101101101 -> 00..001110110110
inline uint32_t carry(uint32_t x) { return x >> 1; }

/// returns the LSB from x (equiv x & 1)
/// \example 00..0111001101101 -> 00..0000000000001
inline uint32_t remainder(uint32_t x) { return x & 1; }



typedef uint32_t (*BitMaskFunction) (uint32_t &buffer, uint8_t pos);

/// Set a bit in the buffer to 1;
/// \param buffer
/// \param pos index from right to set to 1
inline uint32_t one_at(uint32_t &buffer, uint8_t pos) { buffer |= ONE_MASKS[pos]; return 1;}
/// Set a bit in the buffer to 0;
/// \param buffer
/// \param pos index from right to set to 0
inline uint32_t zero_at(uint32_t &buffer, uint8_t pos) { buffer &= ZERO_MASKS[pos]; return 0;}


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
extern const BitMaskFunction PERMUTE_FUNCTIONS[];

inline uint32_t apply_permutation(uint32_t x, uint8_t y, uint32_t &p, uint32_t &q, uint8_t pos) {
    return
        PERMUTE_FUNCTIONS[(x&1)<<2 | y<<1 | 0](p, pos) +
        PERMUTE_FUNCTIONS[(x&1)<<2 | y<<1 | 1](q, pos);
};

/// Return the bit in the given position
/// \example (0b00..001101010000, 8) -> 1
inline uint32_t bit_at(uint32_t x, uint8_t pos) { return (x >> pos) & 1; }


/// LSB -> MSB for an input that may be < 32 bits.
/// \param input number to reverse
/// \return the reversed int
/// \example 00..00010110111 -> 11101101000..00
uint32_t reverse(uint32_t input);
/// Sum of the products of the diagonals in the first pos bits of two numbers.
/// \param p first partial
/// \param q second partial
/// \param pos number of bits used.
/// \return [0, pos] depending on input values.
/// \example (0b011101101, 0b101010110, 7) -> 2
///          note that the left most bit was discarded since only 8 are used.
uint8_t diagonal_multiply(uint32_t p, uint32_t q, uint8_t pos);
