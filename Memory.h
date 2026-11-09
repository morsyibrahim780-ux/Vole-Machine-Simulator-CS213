#ifndef MEMORY_H
#define MEMORY_H

#include <iostream>
#include <string>
#include <iomanip>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <sstream>

using namespace std;

class Screen {
private:
    string buffer;
public:
    void show(string text) {
        buffer += text;
        cout << "Output: " << text << endl;
    }
    void clear() {
        buffer = "";
    }
};

class Memory {
private:
    string cells[256];
    Screen& scr;
public:
    Memory(Screen& s) : scr(s) {
        for(int i = 0; i < 256; i++) {
            cells[i] = "00";
        }
    }

    void write(int address, string value) {
        if (address >= 0 && address < 256) {
            cells[address] = value;
            if (address == 255) {
                scr.show(value);
            }
        }
    }

    string read(int address) {
        if (address >= 0 && address < 256) {
            return cells[address];
        }
        return "00";
    }

    uint8_t readByte(int address) {
        string val = read(address);
        return (uint8_t)stoi(val, nullptr, 16);
    }

    void writeByte(int address, uint8_t value) {
        stringstream ss;
        ss << hex << uppercase << setw(2) << setfill('0') << (int)value;
        write(address, ss.str());
    }

    void clear() {
        for(int i = 0; i < 256; i++) {
            cells[i] = "00";
        }
    }
};

class RegisterFile {
private:
    unsigned char registers[16] = {0};
public:
    unsigned char get(int index) const {
        if (index >= 0 && index < 16) return registers[index];
        return 0;     
    }
    
    void set(int index, unsigned char value) {
        if (index >= 0 && index < 16) registers[index] = value;
    }
    
    void clear() {
        for (int i = 0; i < 16; i++) registers[i] = 0;
    }
    
    void display() const {
        cout << "registers Content" << endl;
        for (int i = 0; i < 16; i++) {
            cout << "R" << hex << uppercase << i << ":" << setw(2) << setfill('0') << (int)registers[i] << " ";
            if ((i + 1) % 4 == 0) cout << endl;
        }
        cout << dec << setfill(' ');
    }
};

class ALU {
public:
    unsigned char addInteger(unsigned char a, unsigned char b) const {
        return a + b;
    }
    
    unsigned char bitwiseAnd(unsigned char a, unsigned char b) const {
        return a & b;
    }
    
    unsigned char bitwiseOr(unsigned char a, unsigned char b) const {
        return a | b;
    }
    
    unsigned char bitwiseXor(unsigned char a, unsigned char b) const {
        return a ^ b;
    }
    
    unsigned char rotateRight(unsigned char value, unsigned char amount) const {
        amount %= 8;
        if (amount == 0) return value;
        return (value >> amount) | (value << (8 - amount));

        
    }
    
    double decodeFloat(unsigned char val) const {
        int sign = (val & 0x80) ? -1 : 1;
        int exp = ((val >> 4) & 0x07) - 4;
        int mantissa = val & 0x0F;
        return sign * (mantissa / 16.0) * pow(2.0, exp);
    }
    
    unsigned char encodeFloat(double val) const {
        if (val == 0.0) return 0;
        unsigned char sign = (val < 0) ? 0x80 : 0;
        val = abs(val);
        int exp = 0;
        while (val >= 1.0 && exp < 3) { val /= 2.0; exp++; }
        while (val < 0.5 && exp > -4) { val *= 2.0; exp--; }
        int mantissa = (int)(val * 16.0 + 0.5);
        if (mantissa > 15) mantissa = 15;
        return sign | ((exp + 4) << 4) | mantissa;
    }
    
    unsigned char addFloat(unsigned char a, unsigned char b) const {
        return encodeFloat(decodeFloat(a) + decodeFloat(b));
    }
};

class ProgramLoader {
    private:

        bool loadInstructions(istream& input, Memory& mem) {

            mem.clear();

            string instruction;
            int address = 0;

            while (input >> instruction) {

                if (instruction == "END" || instruction == "end")
                    break;

                // Check instruction length
                if (instruction.length() != 4) {
                    cout << "Invalid instruction: "
                        << instruction << endl;
                    return false;
                }

                // Check hexadecimal format
                try {
                    size_t pos;
                    stoul(instruction, &pos, 16);

                    if (pos != 4)
                        throw invalid_argument("Invalid hex");
                }
                catch (...) {
                    cout << "Invalid hexadecimal instruction: "
                        << instruction << endl;
                    return false;
                }

                // Check memory size
                if (address > 252) {
                    cout << "Program is too large!" << endl;
                    return false;
                }

                // Store instruction as two bytes
                mem.write(address, instruction.substr(0, 2));
                mem.write(address + 1, instruction.substr(2, 2));

                address += 2;
            }

            cout << "Program loaded successfully!" << endl;
            return true;
        }

    public:

        // Load program from file
        void loadProgramFromFile(Memory& mem, string filename) {

            ifstream file(filename);

            if (!file.is_open()) {
                cout << "Error opening file!" << endl;
                return;
            }

            loadInstructions(file, mem);

            file.close();
        }

        // Load program manually
        void loadProgramManually(Memory& mem) {

            cout << "\nEnter your instructions one by one." << endl;
            cout << "Type END when you finish.\n" << endl;

            loadInstructions(cin, mem);
        }
    };

// MOVED controlUnit UP
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
            cout << "1. Load Program From File\n";
            cout << "2. Enter Program Manually\n";
            cout << "3. Execute One Instruction (Step)\n";
            cout << "4. Run Entire Program\n";
            cout << "5. Display Registers\n";
            cout << "6. Display Program Counter\n";
            cout << "7. Reset Machine\n";
            cout << "0. Exit\n";
            cout << "\nEnter your choice: ";
            cin >> choice;

            switch (choice) {
            case 1:
                cout << "Enter file name: ";
                cin >> filename;
                memory.clear();
                registers.clear();
                reset();
                loader.loadProgramFromFile(memory, filename);
                break;

            case 2:
                memory.clear();
                registers.clear();
                reset();
                loader.loadProgramManually(memory);
                break;

            case 3:
                step(memory, registers, alu);
                break;

            case 4:
                run(memory, registers, alu);
                break;

            case 5:
                registers.display();
                break;

            case 6:
                cout << "Program Counter: " << hex << uppercase << getPC() << dec << endl;
                break;

            case 7:
                memory.clear();
                registers.clear();
                screen.clear();
                reset();
                cout << "Machine reset successfully!" << endl;
                break;

            case 0:
                cout << "Exiting Vole Machine Simulator..." << endl;
                break;

            default:
                cout << "Invalid choice!" << endl;
            }

        } while (choice != 0);
    }
};

#endif