#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "Screen.h"
#include "Memory.h"
#include "RegisterFile.h"
#include "ALU.h"
#include "ControlUnit.h"
#include "ProgramLoader.h"
#include <iostream>
#include <string>

using namespace std;

class Simulator {
private:
    Screen screen;
    Memory memory;
    RegisterFile registers;
    ALU alu;
    controlUnit cpu;
    ProgramLoader loader;

public:
    Simulator() : memory(screen) {}

    void reset() {
        cpu.reset();
    }

    int getPC() const {
        return cpu.getPC();
    }

    void step(Memory& memory, RegisterFile& registers, ALU& alu) {
        cpu.executeNext(memory, registers, alu);
        cpu.displayStatus();
    }

    void run(Memory& memory, RegisterFile& registers, ALU& alu) {
        int steps = 0;
        while (!cpu.isHalted() && !cpu.hasError() && steps < 10000) {
            cpu.executeNext(memory, registers, alu);
            steps++;
        }
        if (steps >= 10000) {
            cout << "Execution stopped: Reached 10,000 step limit.\n";
        }
    }

    void menu() {
        int choice;
        string filename;

        do {
            cout << "\n===== VOLE MACHINE SIMULATOR =====\n";
            cout << "1. Enter Program Manually\n";
            cout << "2. Execute One Instruction (Step)\n";
            cout << "3. Run Entire Program\n";
            cout << "4. Display Registers\n";
            cout << "5. Display Program Counter\n";
            cout << "6. Reset Machine\n";
            cout << "0. Exit\n";
            cout << "\nEnter your choice: ";
            cin >> choice;

            switch (choice) {
            case 1:
                memory.clear();
                registers.clear();
                reset();
                loader.loadProgramManually(memory);
                break;

            case 2:
                step(memory, registers, alu);
                break;

            case 3:
                run(memory, registers, alu);
                break;

            case 4:
                registers.display();
                break;

            case 5:
                cout << "Program Counter: " << hex << uppercase << getPC() << dec << endl;
                break;

            case 6:
                memory.clear();
                registers.clear();
                screen.clear();
                reset();
                cout << "Reset successful" << endl;
                break;

            case 0:
                cout << "Exiting successful" << endl;
                break;

            default:
                cout << "Invalid choice" << endl;
            }

        } while (choice != 0);
    }
};

#endif