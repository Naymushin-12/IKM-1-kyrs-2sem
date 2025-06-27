#include "OfficialHierarchy.h"
#include <iostream>
#include <climits>

using namespace std;

// Реализация OfficialNode
OfficialNode::OfficialNode(int id, int bribe) 
    : id(id), bribeAmount(bribe), boss(nullptr), 
      firstRequired(nullptr), firstSubordinate(nullptr),
      nextRequired(nullptr), nextSubordinate(nullptr) {}

OfficialNode::~OfficialNode() {
    // Очистка списков выполняется в OfficialHierarchy
}

void OfficialNode::setBoss(OfficialNode* boss) {
    this->boss = boss;
    if (boss) {
        // Добавляем себя в список подчиненных начальника
        nextSubordinate = boss->firstSubordinate;
        boss->firstSubordinate = this;
    }
}

void OfficialNode::addRequiredOfficial(OfficialNode* req) {
    req->nextRequired = firstRequired;
    firstRequired = req;
}

void OfficialNode::addSubordinate(OfficialNode* sub) {
    sub->nextSubordinate = firstSubordinate;
    firstSubordinate = sub;
}

// Геттеры OfficialNode
int OfficialNode::getId() const { return id; }
int OfficialNode::getBribe() const { return bribeAmount; }
OfficialNode* OfficialNode::getBoss() const { return boss; }
OfficialNode* OfficialNode::getFirstRequired() const { return firstRequired; }
OfficialNode* OfficialNode::getFirstSubordinate() const { return firstSubordinate; }
OfficialNode* OfficialNode::getNextRequired() const { return nextRequired; }
OfficialNode* OfficialNode::getNextSubordinate() const { return nextSubordinate; }

// Реализация PathNode
PathNode::PathNode(int id, PathNode* next) : id(id), next(next) {}
PathNode::~PathNode() { delete next; }
int PathNode::getId() const { return id; }
PathNode* PathNode::getNext() const { return next; }
void PathNode::setNext(PathNode* next) { this->next = next; }

// Реализация OfficialHierarchy
OfficialHierarchy::OfficialHierarchy() : root(nullptr), firstOfficial(nullptr) {}
OfficialHierarchy::~OfficialHierarchy() { clear(); }

void OfficialHierarchy::clear() {
    OfficialNode* current = firstOfficial;
    while (current != nullptr) {
        OfficialNode* next = current->getNextSubordinate();
        delete current;
        current = next;
    }
    firstOfficial = nullptr;
    root = nullptr;
}

OfficialNode* OfficialHierarchy::findNode(int id) const {
    OfficialNode* current = firstOfficial;
    while (current != nullptr) {
        if (current->getId() == id) return current;
        current = current->getNextSubordinate();
    }
    return nullptr;
}

void OfficialHierarchy::addRequiredDependencies() {
    // Пример: для подписи 4 нужна подпись 5
    OfficialNode* official4 = findNode(4);
    OfficialNode* official5 = findNode(5);
    if (official4 && official5) {
        official4->addRequiredOfficial(official5);
    }
}

void OfficialHierarchy::buildFromConsole() {
    clear();
    
    int count;
    cout << "Введите количество чиновников (1-100): ";
    cin >> count;
    
    // Создаем чиновников
    for (int i = 1; i <= count; ++i) {
        int bribe;
        cout << "Введите взятку для чиновника " << i << ": ";
        cin >> bribe;
        
        OfficialNode* newOfficial = new OfficialNode(i, bribe);
        newOfficial->addSubordinate(firstOfficial);
        firstOfficial = newOfficial;
    }
    
    // Строим иерархию
    for (int i = 1; i <= count; ++i) {
        int bossId;
        cout << "Введите начальника для чиновника " << i 
             << " (0 для главного): ";
        cin >> bossId;
        
        OfficialNode* official = findNode(i);
        if (bossId == 0) {
            if (root) throw "Главный чиновник уже назначен";
            root = official;
        } else {
            OfficialNode* boss = findNode(bossId);
            if (!boss) throw "Неверный ID начальника";
            official->setBoss(boss);
        }
    }
    
    if (!root) throw "Главный чиновник не назначен";
    
    // Добавляем обязательные зависимости
    addRequiredDependencies();
}

void OfficialHierarchy::findAllPaths(OfficialNode* node, PathNode* path, 
                                  int sum, PathNode*& bestPath, int& minSum) const {
    if (!node) return;

    // Добавляем текущего чиновника в путь
    PathNode* newPath = new PathNode(node->getId(), path);
    sum += node->getBribe();

    // Добавляем обязательные подписи
    OfficialNode* req = node->getFirstRequired();
    while (req != nullptr) {
        findAllPaths(req, newPath, sum, bestPath, minSum);
        req = req->getNextRequired();
    }

    // Если это лист и сумма минимальна - сохраняем путь
    if (node->getFirstSubordinate() == nullptr) {
        if (sum < minSum) {
            minSum = sum;
            delete bestPath;
            bestPath = newPath;
            return;
        }
    }

    // Обрабатываем подчиненных
    OfficialNode* sub = node->getFirstSubordinate();
    while (sub != nullptr) {
        findAllPaths(sub, newPath, sum, bestPath, minSum);
        sub = sub->getNextSubordinate();
    }

    delete newPath; // Удаляем если путь не оптимальный
}

void OfficialHierarchy::printResults(PathNode* path, int sum) const {
    cout << "\nМинимальная сумма взяток: " << sum << " у.е.\n";
    cout << "Оптимальный путь: ";
    
    // Разворачиваем список для вывода
    PathNode* prev = nullptr;
    PathNode* current = path;
    while (current != nullptr) {
        PathNode* next = current->getNext();
        current->setNext(prev);
        prev = current;
        current = next;
    }
    
    // Выводим путь
    current = prev;
    while (current != nullptr) {
        if (current != prev) cout << " -> ";
        cout << current->getId();
        current = current->getNext();
    }
    cout << endl;
}

void OfficialHierarchy::findOptimalBribePath() const {
    if (!root) throw "Иерархия не построена";
    
    PathNode* bestPath = nullptr;
    int minSum = INT_MAX;
    
    findAllPaths(root, nullptr, 0, bestPath, minSum);
    
    if (bestPath) {
        printResults(bestPath, minSum);
        delete bestPath;
    } else {
        cout << "Нет доступных путей" << endl;
    }
}
