#include <iostream>
#include <string>
#include "Memory.h"
#include "Screen.h"
#include "ProgramLoader.h"
using namespace std;

int main() {
    Memory mem;
    ProgramLoader loader;
    int choice;
    
    do {
        cout << "\n--- Vole Machine Simulator ---" << endl;
        cout << "1. Load Program from File" << endl;
        cout << "2. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        
        if (choice == 1) {
            string filename;
            cout << "Enter filename (e.g., programVole1.txt): ";
            cin >> filename;
            loader.loadProgramFromFile(mem, filename);
        }
    } while (choice != 2);
    
    return 0;
}
