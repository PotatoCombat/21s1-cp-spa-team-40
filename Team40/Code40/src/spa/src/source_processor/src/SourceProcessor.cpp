#include <fstream>
#include <iostream>
#include <vector>

#include "common/model/Program.h"

#include "source_processor/Line.h"
#include "source_processor/SourceProcessor.h"
#include "source_processor/design_extractor/DesignExtractor.h"

SourceProcessor::SourceProcessor(PKB *pkb) {
    this->pkb = pkb;
    this->parser = Parser();
    this->programParser = ProgramParser();
    this->designExtractor = DesignExtractor(pkb);
}

void SourceProcessor::processSource(string filename) {
    fstream file;
    file.open(filename, ios::in);
    if (file.is_open()) {
        vector<Line> programLines = parser.parseFile(file);
        Program program;
        programParser.parseProgram(programLines, program);
        designExtractor.extract(&program);
    } else {
        cout << "No such file";
    }
    file.close();
}
