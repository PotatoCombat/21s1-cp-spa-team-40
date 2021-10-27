#include "pkb/StatementTable.h"

StatementTable::StatementTable() = default;

void StatementTable::insert(Statement *stmt) {
    if (stmt->getStatementType() == StatementType::UNKNOWN) {
        throw runtime_error(
                "Cannot store a statement with an unknown statement type");
    }
    size++;
    statements.push_back(stmt);
    indices.push_back(stmt->getId());
    indicesByType[stmt->getStatementType()].push_back(stmt->getId());
}

Statement *StatementTable::getStmt(const StmtIndex &index) {
    if (index > size || index <= 0) {
        return nullptr;
    }
    return statements.at(index);
}

StatementType StatementTable::getStmtType(const StmtIndex &index) {
    if (index > size || index <= 0) {
        return StatementType::UNKNOWN;
    }
    return statements.at(index)->getStatementType();
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
