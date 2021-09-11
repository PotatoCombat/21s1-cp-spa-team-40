#include "spa/SPA.h"
#include "common/model/Program.h"
#include "source_processor/DesignExtractor.h"
#include "source_processor/Line.h"
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
        PKB pkb = PKB();
        DesignExtractor designExtractor(pkb);
        designExtractor.extract(program);
    } else {
        cout << "No such file";
    }
    file.close();
}
