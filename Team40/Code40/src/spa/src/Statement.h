#pragma once
#include<vector>;
using namespace std;

#pragma once

enum class StatementType {
	READ,
	PRINT,
	ASSIGN,
	CALL,
	WHILE,
	IF
};

class Statement {
private:
	StatementType type;
	int index;
	vector<char> vars;
	vector<int> consts;
public:
	//constructor
	Statement(int index);

	//setters
	void setIndex(int index);
	void setStatementType(StatementType type);
	void addVar(char var);
	void addConst(int const);

	//getters
	int getIndex();
	StatementType getStatementType();
	vector<char> getVars();
	vector<int> getConsts();
};