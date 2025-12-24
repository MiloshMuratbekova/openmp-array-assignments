// библиотеки которые нам нужны
#include <omp.h>

#include <chrono>
#include <climits>
#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;
using namespace chrono;

int main() {
    // запускаем генератор случайных чисел
    srand(time(nullptr));

    // ========== Задание 1 ==========
    cout << "Задание 1\n";

    // создаем массив на 50000 чисел
    int* arr1 = new int[50000];

    long long sum1 = 0;

    // заполняем массив случайными числами от 1 до 100
    for (int i = 0; i < 50000; i++) {
        arr1[i] = rand() % 100 + 1;
        sum1 += arr1[i];
    }

    // считаем среднее значение
    double avg1 = (double)sum1 / 50000;
    cout << "Среднее значение: " << avg1 << endl;

    // освобождаем память
    delete[] arr1;

    // ========== Задание 2 ==========
    cout << "\nЗадание 2\n";

    int N2 = 1000000;  // миллион элементов
    int* arr2 = new int[N2];

    // заполняем массив случайными числами
    for (int i = 0; i < N2; i++) {
        arr2[i] = rand();
    }

    // начинаем замер времени
    auto start_seq = high_resolution_clock::now();

    int min_seq = INT_MAX;
    int max_seq = INT_MIN;

    // ищем минимум и максимум обычным способом
    for (int i = 0; i < N2; i++) {
        if (arr2[i] < min_seq) {
            min_seq = arr2[i];
        }
        if (arr2[i] > max_seq) {
            max_seq = arr2[i];
        }
    }

    // заканчиваем замер времени
    auto end_seq = high_resolution_clock::now();

    cout << "Минимум: " << min_seq << ", max: " << max_seq << endl;
    cout << "Время: " << duration_cast<milliseconds>(end_seq - start_seq).count() << " ms\n";

    // ========== Задание 3 ==========
    cout << "\nЗадание 3\n";

    int min_par = INT_MAX;
    int max_par = INT_MIN;

    auto start_par = high_resolution_clock::now();

// теперь делаем то же самое но параллельно с OpenMP
// reduction нужен чтобы потоки не мешали друг другу
#pragma omp parallel for reduction(min : min_par) reduction(max : max_par)
    for (int i = 0; i < N2; i++) {
        if (arr2[i] < min_par) {
            min_par = arr2[i];
        }
        if (arr2[i] > max_par) {
            max_par = arr2[i];
        }
    }

    auto end_par = high_resolution_clock::now();

    cout << "Parallel минимум: " << min_par << ", max: " << max_par << endl;
    cout << "Parallel время: " << duration_cast<milliseconds>(end_par - start_par).count()
         << " ms\n";

    delete[] arr2;

    // ========== Задание 4 ==========
    cout << "\nЗадание 4\n";

    int N4 = 5000000;  // 5 миллионов элементов
    int* arr4 = new int[N4];

    // заполняем массив
    for (int i = 0; i < N4; i++) {
        arr4[i] = rand() % 100 + 1;
    }

    // сначала считаем среднее обычным способом
    auto start_avg_seq = high_resolution_clock::now();
    long long sum_seq = 0;

    for (int i = 0; i < N4; i++) {
        sum_seq = sum_seq + arr4[i];
    }

    auto end_avg_seq = high_resolution_clock::now();
    double avg_seq = (double)sum_seq / N4;

    // теперь считаем параллельно с OpenMP
    auto start_avg_par = high_resolution_clock::now();
    long long sum_par = 0;

// reduction(+:sum_par) - чтобы сумма считалась правильно во всех потоках
#pragma omp parallel for reduction(+ : sum_par)
    for (int i = 0; i < N4; i++) {
        sum_par = sum_par + arr4[i];
    }

    auto end_avg_par = high_resolution_clock::now();
    double avg_par = (double)sum_par / N4;

    cout << "Sequential среднее: " << avg_seq << endl;
    cout << "Parallel среднее:   " << avg_par << endl;

    cout << "Sequential время: " << duration_cast<milliseconds>(end_avg_seq - start_avg_seq).count()
         << " ms\n";

    cout << "Parallel время:   " << duration_cast<milliseconds>(end_avg_par - start_avg_par).count()
         << " ms\n";

    delete[] arr4;

    return 0;
}
