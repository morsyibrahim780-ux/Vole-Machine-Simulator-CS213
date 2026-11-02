#ifndef PROGRAM_LOADER_H
#define PROGRAM_LOADER_H

#include <iostream>
#include <fstream>
#include <string>
#include "Memory.h"
using namespace std;

class ProgramLoader {
public:
    void loadProgramFromFile(Memory& mem, string filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Error opening file!" << endl;
            return;
        }
        string instruction;
        int address = 0;
        while (file >> instruction && address < 256) {
            mem.write(address, instruction);
            address++;
        }
        file.close();
        cout << "Program loaded successfully from file." << endl;
    }
};

#endif
