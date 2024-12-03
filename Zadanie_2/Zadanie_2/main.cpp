/* Задание 2
 For_each

     Реализуйте параллельный вариант оператора for_each.
     Функция должна принимать итераторы на начало и конец контейнера и ссылку, обрабатывающую функцию.
     При реализации нужно рекурсивно разделить контейнер на несколько блоков и для каждого запустить отдельную задачу, применяющую обрабатывающую функцию к части контейнера.
 */


#include <iostream>
#include <vector>
#include <thread>
#include <future>
#include <algorithm>

template<typename Iterator, typename Function> // шаблон функции для поиска минимального элемента
void parallel_for_each(Iterator begin, Iterator end, Function f) { // функция для параллельного выполнения функции для каждого элемента в диапазоне
    auto distance = std::distance(begin, end); // вычисляем расстояние между началом и концом диапазона

    // Порог, при котором мы перестаем делить контейнер и выполняем обработку последовательно
    const std::size_t threshold = 1000; // порог можно изменить на любое значение

    // Если количество элементов в контейнере меньше порога, обрабатываем их последовательно
    if (distance <= threshold) { // если рассторяние меньше порога
        std::for_each(begin, end, f); // выполняем обработку последовательно
    } else {
        // Находим середину для разделения
        Iterator mid = begin + distance / 2; // вычисляем середину

        // Запускаем две асинхронные задачи для обработки обеих половин контейнера
        std::future<void> first_half = std::async(std::launch::async, parallel_for_each<Iterator, Function>, begin, mid, f); // запускаем первую часть
        std::future<void> second_half = std::async(std::launch::async, parallel_for_each<Iterator, Function>, mid, end, f); // запускаем вторую часть

        // Ждем завершения выполнения обеих задач
        first_half.get();
        second_half.get();
    }
}


void process(int& value) { // ф-ия для обработки значаения
    value *= 2; // удваиваем значения
}

int main() {
    std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}; // инициализация массива

    // Параллельная обработка
    parallel_for_each(data.begin(), data.end(), process); // выполняем обработку паралллельно

    // Вывод результатов
    for (const auto& value : data) {
        std::cout << value << " ";
    }
    std::cout << std::endl;

    return 0;
}
