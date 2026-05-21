#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <ctime>

using namespace std;

// true = сотрудничество
// false = предательство

// Стратегия 1: всегда сотрудничать
bool always_cooperate(int round_number,
                      const vector<bool>& self_choices,
                      const vector<bool>& enemy_choices)
{
    return true;
}

// Стратегия 2: око за око (tit-for-tat)
bool tit_for_tat(int round_number,
                 const vector<bool>& self_choices,
                 const vector<bool>& enemy_choices)
{
    if (round_number == 0)
        return true;
    return enemy_choices.back();
}

// Стратегия 3: чередование (чётный раунд – сотрудничество, нечётный – предательство)
bool alternating(int round_number,
                 const vector<bool>& self_choices,
                 const vector<bool>& enemy_choices)
{
    return round_number % 2 == 0;
}

// Подсчёт очков за один раунд
void add_score(bool a, bool b, int& score_a, int& score_b)
{
    if (a && b) {
        score_a += 24;
        score_b += 24;
    }
    else if (!a && !b) {
        score_a += 4;
        score_b += 4;
    }
    else if (a && !b) {
        score_a += 0;
        score_b += 20;
    }
    else { // !a && b
        score_a += 20;
        score_b += 0;
    }
}

// Проведение одной игры между двумя стратегиями
void play_game(const string& name_a,
               bool (*strategy_a)(int, const vector<bool>&, const vector<bool>&),
               const string& name_b,
               bool (*strategy_b)(int, const vector<bool>&, const vector<bool>&),
               mt19937& rng)
{
    int score_a = 0;
    int score_b = 0;

    vector<bool> history_a;
    vector<bool> history_b;

    // Генерация случайного числа раундов от 100 до 200 с помощью mt19937
    uniform_int_distribution<int> dist_rounds(100, 200);
    int rounds = dist_rounds(rng);

    cout << "\n=====================================\n";
    cout << "Игра: " << name_a << " против " << name_b << "\n";
    cout << "Количество раундов: " << rounds << "\n";
    cout << "=====================================\n";

    for (int round = 0; round < rounds; ++round)
    {
        bool a = strategy_a(round, history_a, history_b);
        bool b = strategy_b(round, history_b, history_a);

        history_a.push_back(a);
        history_b.push_back(b);

        add_score(a, b, score_a, score_b);

        cout << "Раунд " << round + 1 << ": "
             << name_a << " -> " << (a ? "СОТРУДНИЧАЕТ" : "ПРЕДАЁТ")
             << " | "
             << name_b << " -> " << (b ? "СОТРУДНИЧАЕТ" : "ПРЕДАЁТ")
             << "\n";
    }

    cout << "\n---------- РЕЗУЛЬТАТ ----------\n";
    cout << name_a << ": " << score_a << " очков\n";
    cout << name_b << ": " << score_b << " очков\n";

    if (score_a > score_b)
        cout << "Победитель: " << name_a << "\n";
    else if (score_b > score_a)
        cout << "Победитель: " << name_b << "\n";
    else
        cout << "Ничья!\n";
}

int main()
{
    // Инициализация генератора mt19937
    random_device rd;
    mt19937 rng(rd());

    // Проведение трёх игр, как в исходном задании
    play_game("Всегда сотрудничает", always_cooperate,
              "Око за око", tit_for_tat,
              rng);

    play_game("Око за око", tit_for_tat,
              "Чередование", alternating,
              rng);

    play_game("Всегда сотрудничает", always_cooperate,
              "Чередование", alternating,
              rng);

    return 0;
}