#ifndef PROGRAMLOADER_H
#define PROGRAMLOADER_H

#include "Memory.h"
#include <iostream>
#include <string>
#include <stdexcept>

using namespace std;

class ProgramLoader {
    private:

        bool loadInstructions(istream& input, Memory& memory) {

            memory.clear();

            string instruction;
            int address = 0;

            while (input >> instruction) {

                if (instruction == "END" || instruction == "end"){
                    break;

                }

                if (instruction.length() != 4) {
                    cout << "Invalid instruction: "
                         << instruction << endl;
                    return false;
                }

                try {
                    size_t pos;
                    stoul(instruction, &pos, 16);

                    if (pos != 4)
                        throw invalid_argument("Invalid hex");
                }
                catch (...) {
                    cout << "Invalid instruction "
                         << instruction << endl;
                    return false;
                }

                if (address > 252) {
                    cout << "Program is too large" << endl;
                    return false;
                }

                memory.write(address, instruction.substr(0, 2));
                memory.write(address + 1, instruction.substr(2, 2));

                address += 2;
            }

            cout << "Program loaded successfully" << endl;
            return true;
        }

    public:

        void loadProgramManually(Memory& mem) {

            cout << "\nEnter your instructions one by one as a 4 digits hex code." << endl;
            cout << "Type END when you finish.\n" << endl;

            loadInstructions(cin, mem);
        }
    };

#endif