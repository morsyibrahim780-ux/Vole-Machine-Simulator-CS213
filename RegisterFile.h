#ifndef REGISTERFILE_H
#define REGISTERFILE_H

#include <iostream>
#include <iomanip>

using namespace std;

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

#endif