#include <cstdint>
#include <bits.h>
#include <debugging.h>



int main() {
    uint64_t x = 0b0011110;
    render_bits(x);
    render_bits(reverse(x));

    return 0;
}