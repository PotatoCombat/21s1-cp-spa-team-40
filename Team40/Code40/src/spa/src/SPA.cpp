#include "SPA.h"
#include "SourceProcessor/Line.h"
#include "common/model/ConstantValue.h"
#include "common/model/Program.h"
#include "common/model/Variable.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

void SPA::processSource(string filename) {
    fstream file;
    file.open(filename, ios::in);
    if (file.is_open()) {
        vector<Line> programLines = parser.parseFile(file);
        // Program design entities
        // Program -> list of procedures -> contains a list of statements
        Program program = parser.parseProgram(programLines);
        vector<Variable> varLst = parser.getVarLst();
        vector<ConstantValue> constLst = parser.getConstLst();
        int test = 0; // to bypass debugger
    } else {
        cout << "No such file";
    }
    file.close();
}

int main() { // For testing on Fatin's machine only
    SPA spa;
    spa.processSource("C:\\Users\\Admin\\source\\repos\\21s1-cp-spa-team-"
                      "40\\Team40\\Tests40\\Code1.txt");
}
