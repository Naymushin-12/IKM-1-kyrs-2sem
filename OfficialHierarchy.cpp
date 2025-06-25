#include "OfficialHierarchy.h"
#include <iostream>
#include <algorithm>
#include <climits>
#include <sstream>

using namespace std;

/* Конструктор OfficialNode */
OfficialNode::OfficialNode(int id, int bribe) : id(id), bribeAmount(bribe) {}

/* Установка начальника для текущего чиновника */
void OfficialNode::setBoss(OfficialNode* boss) {
    if (boss) boss->addSubordinate(this); // Добавляем себя к подчиненным начальника
}

/* Добавление подчиненного */
void OfficialNode::addSubordinate(OfficialNode* subordinate) {
    subordinates.push_back(subordinate);
}

/* Геттеры */
int OfficialNode::getId() const { return id; }
int OfficialNode::getBribe() const { return bribeAmount; }
const vector<OfficialNode*>& OfficialNode::getSubordinates() const { return subordinates; }

/* Конструктор и деструктор OfficialHierarchy */
OfficialHierarchy::OfficialHierarchy() = default;
OfficialHierarchy::~OfficialHierarchy() { clear(); }

/* Очистка иерархии */
void OfficialHierarchy::clear() {
    officials.clear();  // unique_ptr автоматически удаляют объекты
    root = nullptr;
}

/* Поиск чиновника по ID */
OfficialNode* OfficialHierarchy::findNode(int id) const {
    auto it = officials.find(id);
    return it != officials.end() ? it->second.get() : nullptr;
}

/* Рекурсивный поиск всех путей в иерархии */
void OfficialHierarchy::findAllPaths(OfficialNode* node,
                                   vector<int>& currentPath,
                                   vector<vector<int>>& allPaths,
                                   int currentSum) const {
    if (!node) return;
    
    // Добавляем текущего чиновника в путь
    currentPath.push_back(node->getId());
    currentSum += node->getBribe();
    
    // Если это листовой узел (нет подчиненных) - сохраняем путь
    if (node->getSubordinates().empty()) {
        allPaths.emplace_back(currentPath);
        return;
    }
    
    // Рекурсивно обрабатываем всех подчиненных
    for (OfficialNode* sub : node->getSubordinates()) {
        findAllPaths(sub, currentPath, allPaths, currentSum);
        currentPath.pop_back(); // Возвращаемся назад для поиска других путей
    }
}

/* Вывод результатов на экран */
void OfficialHierarchy::printResults(const vector<int>& bestPath, int minBribe) const {
    cout << "\nМинимальная сумма взяток: " << minBribe << " у.е.\n";
    cout << "Оптимальный путь: ";
    for (size_t i = 0; i < bestPath.size(); ++i) {
        if (i != 0) cout << " -> ";
        cout << bestPath[i];
    }
    cout << endl;
}

/* Построение иерархии через консольный ввод */
void OfficialHierarchy::buildFromConsole() {
    clear(); // Очищаем предыдущую иерархию
    
    // Лямбда-функция для безопасного ввода чисел
    auto readInt = [](const string& prompt, int min, int max) {
        int value;
        while (true) {
            cout << prompt;
            string input;
            getline(cin, input);
            try {
                value = stoi(input);
                if (value >= min && value <= max) return value;
                cout << "Ошибка: число должно быть от " << min << " до " << max << endl;
            } catch (...) {
                cout << "Ошибка: введите целое число\n";
            }
        }
    };
    
    // Ввод количества чиновников
    int count = readInt("Введите количество чиновников (1-100): ", 1, 100);
    
    // Ввод взяток для каждого чиновника
    for (int i = 1; i <= count; ++i) {
        int bribe = readInt("Введите взятку для чиновника " + to_string(i) + ": ", 0, INT_MAX);
        officials[i] = make_unique<OfficialNode>(i, bribe);
    }
    
    // Построение иерархии (указание начальников)
    for (int i = 1; i <= count; ++i) {
        int bossId = readInt("Введите начальника для чиновника " + to_string(i) + 
                            " (0 для главного): ", 0, count);
        
        if (bossId == 0) {
            if (root) throw runtime_error("Главный чиновник уже назначен");
            root = findNode(i);
        } else {
            OfficialNode* boss = findNode(bossId);
            if (!boss) throw runtime_error("Неверный ID начальника");
            findNode(i)->setBoss(boss);
        }
    }
    
    if (!root) throw runtime_error("Главный чиновник не назначен");
}

/* Поиск оптимального пути с минимальными взятками */
void OfficialHierarchy::findOptimalBribePath() const {
    if (!root) throw runtime_error("Иерархия не построена");
    
    vector<vector<int>> allPaths; // Все возможные пути
    vector<int> currentPath;     // Текущий исследуемый путь
    
    // Находим все возможные пути
    findAllPaths(root, currentPath, allPaths, 0);
    
    int minBribe = INT_MAX;
    vector<int> bestPath;
    
    // Ищем путь с минимальной суммой взяток
    for (const auto& path : allPaths) {
        int sum = 0;
        for (int id : path) {
            sum += findNode(id)->getBribe();
        }
        if (sum < minBribe) {
            minBribe = sum;
            bestPath = path;
        }
    }
    
    // Выводим результаты
    printResults(bestPath, minBribe);
}
