#ifndef SCREEN_H
#define SCREEN_H

#include <iostream>
#include <string>

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

#endif