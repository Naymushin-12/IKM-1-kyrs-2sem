#include "OfficialHierarchy.h"
#include <iostream>

/* Функция для вывода ASCII-арта котика */
void printCat() {
    cout << "\n  /\\_/\\\n ( o.o )\n  > ^ <\n  /   \\\n (     )\n  \"\"\"\"\"\n";
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
        cerr << "Ошибка: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}
