#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <sstream>
#include <algorithm>

using namespace std;

// Поиск позиции кандидата в предпочтении избирателя (0 = лучший)
int findPosition(const vector<string>& pref, const string& cand) {
    for (size_t i = 0; i < pref.size(); ++i)
        if (pref[i] == cand) return i;
    return -1;
}

// Метод Борда
map<string, int> bordaScore(const vector<string>& candidates, const vector<vector<string>>& votes) {
    int n = candidates.size();
    map<string, int> scores;
    for (const string& c : candidates) scores[c] = 0;

    for (const auto& vote : votes) {
        for (size_t pos = 0; pos < vote.size(); ++pos) {
            const string& cand = vote[pos];
            scores[cand] += (n - 1 - pos);
        }
    }
    return scores;
}

// Метод Кондорсе
string condorcetWinner(const vector<string>& candidates, const vector<vector<string>>& votes) {
    int n = candidates.size();
    map<string, map<string, int>> beat;
    for (const string& a : candidates)
        for (const string& b : candidates)
            if (a != b) beat[a][b] = 0;

    for (const auto& vote : votes) {
        for (size_t i = 0; i < vote.size(); ++i) {
            for (size_t j = i + 1; j < vote.size(); ++j) {
                beat[vote[i]][vote[j]]++;
            }
        }
    }

    for (const string& a : candidates) {
        bool winsAll = true;
        for (const string& b : candidates) {
            if (a == b) continue;
            if (beat[a][b] <= beat[b][a]) {
                winsAll = false;
                break;
            }
        }
        if (winsAll) return a;
    }
    return "";
}

void printScores(const map<string, int>& scores) {
    cout << "\n--- Результаты по методу Борда ---\n";
    for (const auto& p : scores)
        cout << p.first << ": " << p.second << " очков\n";
    int maxScore = -1;
    string winner;
    for (const auto& p : scores) {
        if (p.second > maxScore) {
            maxScore = p.second;
            winner = p.first;
        }
    }
    cout << "\nПобедитель по Борду: " << winner << " (" << maxScore << " очков)\n";
}

int main() {
    int nCandidates;
    cout << "Введите количество кандидатов: ";
    cin >> nCandidates;
    cin.ignore();
    vector<string> candidates(nCandidates);
    cout << "Введите имена кандидатов (каждое с новой строки):\n";
    for (int i = 0; i < nCandidates; ++i) {
        getline(cin, candidates[i]);
    }

    int nVoters;
    cout << "Введите количество избирателей: ";
    cin >> nVoters;
    cin.ignore();

    vector<vector<string>> votes(nVoters);
    cout << "Для каждого избирателя введите порядок предпочтения (имена кандидатов в порядке убывания предпочтения, через пробел):\n";
    for (int i = 0; i < nVoters; ++i) {
        cout << "Избиратель " << i+1 << ": ";
        string line;
        getline(cin, line);
        stringstream ss(line);
        string name;
        while (ss >> name) {
            votes[i].push_back(name);
        }
        // Если ввели меньше, чем кандидатов, то запрашиваем остальных
        while (votes[i].size() < candidates.size()) {
            string extra;
            cin >> extra;
            votes[i].push_back(extra);
        }
        // Очистка буфера после возможного дополнительного ввода
        if (votes[i].size() > candidates.size()) {
            cin.ignore();
        }
    }

    auto borda = bordaScore(candidates, votes);
    printScores(borda);

    string cWinner = condorcetWinner(candidates, votes);
    cout << "\n--- Результаты по методу Кондорсе ---\n";
    if (cWinner.empty())
        cout << "Победитель по Кондорсе не определён (цикл Кондорсе).\n";
    else
        cout << "Победитель по Кондорсе: " << cWinner << "\n";

    return 0;
}