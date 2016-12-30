#include <bitset>
#include "solver.h"
#include "bits.h"

// R_i = diagonal_multiply(state.p, state.q, i)
// c_i = (r_(i-1) >> 1) = carry(r_(i-1))
// r_i = R_i + (r_(i-1) >> 1) = R_i + c_i
// s_i = r_i & 1 = remainder(r_i)

inline uint64_t apply_permutation(Partial &state,
                              uint64_t R_i, uint64_t c_i, uint64_t s_i,  // x
                              uint8_t permute_index) {
    uint64_t x = R_i ^ c_i ^ s_i;
    state.p = PERMUTE_FUNCTIONS[(x&1)<<2 | permute_index<<1 | 0](state.p, state.pos);
    state.q = PERMUTE_FUNCTIONS[(x&1)<<2 | permute_index<<1 | 1](state.q, state.pos);
    return R_i + bit_at(state.p, state.pos) + bit_at(state.q, state.pos);
};

inline bool advance (Partial &state) {
    uint8_t permutation = state.permutations[state.pos];

    // 0. Don't advance if we're out of permutations at this index
    if (permutation > 1) { return false;}

    // 1. Increment permutation index for next advance
    state.permutations[state.pos]++;

    // 2. Calculate the new carry for this permutation.

    // when working on r_i, we're finding p_i, q_i
    // so we need to remove these factors from R_i (that is, p_i*q_0 and q_i*p_0)
    // and therefore drop the right-most digits (p_0, q_0) that they would be multiplied by.
    // Luckily, p_0 and q_0 are always 1 which lets us simply add the permutation to
    // finding the new carry.

    uint64_t R_i;
    if (state.pos > 1) {
        R_i = diagonal_multiply(state.p >> 1, state.q >> 1, (uint8_t) (state.pos - 2));
    } else {
        // TODO cleaner way to handle the edge case of no inner terms between p_i, p_0
        R_i = 0;
    }

    // carry(r_(i_1))
    // invariants: state.pos >= 1 and state.carry[0] is 0
    uint64_t c_i = state.carry[state.pos - 1];

    // get the goal's bit at this position
    uint64_t s_i = bit_at(state.goal, state.pos);

    // find this permutation's p_i, q_i and add them to R_i
    R_i = apply_permutation(state, R_i, c_i, s_i, permutation);
    // carry over all but s_i for the next iteration
    state.carry[state.pos] = carry(R_i + c_i);

    return true;
}

inline void reverse(Partial &state) {
    // TODO | be careful of overflow when pos (uint8_t) can wrap on bit length (p, q > 254 bits)
    // TODO | otherwise, we'll end up zeroing the first digit (p_0, q_0)
    state.p = zero_at(state.p, state.pos);
    state.q = zero_at(state.q, state.pos);
    state.permutations[state.pos] = 0;
    state.pos--;
}

Solution solve(uint64_t goal) {
    Partial state = {goal};
    uint64_t pq = 0;

    while (state.pos > 0) {
        if (!advance(state)) {
            reverse(state);
            continue;
        }

        pq = (uint64_t) state.p * (uint64_t) state.q;

        if (pq > goal) continue;  // Try next permutation, same position
        else if (pq < goal) state.pos++; // Move on to next digit
        else if (state.p != 1 && state.q != 1) return Solution{.p=state.p, .q=state.q};  // Non-trivial decomposition
    }
    return Solution{0, 0};
}

