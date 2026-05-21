use std::collections::BTreeMap;
use std::io::{self, Write};

// Поиск позиции кандидата в предпочтении избирателя (0 = лучший)
#[allow(dead_code)]
fn find_position(pref: &[String], cand: &str) -> Option<usize> {
    pref.iter().position(|c| c == cand)
}

// Метод Борда
fn borda_score(candidates: &[String], votes: &[Vec<String>]) -> BTreeMap<String, i32> {
    let n = candidates.len() as i32;
    let mut scores = BTreeMap::new();
    for c in candidates {
        scores.insert(c.clone(), 0);
    }
    for vote in votes {
        for (pos, cand) in vote.iter().enumerate() {
            let points = n - 1 - pos as i32;
            *scores.get_mut(cand).unwrap() += points;
        }
    }
    scores
}

// Метод Кондорсе
fn condorcet_winner(candidates: &[String], votes: &[Vec<String>]) -> Option<String> {
    // Инициализируем матрицу парных побед
    let mut beat = BTreeMap::new();
    for a in candidates {
        let mut row = BTreeMap::new();
        for b in candidates {
            if a != b {
                row.insert(b.clone(), 0);
            }
        }
        beat.insert(a.clone(), row);
    }

    // Подсчёт предпочтений
    for vote in votes {
        for i in 0..vote.len() {
            for j in i + 1..vote.len() {
                let winner = &vote[i];
                let loser = &vote[j];
                *beat.get_mut(winner).unwrap().get_mut(loser).unwrap() += 1;
            }
        }
    }

    // Проверка кандидата, побеждающего всех
    for a in candidates {
        let mut wins_all = true;
        for b in candidates {
            if a == b {
                continue;
            }
            let a_beats_b = beat[a][b];
            let b_beats_a = beat[b][a];
            if a_beats_b <= b_beats_a {
                wins_all = false;
                break;
            }
        }
        if wins_all {
            return Some(a.clone());
        }
    }
    None
}

fn print_scores(scores: &BTreeMap<String, i32>) {
    println!("\n--- Результаты по методу Борда ---");
    for (name, score) in scores {
        println!("{}: {} очков", name, score);
    }
    let winner = scores.iter().max_by_key(|&(_, &score)| score).unwrap();
    println!("\nПобедитель по Борду: {} ({} очков)", winner.0, winner.1);
}

fn main() {
    // Ввод кандидатов
    print!("Введите количество кандидатов: ");
    io::stdout().flush().unwrap();
    let mut input = String::new();
    io::stdin().read_line(&mut input).unwrap();
    let n_candidates: usize = input.trim().parse().expect("Некорректное число");
    let mut candidates = Vec::with_capacity(n_candidates);
    println!("Введите имена кандидатов (каждое с новой строки):");
    for _ in 0..n_candidates {
        let mut name = String::new();
        io::stdin().read_line(&mut name).unwrap();
        candidates.push(name.trim().to_string());
    }

    // Ввод избирателей
    print!("Введите количество избирателей: ");
    io::stdout().flush().unwrap();
    input.clear();
    io::stdin().read_line(&mut input).unwrap();
    let n_voters: usize = input.trim().parse().expect("Некорректное число");
    let mut votes = Vec::with_capacity(n_voters);
    println!("Для каждого избирателя введите порядок предпочтения (имена кандидатов в порядке убывания предпочтения, через пробел):");
    for i in 0..n_voters {
        print!("Избиратель {}: ", i + 1);
        io::stdout().flush().unwrap();
        let mut line = String::new();
        io::stdin().read_line(&mut line).unwrap();
        let preferences: Vec<String> = line
            .split_whitespace()
            .map(|s| s.to_string())
            .collect();
        // Если ввели меньше, чем кандидатов, запрашиваем остальных
        let mut pref = preferences;
        while pref.len() < candidates.len() {
            let mut extra = String::new();
            io::stdin().read_line(&mut extra).unwrap();
            pref.extend(extra.split_whitespace().map(|s| s.to_string()));
        }
        votes.push(pref);
    }

    // Расчёт и вывод
    let borda = borda_score(&candidates, &votes);
    print_scores(&borda);

    let c_winner = condorcet_winner(&candidates, &votes);
    println!("\n--- Результаты по методу Кондорсе ---");
    match c_winner {
        Some(winner) => println!("Победитель по Кондорсе: {}", winner),
        None => println!("Победитель по Кондорсе не определён (цикл Кондорсе)."),
    }
}