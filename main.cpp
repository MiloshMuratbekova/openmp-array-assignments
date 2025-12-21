
#include <iostream>     //ввод/вывод
#include <cstdlib>      //rand(), srand()
#include <ctime>        //time()
#include <chrono>       //замер времени
#include <climits>      //INT_MIN, INT_MAX
#include <omp.h>        //OpenMP

using namespace std;
using namespace chrono;

int main() {
    //инициализация генератора случайных чисел
    srand(time(nullptr));

    //Задание 1 
    cout << "Задание 1\n";

    //динамическое выделение памяти под 50 000 элементов
    int* arr1 = new int[50000];

    long long sum1 = 0;

    //заполнение массива случайными числами от 1 до 100
    for (int i = 0; i < 50000; i++) {
        arr1[i] = rand() % 100 + 1;
        sum1 += arr1[i];
    }

    //вычисление среднего значения
    double avg1 = static_cast<double>(sum1) / 50000;
    cout << "Среднее значение: " << avg1 << endl;

    //освобождение динамической памяти
    delete[] arr1;

    //Задание 2
    cout << "\nЗадание 2\n";

    const int N2 = 1'000'000;
    int* arr2 = new int[N2];

    //заполнение массива случайными значениями
    for (int i = 0; i < N2; i++)
        arr2[i] = rand();

    //начало замера времени
    auto start_seq = high_resolution_clock::now();

    int min_seq = INT_MAX;
    int max_seq = INT_MIN;

    //последовательный поиск минимума и максимума
    for (int i = 0; i < N2; i++) {
        if (arr2[i] < min_seq) min_seq = arr2[i];
        if (arr2[i] > max_seq) max_seq = arr2[i];
    }

    //конец замера времени
    auto end_seq = high_resolution_clock::now();

    cout << "Минимум: " << min_seq << ", max: " << max_seq << endl;
    cout << "Время: "
         << duration_cast<milliseconds>(end_seq - start_seq).count()
         << " ms\n";

    //Задание 3
    cout << "\nЗадание 3\n";

    int min_par = INT_MAX;
    int max_par = INT_MIN;

    auto start_par = high_resolution_clock::now();

    //параллельный цикл с редукцией для min и max
#pragma omp parallel for reduction(min:min_par) reduction(max:max_par)
    for (int i = 0; i < N2; i++) {
        if (arr2[i] < min_par) min_par = arr2[i];
        if (arr2[i] > max_par) max_par = arr2[i];
    }

    auto end_par = high_resolution_clock::now();

    cout << "Parallel минимум: " << min_par << ", max: " << max_par << endl;
    cout << "Parallel время: "
         << duration_cast<milliseconds>(end_par - start_par).count()
         << " ms\n";

    delete[] arr2;

    //Задание 4
    cout << "\nЗадание 4\n";

    const int N4 = 5'000'000;
    int* arr4 = new int[N4];

    for (int i = 0; i < N4; i++)
        arr4[i] = rand() % 100 + 1;

    //последовательное вычисление среднего
    auto start_avg_seq = high_resolution_clock::now();
    long long sum_seq = 0;

    for (int i = 0; i < N4; i++)
        sum_seq += arr4[i];

    auto end_avg_seq = high_resolution_clock::now();
    double avg_seq = static_cast<double>(sum_seq) / N4;

    //параллельное вычисление среднего с reduction
    auto start_avg_par = high_resolution_clock::now();
    long long sum_par = 0;

#pragma omp parallel for reduction(+:sum_par)
    for (int i = 0; i < N4; i++)
        sum_par += arr4[i];

    auto end_avg_par = high_resolution_clock::now();
    double avg_par = static_cast<double>(sum_par) / N4;

    cout << "Sequential среднее: " << avg_seq << endl;
    cout << "Parallel среднее:   " << avg_par << endl;

    cout << "Sequential время: "
         << duration_cast<milliseconds>(end_avg_seq - start_avg_seq).count()
         << " ms\n";

    cout << "Parallel время:   "
         << duration_cast<milliseconds>(end_avg_par - start_avg_par).count()
         << " ms\n";

    delete[] arr4;

    return 0;
}
