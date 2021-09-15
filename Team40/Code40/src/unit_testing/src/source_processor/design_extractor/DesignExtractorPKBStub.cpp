#include "pkb/PKB.h"

class DesignExtractorPKBStub : public PKB {
    int numProc;
    int numStmt;
    int numVar;
    int numConst;
    int numFollows;
    int numParents;
    int numProcModifiesVar;
    int numStmtModifiesVar;
    int numProcUsesVar;
    int numStmtUsesVar;

public:
    DesignExtractorPKBStub() : numProc(0), numStmt(0), numVar(0), numConst(0){};

    void insertProc(Procedure *proc) override { this->numProc += 1; }
    void insertVar(Variable *variable) override { numVar++; }
    void insertConst(ConstantValue *constant) override { numConst++; }
    StmtIndex insertStmt(Statement *statement) override {
        numStmt++;
        return 0; // STUB
    }
    void insertFollows(Statement *precedingStmt,
                       Statement *followingStmt) override {
        numFollows++;
    }
    void insertParent(Statement *parentStmt, Statement *childStmt) override {
        numParents++;
    }

    /// Stores the relationship Modifies(proc, var).
    void insertProcModifyingVar(Procedure *proc, Variable *var) override {
        numProcModifiesVar++;
    }

    /// Stores the relationship Modifies(stmt, var).
    void insertStmtModifyingVar(Statement *stmt, Variable *var) override {
        numStmtModifiesVar++;
    }

    /// Stores the relationship Uses(proc, var).
    void insertProcUsingVar(Procedure *proc, Variable *var) override {
        numProcUsesVar++;
    }

    /// Stores the relationship Uses(stmt, var).
    void insertStmtUsingVar(Statement *stmt, Variable *var) override {
        numStmtUsesVar++;
    }

    int getNumProc() { return numProc; }
    int getNumStmt() { return numStmt; }
    int getNumVar() { return numVar; }
    int getNumConst() { return numConst; }
    int getNumFollows() { return numFollows; }
    int getNumParents() { return numParents; }
    int getNumProcModifiesVar() { return numProcModifiesVar; }
    int getNumStmtModifiesVar() { return numStmtModifiesVar; }
    int getNumProcUsesVar() { return numProcUsesVar; }
    int getNumStmtUsesVar() { return numStmtUsesVar; }
};
