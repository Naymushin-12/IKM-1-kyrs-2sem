#include "OfficialHierarchy.h"
#include <iostream>
#include <algorithm>
#include <climits>
#include <stdexcept>

OfficialNode::OfficialNode(int officialId, int bribe) : 
    id(officialId), bribeAmount(bribe), boss(nullptr) {}

void OfficialNode::setBoss(OfficialNode* officialBoss) {
    boss = officialBoss;
}

void OfficialNode::addSubordinate(OfficialNode* subordinate) {
    subordinates.push_back(subordinate);
}

int OfficialNode::getId() const {
    return id;
}

int OfficialNode::getBribe() const {
    return bribeAmount;
}

OfficialNode* OfficialNode::getBoss() const {
    return boss;
}

const vector<OfficialNode*>& OfficialNode::getSubordinates() const {
    return subordinates;
}

OfficialHierarchy::OfficialHierarchy() : root(nullptr) {}

OfficialHierarchy::~OfficialHierarchy() {
    clearHierarchy();
}

OfficialNode* OfficialHierarchy::findNodeById(int id) const {
    for (OfficialNode* official : officials) {
        if (official->getId() == id) {
            return official;
        }
    }
    return nullptr;
}

void OfficialHierarchy::buildHierarchy() {
    int numOfficials;
    cout << "Введите количество чиновников: ";
    cin >> numOfficials;

    if (numOfficials <= 0) {
        throw invalid_argument("Количество чиновников должно быть положительным");
    }

    // Создаем всех чиновников
    for (int i = 1; i <= numOfficials; ++i) {
        int bribe;
        cout << "Введите сумму взятки для чиновника " << i << ": ";
        cin >> bribe;

        if (bribe < 0) {
            throw invalid_argument("Сумма взятки не может быть отрицательной");
        }

        officials.push_back(new OfficialNode(i, bribe));
    }

    // Устанавливаем отношения начальник-подчиненный
    for (int i = 0; i < numOfficials; ++i) {
        int bossId;
        cout << "Введите ID начальника для чиновника " << i+1 << 
            " (0 если это главный чиновник): ";
        cin >> bossId;

        if (bossId < 0 || bossId > numOfficials) {
            throw invalid_argument("Неверный ID начальника");
        }

        if (bossId == 0) {
            if (root != nullptr) {
                throw invalid_argument("Главный чиновник уже назначен");
            }
            root = officials[i];
        } else {
            OfficialNode* bossNode = findNodeById(bossId);
            if (bossNode == nullptr) {
                throw invalid_argument("Начальник с таким ID не найден");
            }
            officials[i]->setBoss(bossNode);
            bossNode->addSubordinate(officials[i]);
        }
    }

    if (root == nullptr) {
        throw invalid_argument("Главный чиновник не назначен");
    }
}

void OfficialHierarchy::calculateMinBribe(OfficialNode* node, int& totalBribe,
    vector<int>& path) const {
    
    if (node == nullptr) {
        return;
    }

    // Если нет подчиненных, просто добавляем этого чиновника
    if (node->getSubordinates().empty()) {
        totalBribe += node->getBribe();
        path.push_back(node->getId());
        return;
    }

    // Находим подчиненного с минимальной взяткой
    int minBribe = INT_MAX;
    OfficialNode* minSubordinate = nullptr;
    
    for (OfficialNode* sub : node->getSubordinates()) {
        if (sub->getBribe() < minBribe) {
            minBribe = sub->getBribe();
            minSubordinate = sub;
        }
    }

    // Сравниваем взятку текущего чиновника и минимального подчиненного
    if (node->getBribe() <= minBribe) {
        totalBribe += node->getBribe();
        path.push_back(node->getId());
    } else {
        totalBribe += minBribe;
        path.push_back(minSubordinate->getId());
        calculateMinBribe(node, totalBribe, path);
    }
}

void OfficialHierarchy::printPath(const vector<int>& path) const {
    cout << "Порядок получения подписей: ";
    for (size_t i = 0; i < path.size(); ++i) {
        if (i != 0) {
            cout << " -> ";
        }
        cout << path[i];
    }
    cout << endl;
}

void OfficialHierarchy::findMinBribeSolution() {
    if (root == nullptr) {
        throw runtime_error("Иерархия не построена");
    }

    int totalBribe = 0;
    vector<int> path;
    calculateMinBribe(root, totalBribe, path);

    cout << "\nМинимальная сумма взяток: " << totalBribe << " у.е." << endl;
    printPath(path);
}

void OfficialHierarchy::clearHierarchy() {
    for (OfficialNode* official : officials) {
        delete official;
    }
    officials.clear();
    root = nullptr;
}