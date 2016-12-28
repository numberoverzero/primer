#include "solver.h"
#include "bits.h"


// R_i = diagonal_multiply(state.p, state.q, i)
// c_i = (r_(i-1) >> 1) = carry(r_(i-1))
// r_i = R_i + (r_(i-1) >> 1) = R_i + c_i
// s_i = r_i & 1 = remainder(r_i)

inline bool advance (Partial &state) {
    // 0. Don't advance if we're out of permutations at this index
    if (state.permutations[state.pos] > 1) { return false;}

    // 1. Grab the next permutation index for this position, then increment for next advance
    uint8_t permutation = state.permutations[state.pos];
    state.permutations[state.pos]++;

    // 2. Calculate the new carry for this permutation.

    // when working on r_i, we're finding p_i, q_i
    // so we need to remove these factors from R_i (that is, p_i*q_0 and q_i*p_0)
    // and therefore drop the right-most digits (p_0, q_0) that they would be multiplied by.
    // Luckily, p_0 and q_0 are always 1 which lets us simply add the permutation to
    // finding the new carry.

    // TODO is this supposed to be state.pos - 2 or state.pos - 1?
    uint32_t R_i = diagonal_multiply(state.p >> 1, state.q >> 1, (uint8_t) (state.pos - 2));

    // carry(r_(i_1))
    // invariants: state.pos >= 1 and state.carry[0] is 0
    uint32_t c_i = state.carry[state.pos - 1];

    // get the goal's bit at this position
    uint32_t s_i = bit_at(state.goal, state.pos);

    // find this permutation's p_i, q_i and add them to R_i
    R_i += apply_permutation(R_i ^ c_i ^ s_i, permutation, state.p, state.q, state.pos);
    // carry over all but s_i for the next iteration
    state.carry[state.pos] = carry(R_i + c_i);

    // 3. Move to the next digit, clearing any previous permutations.
    state.pq = state.p * state.q;
    state.pos++;
    state.permutations[state.pos] = 0;
}

inline bool check(Partial &state) {
    return state.pq == state.goal && state.p != 1 and state.q != 1;
}

Solution solve(uint32_t goal) {
    Partial state = {goal};
    // Force an exit as soon as the solver falls back to the first digit
    // there's only one option for p_0, q_0
    state.permutations[0] = 2;


    while (state.pos < 32) {
        // TODO advance can fail because permutations are exhausted.
        // TODO handle that here
        if (!advance(state)) {
            state.pos--;
            continue;
        }
        if (check(state)) return Solution{.p=state.p, .q=state.q};
    }
    return Solution{0, 0};
}

