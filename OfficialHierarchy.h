// OfficialHierarchy.h
#ifndef OFFICIALHIERARCHY_H
#define OFFICIALHIERARCHY_H

// Класс, представляющий узел односвязного списка (для хранения подчиненных)
class SubordinateNode {
public:
    class OfficialNode* data;    // Указатель на чиновника
    SubordinateNode* next;       // Указатель на следующий узел
    
    SubordinateNode(class OfficialNode* official) : data(official), next(nullptr) {}
};

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
    
    // Возвращает список подчиненных
    SubordinateNode* getSubordinates() const;
    
private:
    int id;                     // Уникальный идентификатор чиновника
    int bribeAmount;            // Сумма требуемой взятки
    SubordinateNode* subordinatesHead; // Голова списка подчиненных
};

// Класс, представляющий узел списка для хранения пути
class PathNode {
public:
    int id;                     // ID чиновника в пути
    PathNode* next;             // Следующий элемент пути
    
    PathNode(int id) : id(id), next(nullptr) {}
};

// Класс, представляющий список путей
class PathList {
public:
    PathNode* head;             // Голова списка
    PathNode* tail;             // Хвост списка (для быстрого добавления)
    
    PathList() : head(nullptr), tail(nullptr) {}
    ~PathList();
    
    void addNode(int id);       // Добавление узла в конец списка
    PathList* clone() const;    // Создание копии списка
};

// Класс, представляющий список списков (все возможные пути)
class PathListCollection {
public:
    class PathListNode {
    public:
        PathList* data;            // Один путь
        PathListNode* next;         // Следующий элемент
        
        PathListNode(PathList* path) : data(path), next(nullptr) {}
    };
    
    PathListNode* head;             // Голова списка
    
    PathListCollection() : head(nullptr) {}
    ~PathListCollection();
    
    void addPath(PathList* path);   // Добавление пути в коллекцию
};

// Класс, представляющий узел списка чиновников
class OfficialListNode {
public:
    OfficialNode* data;         // Указатель на чиновника
    OfficialListNode* next;      // Указатель на следующий узел
    
    OfficialListNode(OfficialNode* official) : data(official), next(nullptr) {}
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
                    PathList* currentPath,
                    PathListCollection& allPaths) const;
    
    // Вывод результатов (минимальной суммы и оптимального пути)
    void printResults(PathList* bestPath, int minBribe) const;

    OfficialListNode* officialsHead; // Голова списка всех чиновников
    OfficialNode* root;             // Корень иерархии (главный чиновник)
};

#endif
