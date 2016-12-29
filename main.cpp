#include <bitset>
#include <solver.h>
#include <debugging.h>


enum TEST {
    VERY_FAST, FAST, SLOW, VERY_SLOW, DIFFERENT_LENGTHS
};

int main() {
    uint64_t p, q;
    TEST test = VERY_FAST;
    switch(test) {
        case VERY_FAST:  // very fast
            p = 17209;
            q = 21227;
            break;
        case FAST:  // fast
            p = 1000211;
            q = 1000121;
            break;
        case SLOW:  // slow
            p = 4346821;
            q = 4346831;
            break;
        case VERY_SLOW:  // very slow
            p = 15485863;
            q = 15485857;
            break;
        case DIFFERENT_LENGTHS:  // different lengths
            p = 15449411;
            q = 8975063;
            break;
        default: return 1;
    }
    Solution s = solve(p*q);
    render_any("ACTUAL");
    render(p); render(q);
    render_any("EXPECTED");
    render(s.p); render(s.q);
    return 0;
}