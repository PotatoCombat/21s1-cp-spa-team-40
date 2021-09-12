#pragma once

#include "pkb/PKB.h"
#include "source_processor/Parser.h"
#include "source_processor/ProgramParser.h"
#include "source_processor/design-extractor/DesignExtractor.h"

class SourceProcessor {
public:
    explicit SourceProcessor(PKB *pkb);

    void processSource(string filename);

private:
    PKB *pkb;
    Parser parser;
    ProgramParser programParser;
    DesignExtractor designExtractor = DesignExtractor(nullptr);
};
