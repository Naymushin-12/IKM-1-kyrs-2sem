// main.cpp
#include "OfficialHierarchy.h"
#include <iostream>

/* Функция для вывода ASCII-арта котика */
void printCat() {
    cout << "\n\n";
    cout << "  /\\_/\\\n";     
    cout << " ( o.o )\n";      
    cout << "  > ^ <\n";       
    cout << "  /   \\\n";      
    cout << " (     )\n";      
    cout << "  \"\"\"\"\"\n";
}

/* Точка входа в программу */
int main() {
    OfficialHierarchy hierarchy; // Создаем иерархию чиновников
    
    try {
        // 1. Построение иерархии из консольного ввода
        hierarchy.buildFromConsole();
        
        // 2. Поиск и вывод оптимального решения
        hierarchy.findOptimalBribePath();
        
        // 3. Выводим котика как индикатор успешного завершения
        printCat();
    } catch (const exception& e) {
        // Обработка ошибок
        cout << "Ошибка: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}
