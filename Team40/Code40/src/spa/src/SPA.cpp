#include "spa/SPA.h"
#include "common/model/Program.h"
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

void SPA::processSource(string filename) {
    fstream file;
    file.open(filename, ios::in);
    if (file.is_open()) {
        vector<Line> programLines = parser.parseFile(file);
        Program program = programParser.parseProgram(programLines);
    } else {
        cout << "No such file";
    }
    file.close();
}
