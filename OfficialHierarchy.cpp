#include "OfficialHierarchy.h"  // Включение собственного заголовочного файла
#include <iostream>             // Для ввода/вывода
#include <algorithm>            // Для алгоритмов STL (all_of)
#include <climits>              // Для INT_MAX
#include <stdexcept>            // Для исключений
#include <sstream>              // Для работы со строками
#include <cctype>               // Для isdigit и isspace

using namespace std;



bool isDigitsOnly(const string& s) { // Проверяет, содержит ли строка только цифры
    return !s.empty() && all_of(s.begin(), s.end(), ::isdigit);
}


bool isValidNumber(const string& s, bool allowZero) { // Проверяет, является ли строка корректным числом
    if (s.empty()) return false;                     // Пустая строка недопустима
    if (!isDigitsOnly(s)) return false;             // Должны быть только цифры
    if (s[0] == '0' && s.size() > 1) return false; // Запрет ведущих нулей
    if (!allowZero && s == "0") return false;      // Проверка на нулевое значение
    return true;
}


int OfficialHierarchy::getValidatedInput(const string& prompt, int minVal, int maxVal, bool allowZero) { // Получает проверенный ввод от пользователя
    string input;
    int value;

    while (true) {
        cout << prompt;
        getline(cin, input);  // Чтение всей строки ввода

        // Удаление пробелов из введенной строки
        input.erase(remove_if(input.begin(), input.end(), ::isspace), input.end());

        if (input.empty()) {
            cout << "Ошибка: Пустой ввод. Пожалуйста, введите число.\n";
            continue;
        }

        if (!isValidNumber(input, allowZero)) {
            cout << "Ошибка: Введите целое " << (allowZero ? "неотрицательное" : "положительное")
                << " число без лишних символов.\n";
            continue;
        }

        try {
            value = stoi(input);  // Преобразование строки в число
            if (value < minVal || value > maxVal) {
                cout << "Ошибка: Число должно быть от " << minVal << " до " << maxVal << ".\n";
                continue;
            }
            return value;
        }
        catch (...) {
            cout << "Ошибка: Некорректный ввод. Пожалуйста, введите целое число.\n";
        }
    }
}



OfficialNode::OfficialNode(int officialId, int bribe) : // Конструктор OfficialNode
    id(officialId),        // Инициализация ID
    bribeAmount(bribe),    // Инициализация суммы взятки
    boss(nullptr) {}       // Инициализация указателя на начальника (nullptr)


void OfficialNode::setBoss(OfficialNode* officialBoss) { // Установка начальника для текущего чиновника
    boss = officialBoss;  // Установка указателя на начальника
}


void OfficialNode::addSubordinate(OfficialNode* subordinate) { // Добавление подчиненного к текущему чиновнику
    subordinates.push_back(subordinate);  // Добавление в вектор подчиненных
}


int OfficialNode::getId() const { return id; } // Получение ID чиновника


int OfficialNode::getBribe() const { return bribeAmount; } // Получение суммы взятки чиновника


OfficialNode* OfficialNode::getBoss() const { return boss; } // Получение указателя на начальника


const vector<OfficialNode*>& OfficialNode::getSubordinates() const { // Получение списка подчиненных
    return subordinates;
}



// Конструктор OfficialHierarchy
OfficialHierarchy::OfficialHierarchy() : root(nullptr) {}  // Инициализация корня nullptr

// Деструктор OfficialHierarchy(Освобождение ресурсов)
OfficialHierarchy::~OfficialHierarchy() {
    clearHierarchy();  // Очистка иерархии при уничтожении объекта
}



OfficialNode* OfficialHierarchy::findNodeById(int id) const { // Поиск узла чиновника по ID
    // Линейный поиск по всем чиновникам
    for (OfficialNode* official : officials) {
        if (official->getId() == id) {  // Если ID совпадает
            return official;             // Возвращаем найденный узел
        }
    }
    return nullptr;  // Если не нашли, возвращаем nullptr
}


