#ifndef CONTROLUNIT_H
#define CONTROLUNIT_H

#include "Memory.h"
#include "RegisterFile.h"
#include "ALU.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <cstdint>

using namespace std;

class controlUnit {

private:
    int pc = 0;
    uint16_t instructionRegister = 0;
    bool halted = false;
    bool error = false;
    string errorMessage;

    void fail(int failingPc, const string& message) {
        error = true;
        errorMessage = message;
        pc = failingPc;
    }

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

    bool hasError() {
        return error;
    }

    int getPC() const { 
        return pc; 
    }

    void displayStatus() {
        cout << hex << uppercase;
        cout << "PC : " << pc
             << " | Instruction : " << setw(4) << setfill('0') << instructionRegister
             << " | Halted: " << (halted ? "True" : "False")
             << " | Error: " << (error ? errorMessage : "None") << endl;
        cout << dec << setfill(' ');
    }

    bool executeNext(Memory& memory, RegisterFile& registers, ALU& alu) {
        if (halted || error) return false;

        if (pc < 0 || pc > 0xFE || (pc % 2) != 0) {
            fail(pc, "PC out of range");
            return false;
        }

        int failingPc = pc;

        uint8_t highByte = memory.readByte(pc);
        uint8_t lowByte  = memory.readByte(pc + 1);
        instructionRegister = (highByte << 8) | lowByte;
        pc += 2;

        int opcode = (instructionRegister >> 12) & 0x0F;
        int r  = (instructionRegister >> 8) & 0x0F;
        int s  = (instructionRegister >> 4) & 0x0F;
        int t  = instructionRegister & 0x0F;
        int xy = instructionRegister & 0xFF;

        switch (opcode) {
            case 0x1:
                registers.set(r, memory.readByte(xy));
                break;

            case 0x2:
                registers.set(r, (uint8_t)xy);
                break;

            case 0x3:
                memory.writeByte(xy, registers.get(r));
                break;

            case 0x4:
                if (r != 0) {
                    fail(failingPc, "Invalid operand for opcode 4");
                    break;
                }
                registers.set(t, registers.get(s));
                break;

            case 0x5:
                registers.set(r, alu.addInteger(registers.get(s), registers.get(t)));
                break;

            case 0x6:
                registers.set(r, alu.addFloat(registers.get(s), registers.get(t)));
                break;

            case 0x7:
                registers.set(r, alu.bitwiseOr(registers.get(s), registers.get(t)));
                break;

            case 0x8:
                registers.set(r, alu.bitwiseAnd(registers.get(s), registers.get(t)));
                break;

            case 0x9:
                registers.set(r, alu.bitwiseXor(registers.get(s), registers.get(t)));
                break;

            case 0xA:
                if (s != 0) {
                    fail(failingPc, "Invalid operand for opcode A");
                    break;
                }
                registers.set(r, alu.rotateRight(registers.get(r), (unsigned char)t));
                break;

            case 0xB:
                if ((xy % 2) != 0 || xy > 0xFE) {
                    fail(failingPc, "Invalid jump address");
                    break;
                }
                if (registers.get(r) == registers.get(0)) {
                    pc = xy;
                }
                break;

            case 0xC:
                if (instructionRegister == 0xC000) {
                    halted = true;
                } else {
                    fail(failingPc, "Invalid C instruction");
                }
                break;

            case 0xD:
                if (s != 0 || t != 0) {
                    fail(failingPc, "Invalid operand for opcode D");
                    break;
                }
                cout << dec << (int)registers.get(r) << endl;
                break;

            default:
                fail(failingPc, "Invalid opcode");
                break;
        }

        return !halted && !error;
    }
};

#endif