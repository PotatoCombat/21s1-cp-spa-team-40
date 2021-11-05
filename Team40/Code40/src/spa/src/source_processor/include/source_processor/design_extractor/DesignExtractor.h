#pragma once

#include "common/model/Program.h"

#include "pkb/PKB.h"

#include "source_processor/design_extractor/BreadthFirstExtractor.h"
#include "source_processor/design_extractor/DepthFirstExtractor.h"
#include "source_processor/design_extractor/NextBipExtractor.h"

class DesignExtractor {
private:
    PKB *pkb;

public:
    explicit DesignExtractor(PKB *pkb);

    void extract(Program *program);
    void extractDepthFirst(Program *program);
    void extractBreadthFirst(Program *program);
    void extractNextBip(Program *program);
};
