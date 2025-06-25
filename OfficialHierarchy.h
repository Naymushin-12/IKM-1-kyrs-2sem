#ifndef OFFICIALHIERARCHY_H
#define OFFICIALHIERARCHY_H

#include <vector>
#include <string>
#include <memory>
#include <unordered_map>

// Класс, представляющий одного чиновника в иерархии
class OfficialNode {
public:
    // Конструктор: создает чиновника с указанным ID и суммой взятки
    OfficialNode(int id, int bribe);
    
    // Устанавливает начальника для текущего чиновника
    void setBoss(OfficialNode* boss);
    
    // Добавляет подчиненного к текущему чиновнику
    void addSubordinate(OfficialNode* subordinate);
    
    // Возвращает ID чиновника
    int getId() const;
    
    // Возвращает сумму взятки для этого чиновника
    int getBribe() const;
    
    // Возвращает список подчиненных (константная ссылка)
    const std::vector<OfficialNode*>& getSubordinates() const;
    
private:
    int id;                         // Уникальный идентификатор чиновника
    int bribeAmount;                // Сумма требуемой взятки
    std::vector<OfficialNode*> subordinates; // Список подчиненных
};

// Класс, представляющий всю иерархию чиновников
class OfficialHierarchy {
public:
    // Конструктор по умолчанию
    OfficialHierarchy();
    
    // Деструктор - освобождает ресурсы
    ~OfficialHierarchy();
    
    // Построение иерархии через консольный ввод
    void buildFromConsole();
    
    // Поиск и вывод оптимального пути с минимальной суммой взяток
    void findOptimalBribePath() const;
    
private:
    // Очистка иерархии (удаление всех чиновников)
    void clear();
    
    // Поиск чиновника по ID (возвращает nullptr если не найден)
    OfficialNode* findNode(int id) const;
    
    // Рекурсивный поиск всех возможных путей подписания документов
    void findAllPaths(OfficialNode* node,
                    std::vector<int>& currentPath,
                    std::vector<std::vector<int>>& allPaths,
                    int currentSum) const;
    
    // Вывод результатов (минимальной суммы и оптимального пути)
    void printResults(const std::vector<int>& bestPath, int minBribe) const;

    std::unordered_map<int, std::unique_ptr<OfficialNode>> officials; // Все чиновники
    OfficialNode* root = nullptr; // Корень иерархии (главный чиновник)
};
#endif
