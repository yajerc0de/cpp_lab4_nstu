#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <set>
#include <string>

using namespace std;

// Проверка, является ли число простым
bool isPrime(int num) {
    if (num < 2) return false;
    if (num == 2) return true;
    if (num % 2 == 0) return false;
    for (int d = 3; d * d <= num; d += 2)
        if (num % d == 0) return false;
    return true;
}

// Проверка, есть ли в числе хотя бы две одинаковые цифры
bool hasDuplicateDigits(int num) {
    string s = to_string(num);
    set<char> digits;
    for (char c : s) {
        if (digits.count(c)) return true;
        digits.insert(c);
    }
    return false;
}

int main() {
    random_device rd;
    mt19937_64 gen(rd());

    // ---------- 1. Создание массивов arr1 и arr2 ----------
    int n;
    cout << "Введите n (≥ 10): ";
    cin >> n;
    if (n < 10) n = 10;

    vector<int> arr1(n), arr2(n);
    uniform_int_distribution<int> dist(10, 100);

    for (int i = 0; i < n; ++i) {
        arr1[i] = dist(gen);
        arr2[i] = dist(gen);
    }

    cout << "\narr1: ";
    for (int v : arr1) cout << v << " ";
    cout << "\narr2: ";
    for (int v : arr2) cout << v << " ";
    cout << "\n";

    // ---------- 2. Простые числа из [10,70] ----------
    cout << "\nПростые числа в [10,70] из arr1: ";
    for (int v : arr1) {
        if (v >= 10 && v <= 70 && isPrime(v))
            cout << v << " ";
    }
    cout << "\nПростые числа в [10,70] из arr2: ";
    for (int v : arr2) {
        if (v >= 10 && v <= 70 && isPrime(v))
            cout << v << " ";
    }
    cout << "\n";

    // ---------- 3. Сортировка ----------
    vector<int> arr1_desc = arr1;
    vector<int> arr2_asc = arr2;
    sort(arr1_desc.begin(), arr1_desc.end(), greater<int>());
    sort(arr2_asc.begin(), arr2_asc.end());

    cout << "\narr1 по убыванию: ";
    for (int v : arr1_desc) cout << v << " ";
    cout << "\narr2 по возрастанию: ";
    for (int v : arr2_asc) cout << v << " ";
    cout << "\n";

    // ---------- 4. Обмен элементов ----------
    vector<int> new_arr1 = arr1;
    vector<int> new_arr2 = arr2;
    // Меняем местами элементы arr1 с нечётными индексами
    // и элементы arr2 с чётными индексами (последовательно)
    size_t oddIdx = 1;      // нечётные индексы в arr1
    size_t evenIdx = 0;     // чётные индексы в arr2
    while (oddIdx < new_arr1.size() && evenIdx < new_arr2.size()) {
        swap(new_arr1[oddIdx], new_arr2[evenIdx]);
        oddIdx += 2;
        evenIdx += 2;
    }

    cout << "\nПосле замены (нечётные индексы arr1 ↔ чётные индексы arr2):\n";
    cout << "arr1: ";
    for (int v : new_arr1) cout << v << " ";
    cout << "\narr2: ";
    for (int v : new_arr2) cout << v << " ";
    cout << "\n";

    // ---------- 5. Массив из [10000,20000] и числа с повторяющимися цифрами ----------
    int m;
    cout << "\nВведите m (≥ 15): ";
    cin >> m;
    if (m < 15) m = 15;

    vector<int> bigArr(m);
    uniform_int_distribution<int> distBig(10000, 20000);
    for (int i = 0; i < m; ++i) {
        bigArr[i] = distBig(gen);
    }

    vector<int> filtered;
    for (int v : bigArr) {
        if (hasDuplicateDigits(v))
            filtered.push_back(v);
    }

    cout << "\nИсходный массив (первые 20 элементов):\n";
    for (int i = 0; i < min(20, m); ++i) cout << bigArr[i] << " ";
    if (m > 20) cout << "...";
    cout << "\nЧисла с повторяющимися цифрами: ";
    if (filtered.empty()) {
        cout << "нет таких чисел.";
    } else {
        for (int v : filtered) cout << v << " ";
    }
    cout << "\n";

    return 0;
}