#include <cstdint>
#include <bits.h>
#include <debugging.h>



int main() {
    uint64_t x = 0b1111101110001100001100000110000001100000001100000000110001010101;
    render_bits(x);
    render_bits(reverse(x));
    render_bits(reverse(x) << 5);

    return 0;
}