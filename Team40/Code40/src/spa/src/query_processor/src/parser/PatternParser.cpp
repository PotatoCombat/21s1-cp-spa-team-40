#include "query_processor/parser/PatternParser.h"

PatternParser::PatternParser() {
	this->stmt = "";
	this->var = "";
	this->pattern = "";
}

void PatternParser::initReferences(vector<Reference*>& declList) {
	this->declList = declList;
}

void PatternParser::clear() {
	this->declList = vector<Reference*>();
	this->stmt = "";
	this->var = "";
	this->pattern = "";
}

PatternClause* parse(PatTuple patTuple) {
	stmt = get<0>(clsTuple);
	var = get<1>(clsTuple);
	pattern = get<2>(clsTuple);

}
