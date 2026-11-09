#ifndef ALU_H
#define ALU_H

#include <cmath>

using namespace std;

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

#endif