void OfficialHierarchy::buildHierarchy() { // Построение иерархии чиновников (ввод данных от пользователя)
    // Получаем количество чиновников с валидацией (от 1 до 100)
    int numOfficials = getValidatedInput(
        "Введите количество чиновников (1-100): ", 1, 100, false);

    // Создание всех чиновников с валидацией суммы взятки
    for (int i = 1; i <= numOfficials; ++i) {
        int bribe = getValidatedInput(
            "Введите сумму взятки для чиновника " + to_string(i) + ": ",
            0, INT_MAX, true);  // Взятка может быть 0 или больше

        officials.push_back(new OfficialNode(i, bribe));
    }

    // Установка отношений начальник-подчиненный с валидацией
    for (int i = 0; i < numOfficials; ++i) {
        int bossId = getValidatedInput(
            "Введите ID начальника для чиновника " + to_string(i + 1) +
            " (0 если это главный чиновник, 1-" + to_string(numOfficials) + "): ",
            0, numOfficials, true);  // Допустим 0 (для главного) или ID чиновника

        if (bossId == 0) {  // Если это главный чиновник
            if (root != nullptr) {
                cout << "Ошибка: Главный чиновник уже назначен.\n";
                i--; // Повторяем ввод для этого чиновника
                continue;
            }
            root = officials[i];  // Установка корня иерархии
        }
        else {
            OfficialNode* bossNode = findNodeById(bossId);
            officials[i]->setBoss(bossNode);
            bossNode->addSubordinate(officials[i]);
        }
    }

    if (root == nullptr) {
        throw runtime_error("Главный чиновник не назначен");
    }
}


void OfficialHierarchy::calculateMinBribe(OfficialNode* node, int& totalBribe, // Рекурсивный расчет минимальной суммы взяток
    vector<int>& path) const {

    if (node == nullptr) {  // Проверка на нулевой указатель
        return;
    }

    // Если нет подчиненных - листовой узел
    if (node->getSubordinates().empty()) {
        totalBribe += node->getBribe();  // Добавляем взятку
        path.push_back(node->getId());   // Добавляем ID в путь
        return;
    }

    // Поиск подчиненного с минимальной взяткой
    int minBribe = INT_MAX;         // Инициализация минимальной взятки
    OfficialNode* minSubordinate = nullptr;  // Инициализация указателя

    // Перебор всех подчиненных
    for (OfficialNode* sub : node->getSubordinates()) {
        if (sub->getBribe() < minBribe) {  // Поиск минимальной взятки
            minBribe = sub->getBribe();
            minSubordinate = sub;
        }
    }

    // Сравнение взятки текущего чиновника и минимального подчиненного
    if (node->getBribe() <= minBribe) {  // Если взятка текущего меньше
        totalBribe += node->getBribe();  // Добавляем взятку текущего
        path.push_back(node->getId());   // Добавляем ID в путь
    }
    else {  // Если взятка подчиненного меньше
        totalBribe += minBribe;         // Добавляем взятку подчиненного
        path.push_back(minSubordinate->getId());  // Добавляем его ID в путь
        calculateMinBribe(node, totalBribe, path);  // Рекурсивный вызов
    }
}


void OfficialHierarchy::printPath(const vector<int>& path) const { // Вывод пути получения подписей
    cout << "Порядок получения подписей: ";
    for (size_t i = 0; i < path.size(); ++i) {  // Перебор всех элементов пути
        if (i != 0) {  // Если не первый элемент
            cout << " -> ";  // Вывод разделителя
        }
        cout << path[i];  // Вывод ID чиновника
    }
    cout << endl;  // Перевод строки
}


void OfficialHierarchy::findMinBribeSolution() { // Нахождение решения с минимальной суммой взяток
    if (root == nullptr) {  // Проверка, что иерархия построена
        throw runtime_error("Иерархия не построена");
    }

    int totalBribe = 0;      // Инициализация общей суммы взяток
    vector<int> path;        // Инициализация пути подписания

    calculateMinBribe(root, totalBribe, path);  // Расчет решения

    // Вывод результатов
    cout << "\nМинимальная сумма взяток: " << totalBribe << " у.е." << endl;
    printPath(path);  // Вывод пути
}


void OfficialHierarchy::clearHierarchy() { // Очистка иерархии и освобождение памяти
    // Удаление всех чиновников
    for (OfficialNode* official : officials) {
        delete official;  // Освобождение памяти
    }
    officials.clear();  // Очистка вектора
    root = nullptr;     // Сброс указателя на корень
}
