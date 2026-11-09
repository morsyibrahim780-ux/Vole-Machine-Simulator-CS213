#include <iostream>
#include <string>
#include "Memory.h"

using namespace std;

int main() {

    // Create all simulator components

    Memory memory;
    RegisterFile registers;
    ALU alu;
    ProgramLoader loader;
    Simulator simulator;

    int choice;
    string filename;

    bool programLoaded = false;

    do {

        cout << "\n========== VOLE MACHINE SIMULATOR ==========\n";

        cout << "1. Load Program\n";
        cout << "2. Execute One Instruction\n";
        cout << "3. Run Entire Program\n";
        cout << "4. Display Registers\n";
        cout << "5. Display Program Counter\n";
        cout << "6. Reset Machine\n";
        cout << "0. Exit\n";

        cout << "============================================\n";

        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {

        case 1:

            cout << "Enter program filename: ";
            cin >> filename;

            loader.loadProgramFromFile(memory, filename);

            simulator.reset();
            registers.clear();

            programLoaded = true;

            break;


        case 2:

            if (!programLoaded) {
                cout << "Please load a program first!\n";
                break;
            }

            simulator.step(memory, registers, alu);

            cout << "Instruction executed.\n";

            break;


        case 3:

            if (!programLoaded) {
                cout << "Please load a program first!\n";
                break;
            }

            simulator.run(memory, registers, alu);

            cout << "Program execution finished.\n";

            break;


        case 4:

            registers.display();

            break;


        case 5:

            cout << "Program Counter: "
                 << simulator.getPC() << endl;

            break;


        case 6:

            memory.clear();
            registers.clear();
            simulator.reset();

            programLoaded = false;

            cout << "Machine reset successfully!\n";

            break;


        case 0:

            cout << "Exiting Vole Machine Simulator...\n";

            break;


        default:

            cout << "Invalid choice! Try again.\n";

        }

    } while (choice != 0);


    return 0;
}