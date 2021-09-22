#include <fstream>
#include <iostream>
#include <vector>

#include "common/model/Program.h"
#include "source_processor/SourceProcessor.h"
#include "source_processor/parser/Line.h"

SourceProcessor::SourceProcessor(PKB *pkb) {
    this->pkb = pkb;
    this->lexer = Lexer();
    this->parser = Parser();
    this->designExtractor = DesignExtractor(pkb);
}

void SourceProcessor::processSource(string filename) {
    fstream file;
    file.open(filename, ios::in);
    if (file.is_open()) {
        try {
            vector<Line> programLines = lexer.tokenizeFile(file);
            Program program = parser.parseProgram(programLines);
            designExtractor.extract(&program);
        } catch (const exception &e) {
            cout << e.what();
            exit(1);
        }
    } else {
        cout << "No such file";
    }
    file.close();
}
