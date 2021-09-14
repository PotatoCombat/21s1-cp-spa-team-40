#include "source_processor/design_extractor/DesignExtractor.h"

using namespace std;

DesignExtractor::DesignExtractor(PKB *pkb) : pkb(pkb) {}

void DesignExtractor::extract(Program *program) {
    extractDepthFirst(program);
    extractBreadthFirst(program);
}

void DesignExtractor::extractDepthFirst(Program *program) {
    DepthFirstExtractor extractor(pkb);
    extractor.extract(program);
}

void DesignExtractor::extractBreadthFirst(Program *program) {
    FollowsExtractor extractor(pkb);
    extractor.extract(program);
}
