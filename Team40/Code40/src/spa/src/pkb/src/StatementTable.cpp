#include "pkb/StatementTable.h"

StatementTable::StatementTable() = default;

void StatementTable::insert(Statement *stmt) {
    if (stmt->getStatementType() == StatementType::UNKNOWN) {
        throw runtime_error(
                "Cannot store a statement with an unknown statement type");
    }
    size++;
    statements.push_back(stmt);
    indices.push_back(stmt->getIndex());
    indicesByType[stmt->getStatementType()].push_back(stmt->getIndex());
}

Statement *StatementTable::getStmt(const StmtIndex &index) {
    if (index > size) {
        return nullptr;
    }
    return statements.at(index - 1);
}

StatementType StatementTable::getStmtType(const StmtIndex &index) {
    if (index > size) {
        return StatementType::UNKNOWN;
    }
    return statements.at(index - 1)->getStatementType();
}

Iterator<StmtIndex> StatementTable::getIndices() { return {indices}; }

Iterator<StmtIndex> StatementTable::getIndices(const StatementType &type) {
    if (type == StatementType::UNKNOWN) {
        throw runtime_error(
                "Cannot filter statements with an unknown statement type");
    }
    return { indicesByType[type] };
}

size_t StatementTable::getSize() const { return size; }
