#pragma once

#include "DepthFirstExtractor.h"
#include "FollowsExtractor.h"
#include "common/model/Program.h"
#include "pkb/PKB.h"

class DesignExtractor {
private:
    PKB *pkb;

public:
    explicit DesignExtractor(PKB *pkb);

    void extract(Program *program);
    void extractDepthFirst(Program *program);
    void extractBreadthFirst(Program *program);
};
