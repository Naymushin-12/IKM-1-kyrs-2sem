#include "OfficialHierarchy.h"
#include <iostream>

void printPixelArt() {
    cout << "\n\n";
    cout << "   ^__^\n";
    cout << "  (oo)\\_______\n";
    cout << "  (__)\\       )\\/\\\n";
    cout << "      ||----w |\n";
    cout << "      ||     ||\n";
}

int main() {
    OfficialHierarchy hierarchy;
    
    try {
        hierarchy.buildHierarchy();
        hierarchy.findMinBribeSolution();
        printPixelArt();
    } catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
        return 1;
    }

    return 0;
}