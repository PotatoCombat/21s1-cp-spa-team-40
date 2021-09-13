#include "pkb/StatementTable.h"

using namespace std;

StatementTable::StatementTable() = default;

StmtIndex StatementTable::insert(Statement stmt) {
    size++;
    statements.push_back(stmt);
    indices.push_back(size);
    statementToIndex[&statements.back()] = size;

    switch (stmt.getStatementType()) {
    case StatementType::ASSIGN:
        assignStatements.push_back(stmt.getIndex());
        break;
    case StatementType::READ:
        readStatements.push_back(stmt.getIndex());
        break;
    case StatementType::PRINT:
        printStatements.push_back(stmt.getIndex());
        break;
    case StatementType::IF:
        ifStatements.push_back(stmt.getIndex());
        break;
    case StatementType::WHILE:
        whileStatements.push_back(stmt.getIndex());
        break;
    case StatementType::CALL:
        callStatements.push_back(stmt.getIndex());
        break;
    default:
        throw runtime_error(
            "Cannot store a statement with an unknown statement type");
    }
    return size;
}

Statement *StatementTable::getStmt(StmtIndex index) {
    if (index > size) {
        return NULL;
    }
    return &statements.at(index - 1);
}

StatementType StatementTable::getStmtType(StmtIndex index) {
    if (index > size) {
        return StatementType::UNKNOWN;
    }
    return statements.at(index - 1).getStatementType();
}

StmtIndex StatementTable::getIndex(Statement *stmt) {
    auto search = statementToIndex.find(stmt);
    if (search == statementToIndex.end()) {
        return InvalidIndex;
    }
    return search->second;
}

Iterator<StmtIndex> StatementTable::getIndices() { return {indices}; }

Iterator<StmtIndex> StatementTable::getIndices(StatementType type) {
    switch (type) {
    case StatementType::ASSIGN:
        return {assignStatements};
    case StatementType::READ:
        return {readStatements};
    case StatementType::PRINT:
        return {printStatements};
    case StatementType::IF:
        return {ifStatements};
    case StatementType::WHILE:
        return {whileStatements};
    case StatementType::CALL:
        return {callStatements};
    default:
        throw runtime_error("Cannot fetch an iterator of statements of an "
                            "unknown statement type");
    }
}

int StatementTable::getSize() { return size; }
