#include <iostream>
#include <random>

int32_t main() {
    // Number of points generated - N in range [-MAX_LIM, MAX_LIM]
    int N, MAX_LIM;
    std::cin >> N >> MAX_LIM;
    std::random_device device;
    std::mt19937 rng(device());
    std::uniform_int_distribution<int64_t> dist(-MAX_LIM, MAX_LIM);
    std::uniform_int_distribution<uint64_t> penalty(1, 100);

    // Penalty for new line
    auto cur_penalty = penalty(rng) % 100;
    while (!cur_penalty)
        cur_penalty = penalty(rng) % 100;

    std::cout << N << ' ' << cur_penalty << '\n';

    for (int i = 0; i < N; i++) {
        std::cout << dist(rng) << ' ' << dist(rng) <<'\n';
}
}