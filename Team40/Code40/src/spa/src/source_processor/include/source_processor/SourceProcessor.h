#pragma once

#include "pkb/PKB.h"
#include "source_processor/Lexer.h"
#include "source_processor/ProgramParser.h"
#include "source_processor/design_extractor/DesignExtractor.h"

class SourceProcessor {
public:
    explicit SourceProcessor(PKB *pkb);

    void processSource(string filename);

private:
    PKB *pkb;
    Lexer lexer;
    ProgramParser programParser;
    DesignExtractor designExtractor = DesignExtractor(nullptr);
};
