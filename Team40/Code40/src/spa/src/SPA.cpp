#include "SPA.h"
#include <fstream>
#include <iostream>

void SPA::processSource(string filename) {
    fstream file;
    file.open(filename, ios::in);
    if (!file) {
        cout << "No such file";
    } else {
        char ch;
        while (1) {
            file >> ch;
            if (file.eof()) {
                break;
            }
            cout << ch;
        }
    }
    file.close();
}

int main() { // For testing on Fatin's machine only
    SPA spa;
    spa.processSource("C:\\Users\\Admin\\source\\repos\\21s1-cp-spa-team-"
                      "40\\Team40\\Tests40\\Sample_source.txt");
}
