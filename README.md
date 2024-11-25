# Matrix Computation with OpenMP

### Студент
- Имя: Ван Цзыхань
- ST: st136731

# Задание A-10 (Цзыхань Ван)
=============================
```
Написать программу вычисления матричного выражения:

\[
A = B + C^2 + \text{Tr}(B) C^3 + I,
\]

где \(B\), \(C\) — квадратные плотные матрицы, элементы которых имеют тип `double`,  
\(I\) — единичная матрица, \(\text{Tr}(B)\) — след матрицы \(B\). Распараллелить эту  
программу с помощью OpenMP (parallel, task). Исследовать зависимость  
масштабируемости параллельной версии программы от её вычислительной  
трудоёмкости (размера матриц). Проверить корректность параллельной  
версии.

Проверка закона Амдала. Построить зависимость ускорение:число потоков для  
заданного примера.

Срок сдачи отчёта 26.11.2024.
```
## 

## 1.структура содержания

```
parallel/
    └── src/
        ├── draw_result.py                  # ответственен за построение графиков результатов
        ├── matrix_operations_openMP.cpp    # Этот файл содержит операции с матрицами
        ├── matrix_operations_openMP.h      # 
        ├── sequential_execution_openMP.cpp # главная функция
        └── speedup_results.txt             # result

```
## 2.объяснение основных функций
```c
initializeMatrix() // Инициализация матрицы
identityMatrix() // Создание единичной матрицы
traceMatrix() // Вычисление следа матрицы
multiplyMatrices() // Умножение матриц
scalarMultiplyMatrix() // Умножение матрицы на скаляр
addMatrices() // Сложение матриц
compute_error() // Вычисление ошибки
relative_error() // Вычисление относительной ошибки
```

## 3.экспериментальный процесс
#### Примечание: Время работы программы не включает время инициализации матрицы.
### Компиляция и запуск основной функции
```
g++ -fopenmp sequential_execution_openMP.cpp matrix_operations_openMP.cpp -o sequential_execution_openMP.exe
.\sequential_execution.exe
```
### Компиляция и запуск основной функции
![result](imag/result.png)
```
PS D:\workspace\parallel\src> .\sequential_execution_openMP.exe
Program started.
N = 800
Matrix computation completed in 2.591 seconds.
1-thread execution time: 2.591 seconds.
Matrix computation completed in 1.33 seconds.
2-thread execution time: 1.33 seconds.
, Absolute Error (wiht T1): 0.0116587, Relative Error (wiht T1): 5.63723e-16, Speed up(Sp) :1.94812
Matrix computation completed in 0.721 seconds.
4-thread execution time: 0.721 seconds.
, Absolute Error (wiht T1): 0.0145747, Relative Error (wiht T1): 7.04722e-16, Speed up(Sp) :3.59362
Matrix computation completed in 0.425 seconds.
8-thread execution time: 0.425 seconds.
, Absolute Error (wiht T1): 0.00874292, Relative Error (wiht T1): 4.2274e-16, Speed up(Sp) :6.09647
Matrix computation completed in 0.319 seconds.
16-thread execution time: 0.319 seconds.
, Absolute Error (wiht T1): 0.0116587, Relative Error (wiht T1): 5.63723e-16, Speed up(Sp) :8.12226
Matrix computation completed in 0.245 seconds.
32-thread execution time: 0.245 seconds.
, Absolute Error (wiht T1): 0.0116587, Relative Error (wiht T1): 5.63723e-16, Speed up(Sp) :10.5755
Saved to 'speedup_results.txt'.
PS D:\workspace\parallel\src>
```
### Включает время вычислений каждого потока, а также ошибку по сравнению с результатами для одного потока. Поскольку матрицы содержат данные типа double, в параллельных вычислениях неизбежны различия, но ошибка находится в пределах допустимого. Далее на основе значения ускорения будет построен график.

| Threads | Execution Time (s) | Absolute Error (with T1) | Relative Error (with T1) | Speed up (Sp) |
|---------|--------------------|--------------------------|---------------------------|---------------|
| 1       | 2.591              | -                        | -                         | -             |
| 2       | 1.33               | 0.0116587                | 5.63723e-16               | 1.94812       |
| 4       | 0.721              | 0.0145747                | 7.04722e-16               | 3.59362       |
| 8       | 0.425              | 0.00874292               | 4.2274e-16                | 6.09647       |
| 16      | 0.319              | 0.0116587                | 5.63723e-16               | 8.12226       |
| 32      | 0.245              | 0.0116587                | 5.63723e-16               | 10.5755       |

### Построение графиков с использованием matplotlib
![result](imag/Figure_result1.png)
![result](imag/Figure_result2.png)
## Amdahl's Law
\[
S(p) = \frac{1}{(1 - P) + \frac{P}{n}}
\]
### Согласно формуле, можно заметить, что при увеличении числа процессоров ускорение постепенно приближается к фиксированному значению, и график соответствует этому закону.