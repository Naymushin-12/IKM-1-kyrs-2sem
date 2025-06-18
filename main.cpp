#include "OfficialHierarchy.h"  
#include <iostream>           

using namespace std;


//Функция для вывода котика
 
void printPixelArt() {
    cout << "\n\n";
    cout << "  /\\_/\\\n";     
    cout << " ( o.o )\n";      
    cout << "  > ^ <\n";       
    cout << "  /   \\\n";      
    cout << " (     )\n";      
    cout << "  \"\"\"\"\"\n";  
}


int main() {
    setlocale(LC_ALL, "ru");
    OfficialHierarchy hierarchy;  // Создание объекта иерархии
    
    try {
        // Построение иерархии чиновников
        hierarchy.buildHierarchy();
        
        // Нахождение и вывод решения с минимальными взятками
        hierarchy.findMinBribeSolution();
        
        printPixelArt();
    } catch (const exception& e) {
        // Обработка и вывод ошибок
        cout << "Ошибка: " << e.what() << endl;
        return 1;  // Возврат кода ошибки
    }

    return 0;  // Успешное завершение
}
