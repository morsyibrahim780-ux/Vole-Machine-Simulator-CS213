#include <iostream>
#include <string>
#include <cstdint>
#include "Screen.h"
#include <iomanip>
#include <cmath>
using namespace std;

class controlUnit {
    
private:
    int pc = 0;
    uint16_t instructionRegister = 0;
    bool halted = false;
    bool error = false;
    string errorMessage;

public:
  
    void reset() {
        pc = 0;
        instructionRegister = 0;
        halted = false;
        error = false;
        errorMessage = "";
    }

    bool isHalted() {
    return halted;
    }

    bool hasError(){
        return error;
    }

    void displayStatus() {
        cout << "PC: " << pc 
             << " | Instruction: " << instructionRegister 
             << " | Halted: " << (halted ? "True" : "False") 
             << " | Error: " << (error ? errorMessage : "None") << endl;
    }

    bool executeNext(Memory& memory, RegisterFile& registers, ALU& alu) {
    if (halted || error) return false;

    instructionRegister = memory.read(pc);
    pc += 2;

    int opcode = (instructionRegister >> 12) & 0x0F;
    int r = (instructionRegister >> 8) & 0x0F;
    int s = (instructionRegister >> 4) & 0x0F;
    int t = instructionRegister & 0x0F;
    int xy = instructionRegister & 0xFF;

    switch (opcode) {
        case 0x1:
            {
                uint8_t memValue = memory.read(xy);
                registers.setRegister(r, memValue);
            }
            break;

        case 0x2:
            registers.setRegister(r, xy);
            break;

        case 0x3:
            {
                uint8_t regValue = registers.getRegister(r);
                memory.write(xy, regValue);
            }
            break;

        case 0xC:
            if (instructionRegister == 0xC000) {
                halted = true;
            } else {
                error = true;
                errorMessage = "halt unsuccessful";
            }
            break;

        default:
            error = true;
            errorMessage = "Error";
            break;
        }

    return !halted && !error;

    }