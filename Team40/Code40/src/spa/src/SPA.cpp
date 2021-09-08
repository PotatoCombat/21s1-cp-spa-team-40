#include "spa/SPA.h"
#include "common/model/ConstantValue.h"
#include "common/model/Program.h"
#include "common/model/Variable.h"
#include "source_processor/Line.h"
#include <fstream>
#include <iostream>
#include <vector>

void SPA::processSource(string filename) {
    fstream file;
    file.open(filename, ios::in);
    if (file.is_open()) {
        vector<Line> programLines = parser.parseFile(file);
        Program program = parser.parseProgram(programLines);
    } else {
        cout << "No such file";
    }
    file.close();
}
