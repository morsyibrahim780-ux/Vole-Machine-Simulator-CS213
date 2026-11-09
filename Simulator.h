#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include "Memory.h"
#include "RegisterFile.h"
#include "ALU.h"

using namespace std;

class Simulator {
private:
    int pc; 
public:
    Simulator() : pc(0) {}

    void reset(int startAddress = 0) {
        pc = startAddress;
    }

    int getPC() const {
        return pc;
    }

    void step(Memory& mem, RegisterFile& regFile, ALU& alu) {
        if (pc < 0 || pc >= 255) return;
        
        string byte1_str = mem.read(pc);
        string byte2_str = mem.read(pc + 1);
        
        unsigned char op1 = stoi(byte1_str, nullptr, 16);
        unsigned char op2 = stoi(byte2_str, nullptr, 16);
        
        unsigned char opcode = (op1 >> 4) & 0x0F;
        unsigned char r = op1 & 0x0F;
        unsigned char xy = op2;
        unsigned char s = (op2 >> 4) & 0x0F;
        unsigned char t = op2 & 0x0F;
        unsigned char x = (op2 >> 4) & 0x0F;

        pc += 2;

        switch (opcode) {
            case 0x1: { 
                string val = mem.read(xy);
                regFile.set(r, stoi(val, nullptr, 16));
                break;
            }
            case 0x2: { 
                regFile.set(r, xy);
                break;
            }
            case 0x3: { 
                unsigned char val = regFile.get(r);
                stringstream ss;
                ss << hex << uppercase << setw(2) << setfill('0') << (int)val;
                mem.write(xy, ss.str());
                break;
            }
            case 0x4: { 
                regFile.set(r, regFile.get(s));
                break;
            }
            case 0x5: { 
                unsigned char valS = regFile.get(s);
                unsigned char valT = regFile.get(t);
                unsigned char res = alu.addInteger(valS, valT);
                regFile.set(r, res);
                break;
            }
            case 0x6: { 
                unsigned char valS = regFile.get(s);
                unsigned char valT = regFile.get(t);
                unsigned char res = alu.addFloat(valS, valT);
                regFile.set(r, res);
                break;
            }
            case 0x7: { 
                unsigned char valS = regFile.get(s);
                unsigned char valT = regFile.get(t);
                unsigned char res = alu.bitwiseOr(valS, valT);
                regFile.set(r, res);
                break;
            }
            case 0x8: { 
                unsigned char valS = regFile.get(s);
                unsigned char valT = regFile.get(t);
                unsigned char res = alu.bitwiseAnd(valS, valT);
                regFile.set(r, res);
                break;
            }
            case 0x9: { 
                unsigned char valS = regFile.get(s);
                unsigned char valT = regFile.get(t);
                unsigned char res = alu.bitwiseXor(valS, valT);
                regFile.set(r, res);
                break;
            }
            case 0xA: { 
                unsigned char val = regFile.get(r);
                unsigned char res = alu.rotateRight(val, x);
                regFile.set(r, res);
                break;
            }
            case 0xB: { 
                if (regFile.get(r) == regFile.get(0)) {
                    pc = xy;
                }
                break;
            }
            case 0xC: { 
                pc = 256; 
                cout << "Halt instruction executed." << endl;
                break;
            }
            default:
                break;
        }
    }

    void run(Memory& mem, RegisterFile& regFile, ALU& alu) {
        pc = 0;
        while (pc < 256) {
            string op1_str = mem.read(pc);
            unsigned char op1 = stoi(op1_str, nullptr, 16);
            unsigned char opcode = (op1 >> 4) & 0x0F;
            if (opcode == 0xC) {
                step(mem, regFile, alu);
                break;
            }
            step(mem, regFile, alu);
        }
    }
};

#endif
