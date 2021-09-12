#pragma once

#include "EntityTable.h"
#include "common/model/ConstantValue.h"

using namespace std;

class ConstTable : public EntityTable<ConstantValue, ConstName> {
public:
    ConstTable();

    /// Stores the pointer \param constant in the table, and indexes the constant.
    /// \return index of the \param constant in the table.
    void insert(ConstantValue *constant) override;

    /// Returns an iterator for all the variable names stored in the table.
    Iterator<int> getValues();

private:
    vector<int> values;
};
