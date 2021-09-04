#include "SPA.h"
#include "SourceProcessor/Line.h"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

void SPA::processSource(string filename) {
    fstream file;
    file.open(filename, ios::in);
    if (file.is_open()) {
        vector<Line> programLines = parser.parseFile(file);
    } else {
        cout << "No such file";
    }
    file.close();
}

int main() { // For testing on Fatin's machine only
    SPA spa;
    spa.processSource("C:\\Users\\Admin\\source\\repos\\21s1-cp-spa-team-"
                      "40\\Team40\\Tests40\\Sample_source.txt");
}
