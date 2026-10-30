#ifndef SCREEN_H
#define SCREEN_H

#include <iostream>
#include <string>
using namespace std;

class Screen {
public:
    void show(string text) {
        cout << "Output: " << text << endl;
    }
};

#endif
