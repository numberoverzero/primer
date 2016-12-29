#include <bitset>
#include <solver.h>
#include <debugging.h>


int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout<<"usage: "<< argv[0] <<" PUBLIC" << std::endl;
        return 1;
    }
    uint64_t pq = std::stoull(argv[1]);
    Solution s = solve(pq);
    std::cout << pq << ": " << s.p << " " << s.q << "\n";
    return 0;
}