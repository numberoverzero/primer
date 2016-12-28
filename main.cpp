#include <bitset>
#include <iostream>
#include <bits.h>

static int counter = 0;
template <typename T>
void render_any(T t) { std::cout << counter++ << " " << t << std::endl; }
void render_bits(uint32_t x) { render_any(std::bitset<32>(x)); }
void render_bool(bool t) { render_any(t? "true" : "false"); }
void render(uint8_t t) { render_any(unsigned(t)); }
void render(uint32_t t) { render_any(unsigned(t)); }


int main() {
    uint32_t n = 0b101001;
    uint32_t nrev = reverse(n);
    uint32_t nrev_left = keep_left(nrev, 5);
    uint32_t n2 = reverse(nrev_left);
    uint32_t n2_left = keep_left(n2, 5);
    render_bits(n);
    render_bits(nrev);
    render_bits(nrev_left);
    render_bits(n2);
    render_bits(n2_left);
    render_bool(n == n2_left);
    uint8_t t = diagonal_multiply(0b111101100, 0b101010101, 7);
    // 11101100
    // 10101010 (from reversed 01010101)
    // --------
    // 11101000
    render(t);
    render (((uint32_t)0) - ((uint32_t)1));
    return 0;
}