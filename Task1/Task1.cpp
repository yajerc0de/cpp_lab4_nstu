#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>

using namespace std;

// Исходная функция
double f(double x) {
    return cos(x) - (x - 1) * (x - 1);
}

// Производная
double df(double x) {
    return -sin(x) - 2 * (x - 1);
}

// Преобразование для метода простых итераций (левый корень x ≈ 0)
double phi_left(double x) {
    return x - f(x) / 2.0;   // f'(0)=2 → обеспечивает сходимость
}

// Преобразование для метода простых итераций (правый корень x ≈ 1.3)
double phi_right(double x) {
    return 1.0 + sqrt(cos(x));  // при x из (1, 1.5) cos > 0, |φ'| < 1
}

// Отделение корней перебором с шагом
vector<pair<double, double>> findIntervals(double start, double end, double step) {
    vector<pair<double, double>> intervals;
    double x1 = start;
    double x2 = x1 + step;
    while (x2 <= end) {
        if (f(x1) * f(x2) <= 0) {
            intervals.push_back({x1, x2});
        }
        x1 = x2;
        x2 += step;
    }
    return intervals;
}

// Метод половинного деления
double bisection(double a, double b, double eps) {
    cout << "\nМетод половинного деления\n";
    cout << setw(5) << "N" << setw(15) << "a_n" << setw(15) << "b_n" << setw(15) << "b_n-a_n" << endl;
    int n = 0;
    while ((b - a) > eps) {
        cout << setw(5) << n << setw(15) << a << setw(15) << b << setw(15) << (b - a) << endl;
        double c = (a + b) / 2.0;
        if (f(a) * f(c) <= 0)
            b = c;
        else
            a = c;
        n++;
    }
    return (a + b) / 2.0;
}

// Метод Ньютона
double newton(double x0, double eps) {
    cout << "\nМетод Ньютона\n";
    cout << setw(5) << "N" << setw(15) << "x_n" << setw(15) << "x_n+1" << setw(15) << "|dx|" << endl;
    int n = 0;
    double x1;
    do {
        x1 = x0 - f(x0) / df(x0);
        cout << setw(5) << n << setw(15) << x0 << setw(15) << x1 << setw(15) << fabs(x1 - x0) << endl;
        n++;
        if (fabs(x1 - x0) < eps) break;
        x0 = x1;
    } while (true);
    return x1;
}

// Метод простых итераций (автоматически выбирает подходящее преобразование по x0)
double simpleIterations(double x0, double eps) {
    cout << "\nМетод простых итераций\n";
    cout << setw(5) << "N" << setw(15) << "x_n" << setw(15) << "x_n+1" << setw(15) << "|dx|" << endl;
    int n = 0;
    double x1;
    // Выбираем функцию φ в зависимости от начального приближения
    auto phi = (x0 < 0.5) ? phi_left : phi_right;
    do {
        x1 = phi(x0);
        cout << setw(5) << n << setw(15) << x0 << setw(15) << x1 << setw(15) << fabs(x1 - x0) << endl;
        n++;
        if (fabs(x1 - x0) < eps) break;
        x0 = x1;
    } while (true);
    return x1;
}

int main() {
    setlocale(LC_ALL, "Russian");
    double eps;
    cout << "Уравнение: cos(x) - (x-1)^2 = 0\n\n";
    cout << "Введите точность eps: ";
    cin >> eps;

    // Отделяем корни на интервале от -0.5 до 2.5 с шагом 0.2
    vector<pair<double, double>> intervals = findIntervals(-0.5, 2.5, 0.2);
    cout << "\nОтделение корней:\n";
    for (size_t i = 0; i < intervals.size(); ++i) {
        cout << "[" << intervals[i].first << "; " << intervals[i].second << "]\n";
    }

    // Решение для каждого найденного интервала
    for (size_t i = 0; i < intervals.size(); ++i) {
        double a = intervals[i].first;
        double b = intervals[i].second;
        cout << "\n\nКОРЕНЬ " << i + 1 << endl;

        // Половинное деление
        double rootBis = bisection(a, b, eps);
        cout << "\nКорень = " << rootBis << endl;
        cout << "f(x) = " << f(rootBis) << endl;

        // Ньютон (начальное приближение – середина интервала)
        double x0 = (a + b) / 2.0;
        double rootNewton = newton(x0, eps);
        cout << "\nКорень = " << rootNewton << endl;
        cout << "f(x) = " << f(rootNewton) << endl;

        // Простые итерации (для корня 0 может дать быстрое решение, для правого – тоже)
        double rootIter = simpleIterations(x0, eps);
        cout << "\nКорень = " << rootIter << endl;
        cout << "f(x) = " << f(rootIter) << endl;
    }

    return 0;
}