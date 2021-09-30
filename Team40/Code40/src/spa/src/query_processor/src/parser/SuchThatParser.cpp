#include "query_processor/parser/SuchThatParser.h"

SuchThatParser::SuchThatParser() {
	this->type = "";
	this->ref1 = "";
	this->ref2 = "";
}

void SuchThatParser::initReferences(vector<Reference*>& declList) {
	this->declList = declList;
}

void SuchThatParser::clear() {
	this->declList = vector<Reference *>();
	this->type = "";
	this->ref1 = "";
	this->ref2 = "";
}

Clause* SuchThatParser::parse(ClsTuple clsTuple) {
	type = get<0>(clsTuple);
	ref1 = get<1>(clsTuple);
	ref2 = get<2>(clsTuple);

	// choose a parser
}

Clause* SuchThatParser::parseStmtStmt() {

}

Clause* SuchThatParser::parseXEnt() {

}
