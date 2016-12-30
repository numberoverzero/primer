#include <cstdint>
#include <iostream>
#include <bitset>

static long counter = 0;

template <typename T>
void render_any(T t) { std::cout << counter++ << " " << t << std::endl; }

template <typename T>
void render(T t) { render_any(unsigned(t)); }

template <typename T>
void render_bits(T t) { render_any(std::bitset<64>(t)); }