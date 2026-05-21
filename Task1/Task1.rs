use std::io;
use std::f64;

// Исходная функция
fn f(x: f64) -> f64 {
    x.cos() - (x - 1.0).powi(2)
}

// Производная
fn df(x: f64) -> f64 {
    -x.sin() - 2.0 * (x - 1.0)
}

// Преобразование для метода простых итераций (левый корень, x ≈ 0)
fn phi_left(x: f64) -> f64 {
    x - f(x) / 2.0   // f'(0) = 2 → обеспечивает сходимость
}

// Преобразование для метода простых итераций (правый корень, x ≈ 1.3)
fn phi_right(x: f64) -> f64 {
    1.0 + x.cos().sqrt() // при x из (1, 1.5) cos > 0, |φ'| < 1
}

// Отделение корней перебором с шагом
fn find_intervals(start: f64, end: f64, step: f64) -> Vec<(f64, f64)> {
    let mut intervals = Vec::new();
    let mut x1 = start;
    let mut x2 = x1 + step;
    while x2 <= end {
        if f(x1) * f(x2) <= 0.0 {
            intervals.push((x1, x2));
        }
        x1 = x2;
        x2 += step;
    }
    intervals
}

// Метод половинного деления
fn bisection(a: f64, b: f64, eps: f64) -> f64 {
    println!("\nМетод половинного деления");
    println!("{:>5} {:>15} {:>15} {:>15}", "N", "a_n", "b_n", "b_n-a_n");

    let mut a = a;
    let mut b = b;
    let mut n = 0;
    while (b - a) > eps {
        println!("{:>5} {:>15.8} {:>15.8} {:>15.8}", n, a, b, b - a);
        let c = (a + b) / 2.0;
        if f(a) * f(c) <= 0.0 {
            b = c;
        } else {
            a = c;
        }
        n += 1;
    }
    (a + b) / 2.0
}

// Метод Ньютона
fn newton(mut x0: f64, eps: f64) -> f64 {
    println!("\nМетод Ньютона");
    println!("{:>5} {:>15} {:>15} {:>15}", "N", "x_n", "x_n+1", "|dx|");

    let mut n = 0;
    loop {
        let x1 = x0 - f(x0) / df(x0);
        println!("{:>5} {:>15.8} {:>15.8} {:>15.8}", n, x0, x1, (x1 - x0).abs());
        if (x1 - x0).abs() < eps {
            return x1;
        }
        x0 = x1;
        n += 1;
    }
}

// Метод простых итераций (автоматический выбор phi по x0)
fn simple_iterations(mut x0: f64, eps: f64) -> f64 {
    println!("\nМетод простых итераций");
    println!("{:>5} {:>15} {:>15} {:>15}", "N", "x_n", "x_n+1", "|dx|");

    // Выбираем функцию φ в зависимости от начального приближения
    let phi = if x0 < 0.5 { phi_left } else { phi_right };

    let mut n = 0;
    loop {
        let x1 = phi(x0);
        println!("{:>5} {:>15.8} {:>15.8} {:>15.8}", n, x0, x1, (x1 - x0).abs());
        if (x1 - x0).abs() < eps {
            return x1;
        }
        x0 = x1;
        n += 1;
    }
}

fn main() {
    println!("Уравнение: cos(x) - (x-1)^2 = 0");

    // Ввод точности
    println!("Введите точность eps:");
    let mut input = String::new();
    io::stdin().read_line(&mut input).expect("Ошибка ввода");
    let eps: f64 = input.trim().parse().expect("Некорректное число");

    // Отделение корней на интервале [-0.5, 2.5] с шагом 0.2
    let intervals = find_intervals(-0.5, 2.5, 0.2);
    println!("\nОтделение корней:");
    for (a, b) in &intervals {
        println!("[{:.3}; {:.3}]", a, b);
    }

    // Решение для каждого найденного интервала
    for (i, (a, b)) in intervals.iter().enumerate() {
        println!("\n\nКОРЕНЬ {}", i + 1);

        // Метод половинного деления
        let root_bis = bisection(*a, *b, eps);
        println!("\nКорень = {:.8}", root_bis);
        println!("f(x) = {:.2e}", f(root_bis));

        // Метод Ньютона (начальное приближение – середина интервала)
        let x0 = (a + b) / 2.0;
        let root_newton = newton(x0, eps);
        println!("\nКорень = {:.8}", root_newton);
        println!("f(x) = {:.2e}", f(root_newton));

        // Метод простых итераций
        let root_iter = simple_iterations(x0, eps);
        println!("\nКорень = {:.8}", root_iter);
        println!("f(x) = {:.2e}", f(root_iter));
    }
}