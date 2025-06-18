#ifndef OFFICIALHIERARCHY_H  // Защита от повторного включения заголовочного файла
#define OFFICIALHIERARCHY_H

#include <vector>  
#include <string>  

using namespace std;  


class OfficialNode { // Класс, представляющий узел чиновника в иерархии
private:
    int id;                         // Уникальный идентификатор чиновника
    int bribeAmount;                // Сумма взятки, требуемая чиновником
    OfficialNode* boss;             // Указатель на начальника (родительский узел)
    vector<OfficialNode*> subordinates; // Список подчиненных (дочерние узлы)

public:
    // Конструктор узла чиновника
    OfficialNode(int officialId, int bribe);
    
    // Установка начальника для текущего чиновника
    void setBoss(OfficialNode* officialBoss);
    
    // Добавление подчиненного к текущему чиновнику
    void addSubordinate(OfficialNode* subordinate);
    
    // Получение ID чиновника
    int getId() const;
    
    // Получение суммы взятки чиновника
    int getBribe() const;
    
    // Получение указателя на начальника
    OfficialNode* getBoss() const;
    
    // Получение списка подчиненных (константная ссылка)
    const vector<OfficialNode*>& getSubordinates() const;
};



class OfficialHierarchy { //Класс, представляющий иерархию чиновников
private:
    OfficialNode* root;             // Указатель на корневой узел (главный чиновник)
    vector<OfficialNode*> officials; // Вектор всех чиновников в системе

    // Поиск узла чиновника по ID (приватный метод)
    OfficialNode* findNodeById(int id) const;
    
    // Расчет минимальной суммы взяток (рекурсивный приватный метод)
    void calculateMinBribe(OfficialNode* node, int& totalBribe, 
        vector<int>& path) const;
    
    // Вывод пути получения подписей (приватный метод)
    void printPath(const vector<int>& path) const;

public:
    int getValidatedInput(const string& prompt, int minVal, int maxVal, bool allowZero);
    // Конструктор по умолчанию
    OfficialHierarchy();
    
    // Деструктор для очистки памяти
    ~OfficialHierarchy();
    
    // Построение иерархии чиновников (ввод данных от пользователя)
    void buildHierarchy();
    
    // Нахождение решения с минимальной суммой взяток
    void findMinBribeSolution();
    
    // Очистка иерархии и освобождение памяти
    void clearHierarchy();
};

#endif
