#include "common/model/Program.h"
#include "pkb/PKB.h"
#include "source_processor/ExtractionContext.h"

class ParentsExtractor {
private:
    PKB pkb;
    EntityContext<Statement> ctx;

public:
    ParentsExtractor(PKB pkb);

    void handleProgram(Program program);
    void handleStatement(Statement *statement);
};