#include <iostream>
#include <string>
#include "Memory.h"
#include "Screen.h"
#include "ProgramLoader.h"
#include "RegisterFile+ALU"
#include "ControlUnit.h"

using namespace std;

int main() {
    Memory mem;
    ProgramLoader loader;
    int choice;

    do {
        cout << "\n--- Vole Machine Simulator ---" << endl;
        cout << "1. Load Program from File" << endl;
        cout << "2. Exit" << endl;
        cout << "Enter your choice: [span_1](start_span)"[span_1](end_span);
        cin >> choice;

        if (choice == 1) {
            string filename;
            cout << "Enter filename (e.g., programVole1.txt): [span_2](start_span)"[span_2](end_span);
            cin >> filename;
            
            loader.loadProgramFromFile(mem, filename)[span_3](start_span)[span_3](end_span);
            
            RegisterFile regFile;
            ALU alu;
            Screen screen;
            ControlUnit controlUnit(mem, regFile, alu, screen);
            
            cout << "\nExecuting Program..." << endl;
            controlUnit.executeProgram();
        }
    } while (choice != 2);

    return 0;
}