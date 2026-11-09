#ifndef PROGRAM_LOADER_H
#define PROGRAM_LOADER_H

#include <iostream>
#include <fstream>
#include <string>
#include "Memory.h"

using namespace std;

class ProgramLoader {
public:
    void loadProgramFromFile(Memory& mem, const string& filename) {
        mem.clear();

        ifstream file(filename);
        if (!file.is_open()) {
            return;
        }

        string token;
        int address = 0;

        while (file >> token && address < 256) {
            if (token.length() == 4) {
                if (address + 1 < 256) {
                    mem.write(address, token.substr(0, 2));
                    mem.write(address + 1, token.substr(2, 2));
                    address += 2;
                }
            } else if (token.length() <= 2) {
                if (token.length() == 1) {
                    token = "0" + token;
                }
                mem.write(address, token);
                address++;
            }
        }

        file.close();
    }
};

#endif