//#include "spa/SPA.h"

// #include "common/model/ConstantValue.h"
// #include "common/model/Program.h"
// #include "common/model/Variable.h"

// #include "source_processor/Line.h"

#include <fstream>
#include <iostream>
#include <vector>

#include "../include/spa/SPA.h"

// #include "common/model/ConstantValue.h"
// #include "common/model/Program.h"
// #include "common/model/Variable.h"

#include "common/include/common/model/ConstantValue.h"
#include "common/include/common/model/Program.h"
#include "common/include/common/model/Variable.h"

// #include "source_processor/Line.h"

#include "source_processor/include/source_processor/Line.h"

using namespace std;

void SPA::processSource(string filename) {
    fstream file;
    file.open(filename, ios::in);
    if (file.is_open()) {
        vector<Line> programLines = parser.parseFile(file);
        // Program design entities
        // Program -> list of procedures -> contains a list of statements
        Program program = parser.parseProgram(programLines);
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
