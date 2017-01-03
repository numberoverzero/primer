#include <solver.h>
#include <iostream>
#include <bitset>
#include <iomanip>


std::bitset<1> get_bit(uint64_t x, int i) {
    return std::bitset<1>((x >>i)&1);
}

template<int N>
std::bitset<N> get_bits(uint64_t x) {
    uint64_t mask = (1ull<<N)-1;
    return std::bitset<N>(x&mask);
}

std::bitset<64> get_bits(uint64_t x) {
return std::bitset<64>(x);
}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout<<"usage: "<< argv[0] <<" PUBLIC" << std::endl;
        return 1;
    }
    uint64_t pq = std::stoull(argv[1]);
    Solution s = solve(pq);
    std::cout << pq << ": " << s.p << " " << s.q << "\n\n";
    std::cout
            << get_bits(s.p) << '\n' << get_bits(s.q) << '\n'
            << std::setw(64) << std::setfill('=') << "" << '\n'
            << get_bits(pq) << '\n'
            << std::setw(64) << std::setfill('-') << "" << '\n';
    for(int j=0;j<4;j++) {
        for(int i=63; i>=0;i--) {
            std::cout << get_bit(s.carry[i], j);
        } std::cout << "  <- " << j << '\n';
    }


//    for(int i=63; i>=0; i--) {
//        std::cout
//                << get_bits<3>(s.carry[i])
//                << " "
//                << get_bit(pq, i)
//                << ""
//                << get_bit(s.p, i)
//                << ""
//                << get_bit(s.q, i)
//                << '\n';
//    }
    return 0;
}