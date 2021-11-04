#pragma once

#include "pkb/PKB.h"

#include "source_processor/design_extractor/DesignExtractor.h"

#include "source_processor/parser/Lexer.h"
#include "source_processor/parser/Parser.h"

class SourceProcessor {
public:
    explicit SourceProcessor(PKB *pkb);

    void processSource(string filename);

private:
    PKB *pkb;
    Lexer lexer;
    Parser parser;
    DesignExtractor designExtractor = DesignExtractor(nullptr);
};
