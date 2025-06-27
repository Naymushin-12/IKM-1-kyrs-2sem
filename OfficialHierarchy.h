#ifndef OFFICIALHIERARCHY_H
#define OFFICIALHIERARCHY_H

using namespace std;

// Узел, представляющий одного чиновника
class OfficialNode {
public:
    OfficialNode(int id, int bribe);
    ~OfficialNode();
    
    void setBoss(OfficialNode* boss);          // Установить начальника
    void addRequiredOfficial(OfficialNode* req); // Добавить обязательную подпись
    void addSubordinate(OfficialNode* sub);    // Добавить подчиненного
    
    int getId() const;                         // Получить ID
    int getBribe() const;                      // Получить сумму взятки
    OfficialNode* getBoss() const;             // Получить начальника
    OfficialNode* getFirstRequired() const;    // Первый обязательный чиновник
    OfficialNode* getFirstSubordinate() const; // Первый подчиненный
    OfficialNode* getNextRequired() const;     // Следующий обязательный
    OfficialNode* getNextSubordinate() const;  // Следующий подчиненный

private:
    int id;               // Идентификатор чиновника
    int bribeAmount;      // Сумма взятки
    OfficialNode* boss;   // Начальник
    OfficialNode* firstRequired;    // Список обязательных подписей
    OfficialNode* firstSubordinate; // Список подчиненных
    OfficialNode* nextRequired;     // Следующий в списке обязательных
    OfficialNode* nextSubordinate;  // Следующий в списке подчиненных
};

// Узел для хранения пути
class PathNode {
public:
    PathNode(int id, PathNode* next = nullptr);
    ~PathNode();
    
    int getId() const;            // Получить ID чиновника в пути
    PathNode* getNext() const;    // Следующий узел пути
    void setNext(PathNode* next); // Установить следующий узел

private:
    int id;         // ID чиновника
    PathNode* next; // Следующий элемент пути
};

// Класс иерархии чиновников
class OfficialHierarchy {
public:
    OfficialHierarchy();
    ~OfficialHierarchy();
    
    void buildFromConsole();       // Построение иерархии из консоли
    void findOptimalBribePath() const; // Поиск оптимального пути

private:
    void clear();                  // Очистка иерархии
    OfficialNode* findNode(int id) const; // Поиск чиновника по ID
    void findAllPaths(OfficialNode* node, PathNode* path, 
                     int sum, PathNode*& bestPath, int& minSum) const; // Поиск всех путей
    void printResults(PathNode* path, int sum) const; // Вывод результатов
    void addRequiredDependencies(); // Добавление обязательных зависимостей

    OfficialNode* root;           // Главный чиновник
    OfficialNode* firstOfficial;  // Список всех чиновников
};

#endif
