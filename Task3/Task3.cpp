#include <iostream>
#include <vector>
#include <cstdint>

using namespace std;

// Инициализация состояния Xorshift (32-битный)
uint32_t init_xorshift(uint32_t seed) {
    // Если seed = 0, состояние не может быть нулевым (иначе генератор застрянет на 0)
    if (seed == 0) seed = 1;
    return seed;
}

// Генерация следующего псевдослучайного числа, обновление состояния
uint32_t next_xorshift(uint32_t &state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

// Генерация последовательности из N чисел
vector<uint32_t> generate(uint32_t state, int N) {
    vector<uint32_t> result;
    for (int i = 0; i < N; ++i) {
        result.push_back(next_xorshift(state));
    }
    return result;
}

int main() {
    uint32_t seed;
    int N;

    cout << "Введите seed (целое положительное число): ";
    cin >> seed;

    cout << "Введите N (количество генерируемых чисел): ";
    cin >> N;

    if (N <= 0) {
        cerr << "N должно быть положительным." << endl;
        return 1;
    }

    uint32_t state = init_xorshift(seed);
    vector<uint32_t> output = generate(state, N);

    cout << "\nМассив псевдослучайных чисел (Xorshift32):\n";
    for (size_t i = 0; i < output.size(); ++i) {
        cout << output[i];
        if (i != output.size() - 1) cout << " ";
    }
    cout << endl;

    return 0;
}