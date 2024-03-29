#include "solver.h"
#include "bits.h"

// R_i = diagonal_multiply(state.p >> 1, state.q >> 1, i - 2)
// c_i = (r_(i-1) >> 1) = carry(r_(i-1))
// r_i = R_i + (r_(i-1) >> 1) = R_i + c_i
// s_i = r_i & 1 = remainder(r_i)

inline uint64_t apply_permutation(Partial &state,
                              uint64_t c_i, uint8_t s_i,  // x
                              uint8_t permute_index) {
    uint64_t R_i = diagonal_multiply(state.p >> 1, state.q >> 1, (uint8_t) (state.pos - 2));
    uint8_t x = (uint8_t) ((R_i ^ c_i ^ s_i) & 1);
    uint8_t index = (uint8_t) ((x & 1) << 2 | permute_index << 1);
    // TODO | this should not need to lookup a function, just set to 0 or 1 based on index
    // TODO | see https://graphics.stanford.edu/~seander/bithacks.html#ConditionalSetOrClearBitsWithoutBranching
    state.p = PERMUTE_FUNCTIONS[index](state.p, state.pos);
    state.q = PERMUTE_FUNCTIONS[index + 1](state.q, state.pos);
    return R_i + PERMUTE_OFFSET[index>>1];
};

inline bool advance (Partial &state) {
    uint8_t permutation = state.permutations[state.pos]++;
    if (permutation > 1) { return false;}

    uint64_t c_i = state.carry[state.pos - 1];
    uint8_t s_i = state.goal[state.pos];
    uint64_t r_i = apply_permutation(state, c_i, s_i, permutation);
    state.carry[state.pos] = (r_i + c_i) >> 1;
    return true;
}

inline void pop(Partial &state) {
    // TODO | be careful of overflow when pos (uint8_t) can wrap on bit length (p, q > 254 bits)
    // TODO | otherwise, we'll end up zeroing the first digit (p_0, q_0)
    state.p &= ~(1ull << state.pos);
    state.q &= ~(1ull << state.pos);
    state.permutations[state.pos--] = 0;
}

Solution solve(uint64_t goal) {
    Partial state = {goal};
    uint64_t pq = 0;
    while ((pq != goal) || state.p == 1 || state.q == 1) {
        if (advance(state)) {
            pq = state.p * state.q;
            state.pos += pq < goal;
        } else pop(state);
    }
    return Solution{.p=state.p, .q=state.q, .carry=state.carry};
}
