#include <pkb/Abstractions.h>

class ExtractionContext {
private:
    static ExtractionContext *instance;
    StmtIndex prevStatement;
    StmtIndex parentStatement;
    ProcIndex currentProc;

    ExtractionContext() = default;

public:
    ExtractionContext(ExtractionContext const &) = delete;
    void operator=(ExtractionContext const &) = delete;
    static ExtractionContext &getInstance();
    StmtIndex getPrevStatement();
    StmtIndex getParentStatement();
    ProcIndex getCurrentProc();

    void setPrevStatement(StmtIndex stmtIndex);
    void setParentStatement(StmtIndex stmtIndex);
    void setCurrentProc(ProcIndex procIndex);
};
