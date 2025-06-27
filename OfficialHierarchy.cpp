// OfficialHierarchy.cpp
#include "OfficialHierarchy.h"
#include <iostream>
#include <climits>
#include <sstream>

using namespace std;

/* Конструктор OfficialNode */
OfficialNode::OfficialNode(int id, int bribe) : id(id), bribeAmount(bribe), subordinatesHead(nullptr) {}

/* Установка начальника для текущего чиновника */
void OfficialNode::setBoss(OfficialNode* boss) {
    if (boss) boss->addSubordinate(this);
}

/* Добавление подчиненного */
void OfficialNode::addSubordinate(OfficialNode* subordinate) {
    SubordinateNode* newNode = new SubordinateNode(subordinate);
    newNode->next = subordinatesHead;
    subordinatesHead = newNode;
}

/* Геттеры */
int OfficialNode::getId() const { return id; }
int OfficialNode::getBribe() const { return bribeAmount; }
SubordinateNode* OfficialNode::getSubordinates() const { return subordinatesHead; }

/* Деструктор PathList */
PathList::~PathList() {
    PathNode* current = head;
    while (current != nullptr) {
        PathNode* next = current->next;
        delete current;
        current = next;
    }
}

/* Добавление узла в конец списка */
void PathList::addNode(int id) {
    PathNode* newNode = new PathNode(id);
    if (tail == nullptr) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        tail = newNode;
    }
}

/* Создание копии списка */
PathList* PathList::clone() const {
    PathList* newList = new PathList();
    PathNode* current = head;
    while (current != nullptr) {
        newList->addNode(current->id);
        current = current->next;
    }
    return newList;
}

/* Деструктор PathListCollection */
PathListCollection::~PathListCollection() {
    PathListNode* current = head;
    while (current != nullptr) {
        PathListNode* next = current->next;
        delete current->data;
        delete current;
        current = next;
    }
}

/* Добавление пути в коллекцию */
void PathListCollection::addPath(PathList* path) {
    PathListNode* newNode = new PathListNode(path);
    if (head == nullptr) {
        head = newNode;
    } else {
        PathListNode* temp = head;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

/* Конструктор и деструктор OfficialHierarchy */
OfficialHierarchy::OfficialHierarchy() : officialsHead(nullptr), root(nullptr) {}
OfficialHierarchy::~OfficialHierarchy() { clear(); }

/* Очистка иерархии */
void OfficialHierarchy::clear() {
    // Удаляем всех чиновников
    OfficialListNode* current = officialsHead;
    while (current != nullptr) {
        OfficialListNode* next = current->next;
        
        // Удаляем список подчиненных
        SubordinateNode* subCurrent = current->data->getSubordinates();
        while (subCurrent != nullptr) {
            SubordinateNode* subNext = subCurrent->next;
            delete subCurrent;
            subCurrent = subNext;
        }
        
        delete current->data;
        delete current;
        current = next;
    }
    officialsHead = nullptr;
    root = nullptr;
}

/* Поиск чиновника по ID */
OfficialNode* OfficialHierarchy::findNode(int id) const {
    OfficialListNode* current = officialsHead;
    while (current != nullptr) {
        if (current->data->getId() == id) {
            return current->data;
        }
        current = current->next;
    }
    return nullptr;
}

/* Рекурсивный поиск всех путей в иерархии */
void OfficialHierarchy::findAllPaths(OfficialNode* node,
                                  PathList* currentPath,
                                  PathListCollection& allPaths,
                                  int currentSum) const {
    if (!node) return;
    
    // Добавляем текущего чиновника в путь
    currentPath->addNode(node->getId());
    currentSum += node->getBribe();
    
    // Если это листовой узел (нет подчиненных) - сохраняем путь
    if (node->getSubordinates() == nullptr) {
        allPaths.addPath(currentPath->clone());
        return;
    }
    
    // Рекурсивно обрабатываем всех подчиненных
    SubordinateNode* sub = node->getSubordinates();
    while (sub != nullptr) {
        findAllPaths(sub->data, currentPath->clone(), allPaths, currentSum);
        sub = sub->next;
    }
    
    delete currentPath; // Удаляем временный путь
}

/* Вывод результатов на экран */
void OfficialHierarchy::printResults(PathList* bestPath, int minBribe) const {
    cout << "\nМинимальная сумма взяток: " << minBribe << " у.е.\n";
    cout << "Оптимальный путь: ";
    
    PathNode* current = bestPath->head;
    bool first = true;
    while (current != nullptr) {
        if (!first) cout << " -> ";
        cout << current->id;
        first = false;
        current = current->next;
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
        
        OfficialNode* newOfficial = new OfficialNode(i, bribe);
        OfficialListNode* newNode = new OfficialListNode(newOfficial);
        
        // Добавляем в начало списка
        newNode->next = officialsHead;
        officialsHead = newNode;
    }
    
    // Построение иерархии (указание начальников)
    OfficialListNode* current = officialsHead;
    while (current != nullptr) {
        int id = current->data->getId();
        int bossId = readInt("Введите начальника для чиновника " + to_string(id) + 
                            " (0 для главного): ", 0, count);
        
        if (bossId == 0) {
            if (root) throw runtime_error("Главный чиновник уже назначен");
            root = current->data;
        } else {
            OfficialNode* boss = findNode(bossId);
            if (!boss) throw runtime_error("Неверный ID начальника");
            current->data->setBoss(boss);
        }
        
        current = current->next;
    }
    
    if (!root) throw runtime_error("Главный чиновник не назначен");
}

/* Поиск оптимального пути с минимальными взятками */
void OfficialHierarchy::findOptimalBribePath() const {
    if (!root) throw runtime_error("Иерархия не построена");
    
    PathListCollection allPaths; // Все возможные пути
    PathList* currentPath = new PathList(); // Текущий исследуемый путь
    
    // Находим все возможные пути
    findAllPaths(root, currentPath, allPaths, 0);
    
    int minBribe = INT_MAX;
    PathList* bestPath = nullptr;
    
    // Ищем путь с минимальной суммой взяток
    PathListCollection::PathListNode* pathNode = allPaths.head;
    while (pathNode != nullptr) {
        int sum = 0;
        PathNode* node = pathNode->data->head;
        while (node != nullptr) {
            OfficialNode* official = findNode(node->id);
            sum += official->getBribe();
            node = node->next;
        }
        
        if (sum < minBribe) {
            minBribe = sum;
            bestPath = pathNode->data;
        }
        
        pathNode = pathNode->next;
    }
    
    // Выводим результаты
    if (bestPath) {
        printResults(bestPath, minBribe);
    } else {
        cout << "Пути не найдены" << endl;
    }
}
